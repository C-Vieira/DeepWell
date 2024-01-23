#include <rogue.h>

bool cursesSetup(void){
    initscr();
    noecho();
    curs_set(0);

    if(has_colors()){
        start_color();

        init_pair(VISIBLE_COLOR, COLOR_WHITE, COLOR_BLACK); //blue
        init_pair(SEEN_COLOR, COLOR_BLUE, COLOR_BLACK); //white
        init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK); //red
        init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK); //yellow
        init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK); //green
        init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK); //black
        init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK); //cyan
        init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK); //magenta

        return true;
    }else{
        mvprintw(20, 50, "Your system doesn't support color. Can't start game!");
+       getch();

        return false;
    }
}

void gameLoop(void){
    int ch;

    makeFOV(player);
    drawEverything();
    showFloorMessage();

    while(ch = getch()){
        if(ch == 'q') break;

        else if (ch == 'r') { //debug
            floorCount++;
            resetMap();
            showFloorMessage();
        }

        handleInput(ch);
        moveEnemies();

        if ((player->pos.y == exit_pos.y) && (player->pos.x == exit_pos.x)) { //next level
            floorCount++;
            resetMap();
            showFloorMessage();
        }
        else if (map[player->pos.y][player->pos.x].ch == '_') { //over an altar
            applyAltarEffect();
        }
        else if (map[player->pos.y][player->pos.x].ch == 'v') { //stepped on a trap
            activateTrap();
        }

        if(player->xpAmount >= player->amountForNextLevel) levelUP();

        if (defeatedFinalBoss) { //victory
            drawVictoryScreen();
            getch();
            break;
        }

        if (player->health <= 0) { //defeat
            getch();
            break;
        }

        drawEverything();
    }
}

void closeGame(void){
    endwin();
    free(player);

    EntityList* p = enemies; //free enemy list
    while(p->next != NULL){
        p = p->next;
        free(enemies);
        enemies = p;
    }
}
