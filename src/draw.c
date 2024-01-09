#include <rogue.h>

void drawMap(void){
    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if (map[y][x].visible){
                mvaddch(y, x, map[y][x].ch | map[y][x].color | map[y][x].attr);
            }
            else if (map[y][x].seen){
                mvaddch(y, x, map[y][x].ch | COLOR_PAIR(SEEN_COLOR) | map[y][x].attr);
            }
            else{
                mvaddch(y, x, ' ');
            }
        }
    }
}

void drawEntity(Entity* entity){
    mvaddch(entity->pos.y, entity->pos.x, entity->ch | entity->color);
}

void drawStats(void){
    attron(COLOR_PAIR(RED_BLACK));
    mvprintw(25, 5, "HP: %d", player->health);
    attroff(COLOR_PAIR(RED_BLACK));
    attron(COLOR_PAIR(YELLOW_BLACK));
    mvprintw(25, 15, "ATK: %d", player->damage);
    attroff(COLOR_PAIR(YELLOW_BLACK));
    attron(COLOR_PAIR(SEEN_COLOR));
    mvprintw(25, 25, "DEF: %d", player->defense);
    attron(COLOR_PAIR(SEEN_COLOR));
    attron(COLOR_PAIR(GREEN_BLACK));
    mvprintw(25, 35, "LVL: %d (%d/%d)", player->level, player->xpAmount, player->amountForNextLevel);
    attron(COLOR_PAIR(GREEN_BLACK));
    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(25, 75, "FLOOR: %d - %s", floorCount, selectFloorName());
    attroff(COLOR_PAIR(VISIBLE_COLOR));
}

void drawMenu(void){
    clear();
    attron(COLOR_PAIR(SEEN_COLOR));
    mvprintw(10, 35, "$$$$$$$\\                                $$\\      $$\\           $$\\ $$\\  ");
    mvprintw(11, 35, "$$  __$$\\                               $$ | $\\  $$ |          $$ |$$ | ");
    mvprintw(12, 35, "$$ |  $$ | $$$$$$\\   $$$$$$\\   $$$$$$\\  $$ |$$$\\ $$ | $$$$$$\\  $$ |$$ | ");
    mvprintw(13, 35, "$$ |  $$ |$$  __$$\\ $$  __$$\\ $$  __$$\\ $$ $$ $$\\$$ |$$  __$$\\ $$ |$$ | ");
    mvprintw(14, 35, "$$ |  $$ |$$$$$$$$ |$$$$$$$$ |$$ /  $$ |$$$$  _$$$$ |$$$$$$$$ |$$ |$$ | ");
    mvprintw(15, 35, "$$ |  $$ |$$   ____|$$   ____|$$ |  $$ |$$$  / \\$$$ |$$   ____|$$ |$$ | ");
    mvprintw(16, 35, "$$$$$$$  |\\$$$$$$$\\ \\$$$$$$$\\ $$$$$$$  |$$  /   \\$$ |\\$$$$$$$\\ $$ |$$ | ");
    mvprintw(17, 35, "\\_______/  \\_______| \\_______|$$  ____/ \\__/     \\__| \\_______|\\__|\\__| ");
    mvprintw(18, 35, "                              $$ |                                      ");
    mvprintw(19, 35, "                              $$ |                                      ");
    mvprintw(20, 35, "                              \\__|                                      ");
    mvprintw(21, 35, "                                                                        ");
    mvprintw(22, 35, "                                                                        ");
    mvprintw(23, 35, "                      PRESS ANY KEY TO START                            ");
    mvprintw(24, 35, "                            QUIT (q)                                    ");
    attroff(COLOR_PAIR(SEEN_COLOR));
}

void drawVictoryScreen(void){
    clear();
    attron(COLOR_PAIR(GREEN_BLACK));
    mvprintw(6, 35, " . . . V I C T O R Y ! . . .  ");
    mvprintw(7, 35, "                              ");
    mvprintw(8, 35, "        ___________           ");
    mvprintw(9, 35, "       '._==_==_=_.'          ");
    mvprintw(10, 35, "    . -\\:        /- .       ");
    mvprintw(11, 35, "    | (| :.      | ) |       ");
    mvprintw(12, 35, "     '-|:.       |- '        ");
    mvprintw(13, 35, "       \\::.     /           ");
    mvprintw(14, 35, "        '::.  .'             ");
    mvprintw(15, 35, "          )  (               ");
    mvprintw(16, 35, "        _.'  '._             ");
    mvprintw(17, 35, "       `********`            ");
    mvprintw(18, 35, "                             ");
    mvprintw(19, 35, "      CONGRATULATIONS !      ");
    mvprintw(20, 35, " YOU HAVE BRAVED THE CHASMS  ");
    mvprintw(21, 35, "BELOW AND CAME OUT VICTORIOUS");
    attroff(COLOR_PAIR(GREEN_BLACK));
}

void drawDefeatScreen(char* lastAttacker){
    clear();
    attron(COLOR_PAIR(RED_BLACK));
    mvprintw(10, 35, ". . . D E F E A T . . .");
    mvprintw(11, 35, "                       ");
    mvprintw(12, 35, "      -----------      ");
    mvprintw(13, 35, "     /           \\    ");
    mvprintw(14, 35, "    |  HERE LIES  |    ");
    mvprintw(15, 35, "       %s              ", player->name);
    mvprintw(16, 35, "    |             |    ");
    mvprintw(17, 35, "    |  FELLED BY  |    ");
    mvprintw(18, 35, "       %s              ", lastAttacker);
    mvprintw(19, 35, "    |             |    ");
    mvprintw(20, 35, "   *|             |*   ");
    mvprintw(21, 35, "  NMNMNMNMNMNMNMNMNNM  ");
    attroff(COLOR_PAIR(RED_BLACK));
}

void drawEverything(void){
    clear();
    drawMap();
    drawEntity(player);

    EntityList* p = enemies;
    while(p != NULL){
        drawEntity(p->entity); //draws all enemies
        p = p->next;
    }

    drawStats();
    scanNearbyEntities();
}
