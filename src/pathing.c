#include <rogue.h>

//move an enemy entity randomly in four directions
void moveRandom(Entity* enemy) {
    if (enemy->immobile) return;

    Position newPos = { enemy->pos.y, enemy->pos.x };

    int randDir = rand() % 4;
    switch (randDir) {
    case 0:
        newPos.y--; //up
        break;
    case 1:
        newPos.x--; //left
        break;
    case 2:
        newPos.y++; //down
        break;
    case 3:
        newPos.x++; //right
        break;
    default:
        break;
    }

    if (map[newPos.y][newPos.x].walkable) {
        if ((newPos.y == player->pos.y) && (newPos.x == player->pos.x)) //attack player, don't move
            attackEntity(player, enemy);
        else {
            enemy->pos.y = newPos.y;
            enemy->pos.x = newPos.x;
        }
    }
}

//move an enemy in the direction of a given target
void followTarget(Entity* enemy, Entity* target) {
    if (enemy->immobile) return;

    Position newPos = { enemy->pos.y, enemy->pos.x };
    int radius = 10;

    /*This produces very janky results...
    Enemies do find their way towards their target, but end up preferring to move right/left and avoid up/down whenever possible,
    resulting in a "crab-like" strafing pattern and sometimes getting stuck.
    Should probably swap this out for a better pathfinding algorithm..
    */

    if (getDistance(enemy->pos, target->pos) < radius && lineOfSight(enemy->pos, target->pos)) { //only follow if within radius and line of sight
        enemy->color = pickEnemyColorFromType(enemy->type);
        enemy->seen = true;
        if (abs((enemy->pos.x - 1) - target->pos.x) < abs(enemy->pos.x - target->pos.x) && map[enemy->pos.y][enemy->pos.x - 1].walkable) //step left
            newPos.x--;
        else if (abs((enemy->pos.x + 1) - target->pos.x) < abs(enemy->pos.x - target->pos.x) && map[enemy->pos.y][enemy->pos.x + 1].walkable) //step right
            newPos.x++;
        else if (abs((enemy->pos.y - 1) - target->pos.y) < abs(enemy->pos.y - target->pos.y) && map[enemy->pos.y - 1][enemy->pos.x].walkable) //step up
            newPos.y--;
        else if (abs((enemy->pos.y + 1) - target->pos.y) < abs(enemy->pos.y - target->pos.y) && map[enemy->pos.y + 1][enemy->pos.x].walkable) //step left
            newPos.y++;

        if ((newPos.y == player->pos.y) && (newPos.x == player->pos.x)) //attack player, don't move
            attackEntity(player, enemy);
        else if (checkForTarget(newPos) == NULL) { //enemies body block each other
            enemy->pos.y = newPos.y;
            enemy->pos.x = newPos.x;
        }
    }
    else {
        if (enemy->seen) { //move at random otherwise
            enemy->color = COLOR_PAIR(SEEN_COLOR);
            moveRandom(enemy);
        }
        else {
            enemy->color = COLOR_PAIR(BLACK_BLACK);
            moveRandom(enemy);
        }
    }
}

void seekTarget(Entity* enemy, Entity* target) {
    if (enemy->immobile) return;

    Position start = { enemy->pos.y, enemy->pos.x };
    Position goal = { target->pos.y, target->pos.x };
    Position newPos = { enemy->pos.y, enemy->pos.x };

    int radius = 5;
    
    //add neighborhood to tile list
    Position temp;
    for (int y = -1; y < 2; y++) {
        temp.y = start.y + y;
        for (int x = -1; x < 2; x++) {
            temp.x = start.x + x;
            if(map[temp.y][temp.x].walkable && isInMap(temp.y, temp.x)) addTile(temp); //only add tile if walkable and inside the map
        }
    }
   
    //remove the start position
    removeTile(start);

    if (getDistance(enemy->pos, target->pos) < radius && lineOfSight(enemy->pos, target->pos)) { //only follow if within radius and line of sight
        enemy->color = pickEnemyColorFromType(enemy->type);
        enemy->seen = true;
        
        newPos = getLowestCostFromNeighbors(start, goal); //get the position that bring us closest to the target

        //debug
        //mvprintw(0, 0, "%s moved to y:%d x:%d", enemy->name, newPos.y, newPos.x);
        //refresh();
        //getch();

        if ((newPos.y == player->pos.y) && (newPos.x == player->pos.x)) { //attack player, don't move

            Position tempX = { enemy->pos.y, player->pos.x };
            Position tempY = { player->pos.y, enemy->pos.x };

            if ((enemy->pos.y != player->pos.y) && (enemy->pos.x != player->pos.x)) { //if diagonal
                if (checkForTarget(tempX) == NULL && map[tempX.y][tempX.x].walkable) enemy->pos.x = player->pos.x; //align on x if empty
                else if (checkForTarget(tempY) == NULL && map[tempY.y][tempY.x].walkable) enemy->pos.y = player->pos.y; //align on y if empty

                clearAllTiles();
                return;
            }

            attackEntity(player, enemy); //attack player
        }
        else if (checkForTarget(newPos) == NULL) { //enemies body block each other
            enemy->pos.y = newPos.y;
            enemy->pos.x = newPos.x;
        }
    }
    else {
        if (enemy->seen) { //move at random otherwise
            enemy->color = COLOR_PAIR(SEEN_COLOR);
            moveRandom(enemy);
        }
        else {
            enemy->color = COLOR_PAIR(BLACK_BLACK);
            moveRandom(enemy);
        }
    }

    clearAllTiles();
}

//add a tile to the front of a tile list
void addTile(Position tilePos) {
    TileList* newEntry = calloc(1, sizeof(TileList));
    newEntry->tilePos = tilePos;
    newEntry->next = neighbors;

    neighbors = newEntry;
}

//empty out a tile list
void clearAllTiles(void) {
    TileList* p = neighbors;
    while (neighbors != NULL) {
        neighbors = neighbors->next;
        free(p);
        p = neighbors;
    }
}

//remove a given tile from a tile list
void removeTile(Position target) {
    if ((neighbors->tilePos.y == target.y && neighbors->tilePos.x == target.x) ||
        ((neighbors->tilePos.y == target.y && neighbors->tilePos.x == target.x) && neighbors->next == NULL)) { //found at first entry
        TileList* p = neighbors;
        neighbors = neighbors->next;
        free(p);
        return;
    }

    TileList* aux = neighbors;
    while (aux->next != NULL) {
        if (aux->next->tilePos.y == target.y && aux->next->tilePos.x == target.x) {
            TileList* p = aux->next;
            aux->next = aux->next->next;
            free(p);
            return;
        }
        aux = aux->next;
    }
    return;
}

Position getLowestCostFromNeighbors(Position start, Position goal) {

    int lowest = INT_MAX, fCost = 0, gCost = 0, hCost = 0;
    Position current, found = start;

    TileList* p = neighbors;
    while (p != NULL) {
        current = p->tilePos;

        gCost = getDistance(start, current) * 10;
        hCost = getDistance(current, goal) * 10;
        fCost = gCost + hCost;

        if (fCost < lowest) {
            lowest = fCost;
            found = current;
        }

        //debug
        /*mvprintw(0, 0, "Current - y:%d x:%d", current.y, current.x);
        mvprintw(1, 0, "Found - y:%d x:%d", found.y, found.x);
        mvprintw(2, 0, "Player - y:%d x:%d", player->pos.y, player->pos.x);
        mvprintw(3, 0, "Goal - y:%d x:%d", goal.y, goal.x);
        refresh();
        getch();*/

        p = p->next;
    }

    return found;
}