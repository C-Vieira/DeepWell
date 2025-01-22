#pragma once

#include <curses.h>

//color pairs
#define WHITE_BLACK 1
#define BLUE_BLACK 2
#define RED_BLACK 3
#define YELLOW_BLACK 4
#define GREEN_BLACK 5
#define BLACK_BLACK 6
#define CYAN_BLACK 7
#define MAGENTA_BLACK 8

bool cursesSetup() {
    initscr();
    noecho();
    curs_set(0);

    if (has_colors()) {
        start_color();

        init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK); //white
        init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK); //blue
        init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK); //red
        init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK); //yellow
        init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK); //green
        init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK); //black
        init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK); //cyan
        init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK); //magenta

        return true;
    }
    else {
        mvprintw(20, 50, "Your system doesn't support color. Can't start game!");
        getch();

        return false;
    }
}