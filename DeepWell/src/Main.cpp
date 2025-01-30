#include "Engine.h"

// Entry Point
int main() {

	srand(time(NULL));

	flecs::world world;
	ecsSetup(world);

	bool compatibleTerminal = cursesSetup();

	if (compatibleTerminal) {

		int op;
		do {
			world.add<ActiveScene, MenuScene>();
			op = getch();
			clear();

			if (op != 'q') {
				world.add<ActiveScene, GameScene>();
				doGameLoop(world);
			}
				

		} while (op != 'q');

		closeGame();

	}
	else {
		endwin();
	}

	return 0;

}