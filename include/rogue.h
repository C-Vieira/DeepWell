#ifndef ROGUE_H
#define ROGUE_H

#include <C:\MinGW\include\curses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define INT_MAX 2147483647

//color pairs
#define VISIBLE_COLOR 1
#define SEEN_COLOR 2
#define RED_BLACK 3
#define YELLOW_BLACK 4
#define GREEN_BLACK 5
#define BLACK_BLACK 6
#define CYAN_BLACK 7
#define MAGENTA_BLACK 8

//enemy types
#define PLAYER -2
#define EMPTY -1
#define CORPSE 0
#define FLY 1
#define WORM 2
#define RAT 3
#define RAT_KING 4
#define LIVING_FLESH 5
#define CAVE_SPIDER 6
#define BAT 7
#define GHOUL 8
#define CENTIPEDE 9
#define OOZE 10
#define DEEP_ONE 11
#define DAGON 12
#define STAR_SPAWN 13
#define COSMIC_HORROR 14
#define CTHULHU 15

//floors
#define START_FLOOR 0
#define FINAL_FLOOR 20

typedef struct{
int y;
int x;
}Position;

typedef struct{
char ch;
int color;
bool walkable;
bool transparent;
bool visible;
bool seen;
int attr;
}Tile;

typedef struct TileList {
Position tilePos;
struct TileList* next;
}TileList;

typedef struct{
Position pos;
int type;
char* name;
char ch;
int color;
int health;
int damage;
int defense;
bool immobile;
bool seen;
int level;
int xpAmount;
int amountForNextLevel;
}Entity;

typedef struct EntityList{
Entity* entity;
struct EntityList* next;
}EntityList;

typedef struct{
int height;
int width;
Position pos;
Position center;
}Room;

//draw.c
void drawMap(void);
void drawEntity(Entity* entity);
void drawMenu(void);
void drawVictoryScreen(void);
void drawDefeatScreen(char* lastAttacker);
void drawStats(void);
void drawEverything(void);

//enemy.c functions
void addEnemy(Entity* enemy);
void clearAllEnemies(void);
void removeEnemy(Entity* target);
int pickEnemyColorFromType(int type);
void attackEntity(Entity* target, Entity* attacker);
void spawnCorpse(Entity* entity);
Entity* createEnemy(Position pos, int type, char* name, char symbol, int color, int health, int damage, int defense, bool isImmobile, int xpAmount);
Entity* checkForTarget(Position target_pos);
void moveEnemies(void);

//engine.c functions
bool cursesSetup(void);
void gameLoop(void);
void closeGame(void);

//floor.c functions
int selectEnemiesFromPool(void);
char* selectFloorName(void);
void showFloorMessage(void);

//map.c functions
Tile** createMapTiles(void);
Position setupMap(void);
Position setupStartRoom(void);
Position setupFinalArena(void);
void freeMap(void);
void resetMap(void);

//pathing functions
void moveRandom(Entity* enemy);
void followTarget(Entity* enemy, Entity* target);
void seekTarget(Entity* enemy, Entity* target);
void addTile(Position tilePos);
void clearAllTiles(void);
void removeTile(Position target);
Position getLowestCostFromNeighbors(Position start, Position goal);

//player.c functions
Entity* createPlayer(Position start_pos);
void handleInput(int input);
void movePlayer(Position newPos);
void getPlayerName(void);
void levelUP(void);
void applyAltarEffect(void);
void scanNearbyEntities(void);

//room.c functions
Room createRoom(int y, int x, int height, int width);
void addEnemiesToRoom(Room newRoom);
Room createStartRoom(int y, int x, int height, int width);
Room createFinalArena(int y, int x, int height, int width);
void addRoomToMap(Room room);
void addExit(Room room);
void addAltar(Room room);
void connectRoomCenters(Position centerOne, Position centerTwo);

//fov.c functions
void makeFOV(Entity* player);
void clearFOV(Entity* player);
int getDistance(Position origin, Position target);
bool isInMap(int y, int x);
bool lineOfSight(Position origin, Position target);
int getSign(int a);

//extern
extern const int MAP_HEIGHT;
extern const int MAP_WIDTH;
extern Entity* player;
extern Tile** map;
extern EntityList* enemies; //global enemy list
extern TileList* neighbors;
extern int floorCount;
extern Position exit_pos;
extern bool defeatedFinalBoss;

#endif // ROGUE_H
