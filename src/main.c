#include <rogue.h>

const int MAP_HEIGHT = 25;
const int MAP_WIDTH = 100;

Entity* player;
EntityList* enemies = NULL;
Tile** map;
int floorCount = 0;
Position exit_pos;
bool defeatedFinalBoss = false;
TileList* neighbors = NULL;

int main(void){

    Position start_pos;
    bool compatibleTerminal;

    compatibleTerminal = cursesSetup();

    srand(time(NULL));

    if(compatibleTerminal){

        map = createMapTiles();
        start_pos = setupMap();
        player = createPlayer(start_pos);

        int op;
        do {
            drawMenu();
            op = getch();

            if (op != 'q') {
                getPlayerName();
                gameLoop();
            }

            //clean up after defeat/victory
            floorCount = 0;
            defeatedFinalBoss = false;
            resetMap();
            player = createPlayer(player->pos);

        } while (op != 'q');

        closeGame();

    }else{
        endwin();
    }

    return 0;
}
