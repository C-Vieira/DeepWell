#include "Engine.h"

flecs::system printSystem;

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

    // Tags
    world.component<TakesInputTag>();
    world.component<FollowsAITag>();
}

static void setupSystems(flecs::world world) {
    // HandleInput System
    world.system<PositionComponent, DirectionComponent, KeyInputComponent>("HandleInputSystem")
        .kind(flecs::OnLoad)
        .with<TakesInputTag>()
        .each([](PositionComponent& pos, DirectionComponent& dir, KeyInputComponent& kin) {
        dir.y = 0; dir.x = 0;

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

        pos.y += dir.y;
        pos.x += dir.x;
    });

    // MoveRandom System
    world.system<PositionComponent, DirectionComponent>("MoveRandomSystem")
        .with<FollowsAITag>()
        .each([](PositionComponent& pos, DirectionComponent& dir) {
        pos.y += dir.y + getRandomDirection();
        pos.x += dir.x + getRandomDirection();
    });

    // Print System
    printSystem = world.system<PositionComponent, CharComponent, ColorComponent>("PrintSystem")
        .kind(flecs::OnStore)
        .each([](PositionComponent& pos, CharComponent& cha, ColorComponent& c) {
        mvaddch(pos.y, pos.x, cha.ch | c.color);
    });
}

static void setupEntities(flecs::world world) {
    // Player Entity
    world.entity("Player")
        .insert([](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c, KeyInputComponent& kin) {
        pos = { rand() % 30, rand() % 100 };
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
            pos = { rand() % 30, rand() % 100 };
            dir = { 0, 0 };
            cha = { '@' };
            c = { COLOR_PAIR(GREEN_BLACK) };
        }).add(world.component<FollowsAITag>());
    }
}

void ECSSetup(flecs::world world)
{
    
    registerComponents(world);
    setupSystems(world);
    setupEntities(world);

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
