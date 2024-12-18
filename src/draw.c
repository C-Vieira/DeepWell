#include <rogue.h>

// Windows

WINDOW* createWindow(int height, int width, int starty, int startx) {
    WINDOW* newWindow;

    newWindow = newwin(height, width, starty, startx);
    box(newWindow, 0, 0);

    wrefresh(newWindow);

    return newWindow;
}

void clearWindow(void) {
    touchwin(mainWin);
    touchwin(hudWin);
}

void drawWindow(void) {
    wrefresh(mainWin);
    wrefresh(hudWin);
    doupdate();
}

// ------------------------------------------------------------------------------------------------

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

// HUD

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

// Menus

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

// ------------------------------------------------------------------------------------------------

void drawEverything(void){
    clear();
    //clearWindow();
    drawMap();
    drawEntity(player);

    EntityList* p = enemies;
    while(p != NULL){
        drawEntity(p->entity); //draws all enemies
        p = p->next;
    }

    drawStats();
    scanNearbyEntities();
    //drawWindow();
}

// Messages

void showAttackBlockedMessage(Entity* target) {
    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
    mvprintw(26, 75, "%s blocked the attack!", target->name); //log message
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    getch();
}

void showCriticalHitMessage(void) {
    attron(COLOR_PAIR(YELLOW_BLACK));
    mvprintw(27, 75, "Critical hit!"); //log message
    attroff(COLOR_PAIR(YELLOW_BLACK));
}

void showHitMessage(Entity* target, Entity* attacker) {
    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
    mvprintw(26, 75, "%s scored a hit on %s!", attacker->name, target->name); //log message
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    getch();
}

void showDefeatedMessage(Entity* target) {
    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
    mvprintw(26, 75, "%s was defeated!", target->name); //log message
    mvprintw(27, 75, "                                                    ");
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    getch();
}

void showPlayerNameRequest(void) {
    mvprintw(23, 35, "                                               ");
    mvprintw(24, 35, "                                               ");
    mvprintw(23, 56, "Who am I? ");
    refresh();
}

void showLevelUPMessage(void) {
    attron(COLOR_PAIR(GREEN_BLACK));
    mvprintw(27, 75, "LEVEL UP!");
    attroff(COLOR_PAIR(GREEN_BLACK));
    getch();
}

void showBadAltarMessage(void) {
    attron(COLOR_PAIR(RED_BLACK));
    mvprintw(26, 75, "The altar trembles before you");
    mvprintw(27, 75, "-20 HP!");
    attroff(COLOR_PAIR(RED_BLACK));
    getch();
}

void showGoodAltarMessage(void) {
    attron(COLOR_PAIR(GREEN_BLACK));
    mvprintw(26, 75, "The altar glows with a dim light");
    mvprintw(27, 75, "+50 HP!");
    attroff(COLOR_PAIR(GREEN_BLACK));
    getch();
}

// Scans and shows entities in a radius around the player
void scanNearbyEntities(void) {
    int radius = 10, y = 26;

    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 5, "NEARBY:");
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    EntityList* p = enemies;
    while (p != NULL) {
        if (getDistance(p->entity->pos, player->pos) < radius && lineOfSight(p->entity->pos, player->pos)) {
            mvaddch(++y, 5, p->entity->ch | p->entity->color);
            mvprintw(y, 6, ": %s                                ", p->entity->name);
        }
        p = p->next;
    }
}

void showFloorMessage(void) {
    switch (floorCount)
    {
    case 0:
        mvprintw(0, 0, "The door is shut, it's only down from here ...");
        getch();
        break;
    case 1:
        mvprintw(0, 0, "You can hear flies buzzing about ...");
        getch();
        break;
    case 6:
        mvprintw(0, 0, "A chill runs down your spine ...");
        getch();
        break;
    case 11:
        mvprintw(0, 0, "You're beyond return ...");
        getch();
        break;
    case 16:
        mvprintw(0, 0, "We were not meant to get here ...");
        getch();
        break;
    case 21:
        mvprintw(0, 0, "Chaos incarnate Cthulhu awakens !");
        getch();
        break;
    default:
        break;
    }
}
