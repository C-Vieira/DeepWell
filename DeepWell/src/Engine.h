#pragma once

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>

#include <curses.h>

#include <flecs.h>
#include "Components.h"

//color pairs
#define WHITE_BLACK 1
#define BLUE_BLACK 2
#define RED_BLACK 3
#define YELLOW_BLACK 4
#define GREEN_BLACK 5
#define BLACK_BLACK 6
#define CYAN_BLACK 7
#define MAGENTA_BLACK 8



void ECSSetup(flecs::world world);

bool cursesSetup();

void doGameLoop(flecs::world world);

void closeGame();