#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "Setup.h"

#include <flecs.h>

// Components
struct PositionComponent {
	int y, x;
};

struct DirectionComponent {
	int y, x;
};

struct CharComponent {
	char ch;
};

struct ColorComponent {
	int color;
};

// Tags
struct TakesInputTag { };
struct FollowsAITag { };

// Functions
int getRandomDirection() {
	return rand() % 3 - 1;
}

// Global Input var
char ch;

int main() {

	srand(time(NULL));

	flecs::world world;

	// Coponent Registration
	world.component<PositionComponent>();
	world.component<DirectionComponent>();
	world.component<CharComponent>();
	world.component<ColorComponent>();

	world.component<TakesInputTag>();
	world.component<FollowsAITag>();

	// HandleInput System
	flecs::system handleInputSystem = world.system<PositionComponent, DirectionComponent, TakesInputTag>("HandleInputSystem")
		.kind(flecs::OnLoad)
		.each([](PositionComponent& pos, DirectionComponent& dir, TakesInputTag& in) {
		dir.y = 0; dir.x = 0;
		
		switch(ch) {
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
	flecs::system moveSystem = world.system<PositionComponent, DirectionComponent, FollowsAITag>("MoveRandomSystem")
		.each([](PositionComponent& pos, DirectionComponent& dir, FollowsAITag& fai) {
		pos.y += dir.y + getRandomDirection();
		pos.x += dir.x + getRandomDirection();
	});
	
	// Print System
	flecs::system printSystem = world.system<PositionComponent, CharComponent, ColorComponent>("PrintSystem")
		.kind(flecs::OnStore)
		.each([](PositionComponent& pos, CharComponent& cha, ColorComponent& c) {
		mvaddch(pos.y, pos.x, cha.ch | c.color);
	});

	// Entities
	const int MAX_ENTITIES = 30;
	for (int i = 0; i < MAX_ENTITIES; i++) {
		world.entity()
			.insert([](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c) {
			pos = { rand() % 30, rand() % 100 };
			dir = { 0, 0 };
			cha = { '@' };
			c	= { COLOR_PAIR( GREEN_BLACK ) };
		}).add(world.component<FollowsAITag>());
	}

	// Player Entity
	auto player = world.entity("Player")
		.insert([](PositionComponent& pos, DirectionComponent& dir, CharComponent& cha, ColorComponent& c) {
		pos = { rand() % 30, rand() % 100 };
		dir = { 0, 0 };
		cha = { '@' };
		c	= { COLOR_PAIR(WHITE_BLACK) };
	});
	player.add(world.component<TakesInputTag>());
	
	// Cureses Setup
	cursesSetup();

	//char ch2 = '@;
	mvprintw(2, 5, "Hello from curses!");
	//mvaddch(3, 5, ch2);

	printSystem.run();

	while (ch = getch()) {
		if (ch == 'q') break;
		
		clear();

		world.progress(); // Advances the world by 1 tick (Runs all systems once)

		//moveSystem.run();
		//printSystem.run();

	}

	endwin();

	return 0;
}



// Move System Iteration
		//world.system("MoveSystem")
		//	.run([](flecs::iter& it) {
		//	while (it.next()) {
		//		auto p = it.field<PositionComponent>(0);
		//		auto d = it.field<DirectionComponent>(1);
		//
		//		for (auto i : it) {
		//			p[i].y += d[i].y;
		//			p[i].x += d[i].x;
		//		}
		//	}
		//});

		// Print System Iteration
		//world.system("PrintSystem")
		//	.run([](flecs::iter& it) {
		//	while (it.next()) {
		//		auto p = it.field<PositionComponent>(0);
		//		auto c = it.field<CharComponent>(1);
		//
		//		for (auto i : it) {
		//			mvprintw(p[i].y, p[i].x, c[i].ch.c_str());
		//		}
		//	}
		//});