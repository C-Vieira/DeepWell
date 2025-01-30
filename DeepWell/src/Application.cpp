#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
    endwin();
}


void Application::Run()
{
    mvprintw(5, 6, "Hello from curses!");
    int op;
    int y = 1;

    // Game Loop
	while (m_Running) {
        
        op = getch();
        mvprintw(5 + y++, 6, "Running...");

        if (op == 'q') break;
	}
}

void Application::Init() {
    // Curses Setup
    initscr();
    noecho();
    curs_set(0);
}