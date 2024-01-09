#include <rogue.h>

Tile** createMapTiles(void){
    Tile** tiles = calloc(MAP_HEIGHT, sizeof(Tile*));

    for(int y = 0; y < MAP_HEIGHT; y++){
        tiles[y] = calloc(MAP_WIDTH, sizeof(Tile));
        for(int x = 0; x < MAP_WIDTH; x++){
            tiles[y][x].ch = '#';
            tiles[y][x].color = COLOR_PAIR(VISIBLE_COLOR);
            tiles[y][x].walkable = false;
            tiles[y][x].transparent = false;
            tiles[y][x].visible = false;
            tiles[y][x].seen = false;
            tiles[y][x].attr = A_NORMAL;
        }
    }

    return tiles;
}

Position setupMap(void){
    if (floorCount == 0) {
        return setupStartRoom();
    }
    else if (floorCount > FINAL_FLOOR) {
        return setupFinalArena();
    }

    int y, x, height, width, n_rooms;
    n_rooms = (rand() % 11) + 5;
    Room* rooms = calloc(n_rooms, sizeof(Room));
    Position start_pos;

    for(int i = 0; i < n_rooms; i++){
        y = (rand() % (MAP_HEIGHT - 10)) + 1;
        x = (rand() % (MAP_WIDTH - 20)) + 1;
        height = (rand() % 7) + 3;
        width = (rand() % 15) + 5;
        rooms[i] = createRoom(y, x, height, width);
        addRoomToMap(rooms[i]);

        if(i > 0){
            connectRoomCenters(rooms[i-1].center, rooms[i].center);
        }
    }
    //place an exit at the center of last room
    addExit(rooms[n_rooms - 1]);
    exit_pos.y = rooms[n_rooms - 1].center.y;
    exit_pos.x = rooms[n_rooms - 1].center.x;

    if(floorCount%2 == 0) //only on even floors
        addAltar(rooms[(int)(n_rooms/2)]); //add an altar to one of the rooms

    start_pos.y = rooms[0].center.y;
    start_pos.x = rooms[0].center.x;

    free(rooms);
    
    return start_pos;
}

Position setupStartRoom(void) {

    Room startRoom;
    Position start_pos;

    startRoom = createStartRoom(10, 35, 7, 11);

    //place an exit at the center of the room
    addExit(startRoom);
    exit_pos.y = startRoom.center.y;
    exit_pos.x = startRoom.center.x;

    start_pos.y = startRoom.center.y + 2;
    start_pos.x = startRoom.center.x;
    
    return start_pos;
}

Position setupFinalArena(void) {

    Room finalArena;
    Position start_pos;

    finalArena = createFinalArena(10, 35, 11, 33);

    start_pos.y = finalArena.center.y + 3;
    start_pos.x = finalArena.center.x;

    return start_pos;
}

void freeMap(void){
    for(int y = 0; y < MAP_HEIGHT; y++)
        free(map[y]);
    free(map);
}

void resetMap(void){
    Position newStart_pos;
    clearAllEnemies();
    freeMap();
    map = createMapTiles();
    newStart_pos = setupMap();
    player->pos = newStart_pos;
    makeFOV(player);
    drawEverything();
}
