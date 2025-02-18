#include "Application.h"
#include "Component.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

char Application::curentInput = ' ';

Application::Application() {}

Application::~Application() { endwin(); }

void Application::Run() {
  mvprintw(5, 6, "Hello from curses!");
  int op;

  // Game Loop
  while (m_Running) {

    Application::curentInput = getch();
    op = Application::curentInput;

    clear();
    for (int i = 0; i < m_Entities.size(); i++) {
      m_Entities[i].update();
    }

    if (op == 'q')
      break;
  }
}

void Application::Init() {
  srand((unsigned int)time(NULL));

  // Curses Setup
  initscr();
  noecho();
  curs_set(0);

  if (has_colors()) {
    start_color();

    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);     // blue
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);       // white
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);         // red
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);   // yellow
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);     // green
    init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK);     // black
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);       // cyan
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK); // magenta

  } else {
    mvprintw(20, 50, "Your system doesn't support color. Can't start game!");
    getch();
  }

  // Player Setup
  Position pos = {5, 3};
  InputComponent *input = new InputComponent();
  RenderComponent *render = new RenderComponent('@', COLOR_PAIR(WHITE_BLACK));
  Entity player(pos);
  player.addComponent(input);
  player.addComponent(render);
  m_Entities.push_back(player);

  // Setup Test Entities
  for (int i = 0; i < 10; i++) {
    Position pos = {rand() % 40, rand() % 20};
    MoveRandomComponent *move = new MoveRandomComponent();
    RenderComponent *render = new RenderComponent('@', COLOR_PAIR(GREEN_BLACK));
    Entity entity(pos);
    entity.addComponent(move);
    entity.addComponent(render);
    m_Entities.push_back(entity);
  }
}
