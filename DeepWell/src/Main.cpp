#include "Engine.h"

// Entry Point
int main() {

	srand(time(NULL));

	flecs::world world;
	ECSSetup(world);

	bool compatibleTerminal = cursesSetup();

	if (compatibleTerminal) {

		int op;
		do {
			// "Simulates Menu Screen"
			clear();
			mvprintw(2, 5, "Menu Screen");
			mvprintw(3, 5, " - Welcome!");
			op = getch();
			clear();

			if(op != 'q')
				doGameLoop(world);

		} while (op != 'q');

		closeGame();

	}
	else {
		endwin();
	}

	return 0;

}