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
    showPlayerNameRequest();
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
    
    showLevelUPMessage();
}

// Unused
void activateTrap(void) {
    player->health -= 10; //lose 10 hp
    if (player->health < 0) player->health = 1; //do not let the player die

    attron(COLOR_PAIR(RED_BLACK));
    mvprintw(26, 75, "%s sprung a trap!", player->name);
    mvprintw(27, 75, "-10 HP!");
    attroff(COLOR_PAIR(RED_BLACK));
    getch();
    map[player->pos.y][player->pos.x].ch = '^';
    map[player->pos.y][player->pos.x].attr = A_NORMAL;
    map[player->pos.y][player->pos.x].color = COLOR_PAIR(RED_BLACK);
}

void applyAltarEffect(void) {
    if ((rand() % 10) != 9) {
        player->health += 50; //heal 50 hp
        
        showGoodAltarMessage();
    }
    else {
        player->health -= 20; //lose 20 hp
        if (player->health < 0) player->health = 1; //do not let the player die

        showBadAltarMessage();
    }
    map[player->pos.y][player->pos.x].ch = ' ';
    map[player->pos.y][player->pos.x].attr = A_NORMAL;
}
