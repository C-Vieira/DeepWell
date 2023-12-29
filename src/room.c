#include <rogue.h>

Room createRoom(int y, int x, int height, int width){
    Room newRoom;

    newRoom.pos.y = y;
    newRoom.pos.x = x;
    newRoom.height = height;
    newRoom.width = width;
    newRoom.center.y = y + (int)(height/2);
    newRoom.center.x = x + (int)(width/2);

    addEnemiesToRoom(newRoom);

    return newRoom;
}

void addEnemiesToRoom(Room newRoom) {
    //define a random amount of enemies in the room
    int n_enemies = rand() % 3;

    //define a random position for enemy spawn
    Position randPos;
    randPos.y = newRoom.center.y + ((rand() % 2) + 1);
    randPos.x = newRoom.center.x + ((rand() % 2) + 1);

    //define a random type of enemy to spawn
    int randType = selectEnemiesFromPool();

    //create and add enemies to the global enemy list
    for (int i = 0; i < n_enemies; i++) {
        switch (randType)
        {
        case FLY:
            addEnemy(createEnemy(randPos, FLY, "Fly", 'f', COLOR_PAIR(BLACK_BLACK), 5, 1, 0, false, 5));
            break;
        case WORM:
            addEnemy(createEnemy(randPos, WORM, "Worm", 's', COLOR_PAIR(BLACK_BLACK), 10, 3, 2, false, 10));
            break;
        case RAT:
            addEnemy(createEnemy(randPos, RAT, "Rat", 'r', COLOR_PAIR(BLACK_BLACK), 14, 5, 3, false, 20));
            break;
        case RAT_KING:
            addEnemy(createEnemy(randPos, RAT_KING, "Rat King", 'K', COLOR_PAIR(BLACK_BLACK), 20, 12, 5, false, 35));
            break;
        case CAVE_SPIDER:
            addEnemy(createEnemy(randPos, CAVE_SPIDER, "Cave Spider", 'x', COLOR_PAIR(BLACK_BLACK), 25, 18, 8, false, 25));
            break;
        case BAT:
            addEnemy(createEnemy(randPos, BAT, "Bat", 'b', COLOR_PAIR(BLACK_BLACK), 32, 24, 12, false, 35));
            break;
        case GHOUL:
            addEnemy(createEnemy(randPos, GHOUL, "Ghoul", 'g', COLOR_PAIR(BLACK_BLACK), 38, 28, 16, false, 40));
            break;
        case CENTIPEDE:
            addEnemy(createEnemy(randPos, CENTIPEDE, "Centipede", 'c', COLOR_PAIR(BLACK_BLACK), 45, 32, 20, false, 50));
            break;
        case OOZE:
            addEnemy(createEnemy(randPos, OOZE, "Ooze", 'o', COLOR_PAIR(BLACK_BLACK), 50, 38, 22, false, 45));
            break;
        case DEEP_ONE:
            addEnemy(createEnemy(randPos, DEEP_ONE, "Deep One", 'd', COLOR_PAIR(BLACK_BLACK), 58, 42, 32, false, 50));
            break;
        case DAGON:
            addEnemy(createEnemy(randPos, DAGON, "Dagon", 'D', COLOR_PAIR(BLACK_BLACK), 72, 68, 54, false, 100));
            break;
        case STAR_SPAWN:
            addEnemy(createEnemy(randPos, STAR_SPAWN, "Star Spawn", 'S', COLOR_PAIR(BLACK_BLACK), 75, 68, 58, false, 110));
            break;
        case COSMIC_HORROR:
            addEnemy(createEnemy(randPos, COSMIC_HORROR, "Cosmic Horror", 'H', COLOR_PAIR(BLACK_BLACK), 82, 80, 80, false, 120));
            break;
        case CTHULHU:
            addEnemy(createEnemy(randPos, CTHULHU, "Cthulhu", 'C', COLOR_PAIR(BLACK_BLACK), 1000, 200, 250, false, 1000));
            break;
        default:
            addEnemy(createEnemy(randPos, EMPTY, "", ' ', COLOR_PAIR(BLACK_BLACK), 0, 0, 0, true, 0));
            break;
        }
    }
}

Room createStartRoom(int y, int x, int height, int width) {
    Room newRoom;

    newRoom.pos.y = y;
    newRoom.pos.x = x;
    newRoom.height = height;
    newRoom.width = width;
    newRoom.center.y = y + (int)(height/2);
    newRoom.center.x = x + (int)(width/2);

    //middle
    for (int y = newRoom.pos.y; y < newRoom.pos.y + newRoom.height; y++) {
        for (int x = newRoom.pos.x; x < newRoom.pos.x + newRoom.width; x++) {
            map[y][x].ch = ' ';
            map[y][x].walkable = true;
            map[y][x].transparent = true;
        }
    }
    //left
    for (int y = newRoom.pos.y; y < newRoom.pos.y + newRoom.height; y++) {
        map[y][x].ch = '+';
        map[y][x].color = COLOR_PAIR(YELLOW_BLACK);
        map[y][x].walkable = false;
        map[y][x].transparent = false;
    }
    //right
    for (int y = newRoom.pos.y; y < newRoom.pos.y + newRoom.height; y++) {
        map[y][newRoom.pos.x + newRoom.width - 1].ch = '+';
        map[y][newRoom.pos.x + newRoom.width - 1].color = COLOR_PAIR(YELLOW_BLACK);
        map[y][newRoom.pos.x + newRoom.width - 1].walkable = false;
        map[y][newRoom.pos.x + newRoom.width - 1].transparent = false;
    }
    //top
    for (int x = newRoom.pos.x; x < newRoom.pos.x + newRoom.width; x++) {
        map[y][x].ch = '+';
        map[y][x].color = COLOR_PAIR(YELLOW_BLACK);
        map[y][x].walkable = false;
        map[y][x].transparent = false;
    }
    //bottom
    for (int x = newRoom.pos.x; x < newRoom.pos.x + newRoom.width; x++) {
        map[newRoom.pos.y + newRoom.height - 1][x].ch = '+';
        map[newRoom.pos.y + newRoom.height - 1][x].color = COLOR_PAIR(YELLOW_BLACK);
        map[newRoom.pos.y + newRoom.height - 1][x].walkable = false;
        map[newRoom.pos.y + newRoom.height - 1][x].transparent = false;
    }
    //corners
    map[y][x].ch = 'o';
    map[newRoom.pos.y + newRoom.height - 1][x].ch = 'o';
    map[y][newRoom.pos.x + newRoom.width - 1].ch = 'o';
    map[newRoom.pos.y + newRoom.height - 1][newRoom.pos.x + newRoom.width - 1].ch = 'o';
    //door
    map[newRoom.pos.y + newRoom.height - 1][newRoom.center.x].ch = '\\';

    return newRoom;
}

Room createFinalArena(int y, int x, int height, int width) {
    Room newRoom;

    newRoom.pos.y = y;
    newRoom.pos.x = x;
    newRoom.height = height;
    newRoom.width = width;
    newRoom.center.y = y + (int)(height / 2);
    newRoom.center.x = x + (int)(width / 2);
    
    for (int y = newRoom.pos.y; y < newRoom.pos.y + newRoom.height; y++) {
        for (int x = newRoom.pos.x; x < newRoom.pos.x + newRoom.width; x++) {
            map[y][x].ch = ' ';
            map[y][x].walkable = true;
            map[y][x].transparent = true;
        }
    }

    addEnemy(createEnemy(newRoom.center, CTHULHU, "Cthulhu", 'C', COLOR_PAIR(BLACK_BLACK), 1000, 200, 100, false, 1000));

    return newRoom;
}

void addRoomToMap(Room room){
    for(int y = room.pos.y; y < room.pos.y + room.height; y++){
        for(int x = room.pos.x; x < room.pos.x + room.width; x++){
            map[y][x].ch = ' ';
            map[y][x].walkable = true;
            map[y][x].transparent = true;
        }
    }
}

void addExit(Room room){
    map[room.center.y][room.center.x].ch = '[';
    map[room.center.y][room.center.x].color = COLOR_PAIR(GREEN_BLACK);
}

void connectRoomCenters(Position centerOne, Position centerTwo){
    Position temp;
    temp.x = centerOne.x;
    temp.y = centerOne.y;

    while(true){
        if(abs((temp.x - 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
            temp.x--;
        else if(abs((temp.x + 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
            temp.x++;
        else if(abs((temp.y + 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
            temp.y++;
        else if(abs((temp.y - 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
            temp.y--;
        else
            break;

        map[temp.y][temp.x].ch = ' ';
        map[temp.y][temp.x].walkable = true;
        map[temp.y][temp.x].transparent = true;
    }
}
