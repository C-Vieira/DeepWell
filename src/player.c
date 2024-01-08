#include <rogue.h>

Entity* createPlayer(Position start_pos){
    Entity* newPlayer = calloc(1, sizeof(Entity));

    newPlayer->pos.y = start_pos.y;
    newPlayer->pos.x = start_pos.x;
    newPlayer->type = PLAYER;
    newPlayer->name = "Player";
    newPlayer->ch = '@';
    newPlayer->color = COLOR_PAIR(VISIBLE_COLOR);
    newPlayer->health = 100;
    newPlayer->damage = 5;
    newPlayer->defense = 0;
    newPlayer->immobile = false;
    newPlayer->seen = true;
    newPlayer->level = 1;
    newPlayer->xpAmount = 0;
    newPlayer->amountForNextLevel = 100;

    return newPlayer;
}

void handleInput(int input){

    Position newPos = {player->pos.y, player->pos.x};

    switch(input){
    case 'w':
        newPos.y--; //up
        break;
    case 'a':
        newPos.x--; //left
        break;
    case 's':
        newPos.y++; //down
        break;
    case 'd':
        newPos.x++; //right
        break;
    default:
        break;
    }

    movePlayer(newPos);
}

void movePlayer(Position newPos){
    if(map[newPos.y][newPos.x].walkable){
        if(checkForTarget(newPos) != NULL) //check for an enemy at the target position
            attackEntity(checkForTarget(newPos), player); //attack the enemy
        else{
            clearFOV(player);
            player->pos.y = newPos.y;
            player->pos.x = newPos.x;
            makeFOV(player);
        }
    }
}

void getPlayerName(void) {
    echo();
    char* newName;
    mvprintw(23, 35, "                                               ");
    mvprintw(24, 35, "                                               ");
    mvprintw(23, 56, "Who am I? ");
    refresh();
    getstr(newName);
    player->name = newName;
    noecho();
}

void levelUP(void){
    player->health += ((player->level%10) + 10);
    player->damage += ((player->level%10) + 2);
    player->defense += ((player->level%10) + 2);
    player->level += 1;
    player->xpAmount = 0;
    player->amountForNextLevel = pow(player->level, 2) + (50 * player->level) + 50;
    attron(COLOR_PAIR(GREEN_BLACK));
    mvprintw(27, 75, "LEVEL UP!");
    attroff(COLOR_PAIR(GREEN_BLACK));
    getch();
}

//scans and shows entities in a radius around the player
void scanNearbyEntities(void) {
    int radius = 10, y = 26;

    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 5, "NEARBY:");
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    EntityList* p = enemies;
    while (p != NULL) {
        if (getDistance(p->entity->pos, player->pos) < radius && lineOfSight(p->entity->pos, player->pos)){
            mvaddch(++y, 5, p->entity->ch | p->entity->color);
            mvprintw(y, 6, ": %s                                ", p->entity->name);
        }
        p = p->next;
    }
}