#include "Scene.h"

void resetScene(flecs::world& world) {
	ecs_defer_begin(world);
	world.delete_with(flecs::ChildOf, world.entity<SceneRoot>());
	ecs_defer_end(world);
}

void initScenes(flecs::world& world) {
	world.component<ActiveScene>().add(flecs::Exclusive);

	flecs::entity menu = world.pipeline().with(flecs::System).without<GameScene>().build();

	flecs::entity game = world.pipeline().with(flecs::System).without<MenuScene>().build();

	world.set<MenuScene>({ menu });
	world.set<GameScene>({ game });

	world.observer<ActiveScene>("Scene Changed to Menu")
		.event(flecs::OnAdd)
		.second<MenuScene>()
		.each(setupMenuScene);

	world.observer<ActiveScene>("Scene Changed to Game")
		.event(flecs::OnAdd)
		.second<GameScene>()
		.each(setupGameScene);
}

void setupMenuScene(flecs::iter& it, size_t, ActiveScene) {
	flecs::world world = it.world();
	flecs::entity scene = world.component<SceneRoot>();

	resetScene(world);

	clear();
	mvprintw((LINES / 2) - 1, (COLS - 29) / 2, " - - -  MENU  SCREEN  - - - ");
	mvprintw(LINES / 2, (COLS - 29) / 2, " - Press any key to start - ");

	world.set_pipeline(world.get<MenuScene>()->pip);
}

void setupGameScene(flecs::iter& it, size_t, ActiveScene) {
	flecs::world world = it.world();
	flecs::entity scene = world.component<SceneRoot>();

	resetScene(world);

	// Map Tiles
	setupMapTileEntities(world, scene);

	flecs::query<PositionComponent, WalkableComponent> q = world.query<PositionComponent, WalkableComponent>("CollisionQuery");

	// Test Entities
	const int MAX_ENTITIES = 30;
	for (int i = 0; i < MAX_ENTITIES; i++) {
		world.entity()
			.insert([&](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c) {

			//flecs::entity validSpawnPos;

			//do {
			//	int randY = rand() % LINES;
			//	int randX = rand() % COLS;
			//
			//	validSpawnPos = q.find([&](PositionComponent& p, WalkableComponent& w) {
			//		return ((p.y == randY) && (p.x == randX) && w.walkable);
			//		});
			//
			//	if (validSpawnPos) {
			//		pos = { randY, randX };
			//	}
			//
			//} while (!validSpawnPos);

			pos = { rand() % LINES, rand() % COLS };

			dir = { 0, 0 };
			cha = { '@' };
			c = { COLOR_PAIR(GREEN_BLACK) };
			})
			.child_of(scene)
			.add(world.component<FollowsAITag>());
	}

	world.set_pipeline(world.get<GameScene>()->pip);
}