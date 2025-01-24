#include "Engine.h"

using CollisionQuery = flecs::query<PositionComponent, WalkableComponent>;

flecs::system printSystem;
flecs::system printTileSystem;

// Helper Function
static int getRandomDirection() {
    return rand() % 3 - 1;
}

static void registerComponents(flecs::world world) {
    // Coponent Registration
    world.component<PositionComponent>();
    world.component<DirectionComponent>();
    world.component<CharComponent>();
    world.component<ColorComponent>();
    world.component<KeyInputComponent>();
    world.component<WalkableComponent>();

    // Tags
    world.component<TakesInputTag>();
    world.component<FollowsAITag>();
    world.component<TileEntityTag>();
}

static void setupSystems(flecs::world world) {
    // Query for finding a tile entity at a position
    CollisionQuery q = world.query<PositionComponent, WalkableComponent>("CollisionQuery");

    // HandleInput System
    world.system<DirectionComponent, KeyInputComponent>("HandleInputSystem")
        .kind(flecs::OnLoad)
        .with<TakesInputTag>()
        .each([](DirectionComponent& dir, KeyInputComponent& kin) {

        switch (kin.key) {
        case 'w':
            dir.y = -1;
            break;
        case 'a':
            dir.x = -1;
            break;
        case 's':
            dir.y = 1;
            break;
        case 'd':
            dir.x = 1;
            break;
        default:
            break;
        }

    });

    // RandomDir System
    world.system<DirectionComponent>("RandomDirSystem")
        .kind(flecs::PreUpdate)
        .with<FollowsAITag>()
        .each([](DirectionComponent & dir) {
        // Set random direction between -1 and 1
        dir.y = getRandomDirection();
        dir.x = getRandomDirection();
    });

    // Move System
    world.system<PositionComponent, DirectionComponent>("MoveSystem")
        .each([q](PositionComponent& pos, DirectionComponent& dir) {
        
        // Look for the tile at the target position
        flecs::entity e = q.find([&](PositionComponent& p, WalkableComponent& w) {
            return ((p.y == (pos.y + dir.y)) && (p.x == (pos.x + dir.x)) && w.walkable);
        });

        // Found a walkable tile
        if (e) {
            // Can move
            pos.y += dir.y;
            pos.x += dir.x;
        }
        
        dir.y = 0; dir.x = 0;
    });

    // PrintTile System
    printTileSystem = world.system<PositionComponent, CharComponent, ColorComponent>("PrintTileSystem")
        .kind(flecs::PostUpdate)
        .with<TileEntityTag>()
        .each([](PositionComponent& pos, CharComponent& cha, ColorComponent& c) {
        mvaddch(pos.y, pos.x, cha.ch | c.color);
    });

    // Print System
    printSystem = world.system<PositionComponent, CharComponent, ColorComponent>("PrintSystem")
        .kind(flecs::OnStore)
        .without<TileEntityTag>()
        .each([](PositionComponent& pos, CharComponent& cha, ColorComponent& c) {
        mvaddch(pos.y, pos.x, cha.ch | c.color);
    });
}

static void setupEntities(flecs::world world) {
    // Player Entity
    world.entity("Player")
        .insert([](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c, KeyInputComponent& kin) {
        pos = { 5, 5 };
        dir = { 0, 0 };
        cha = { '@' };
        c = { COLOR_PAIR(WHITE_BLACK) };
        kin = { ' ' };
    }).add(world.component<TakesInputTag>());

    // Test Entities
    const int MAX_ENTITIES = 30;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        world.entity()
            .insert([](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c) {
            pos = { rand() % 30, rand() % 80 };
            dir = { 0, 0 };
            cha = { '@' };
            c = { COLOR_PAIR(GREEN_BLACK) };
        }).add(world.component<FollowsAITag>());
    }
}

void ECSSetup(flecs::world world)
{
    
    registerComponents(world);
    setupEntities(world);
    setupSystems(world);

}

bool cursesSetup() {
    initscr();
    noecho();
    curs_set(0);

    if (has_colors()) {
        start_color();

        init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK); //white
        init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK); //blue
        init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK); //red
        init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK); //yellow
        init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK); //green
        init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK); //black
        init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK); //cyan
        init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK); //magenta

        return true;
    }
    else {
        mvprintw(20, 50, "Your system doesn't support color. Can't start game!");
        getch();

        return false;
    }
}

void doGameLoop(flecs::world world)
{
    setupMapTileEntities(world);

    //flecs::query<PositionComponent, WalkableComponent> q = world.query<PositionComponent, WalkableComponent>();
    //flecs::entity e = q.find([](PositionComponent& p, WalkableComponent& w) {
    //    return (p.y == 10 && p.x == 10) && w.walkable;
    //});
    //
    //if (e) {
    //    mvprintw(5, 5, "Tile Found !");
    //}
    //else {
    //    mvprintw(5, 5, "Tile not Found ...");
    //}
    //getch();

    printTileSystem.run();
    printSystem.run();

    flecs::entity player = world.lookup("Player");

    while (player.set(KeyInputComponent { getch() })) {
        if (player.get<KeyInputComponent>()->key == 'q') break;

        clear();

        world.progress(); // Advances the world by 1 tick (Runs all systems once)

    }
}

void closeGame()
{
    endwin();
}
