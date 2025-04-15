#include "Renderer.h"
#include "Entity.h"

namespace Atlas {
#include <curses.h>

Renderer::Renderer() {}

Renderer &Renderer::getInstance() {
  static Renderer *instance = new Renderer();
  return *instance;
}

WINDOW *Renderer::createWindow(int height, int width, int starty, int startx) {
  WINDOW *win;

  win = newwin(height, width, starty, startx);
  box(win, 0, 0);

  refresh();
  wrefresh(win);

  return win;
}

WINDOW *Renderer::getMainWindow() { return mainWindow; }

void Renderer::setMainWindow(WINDOW *win) {
  mainWindow = win;

  mvwprintw(mainWindow, 0, 1, "MainWindow");
  wrefresh(mainWindow);
}

void Renderer::clearWindow(WINDOW *win) {
  wclear(mainWindow);
  box(mainWindow, 0, 0);

  mvwprintw(mainWindow, 0, 1, "MainWindow");
}

void Renderer::destroyWindow(WINDOW *win) {
  wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

  refresh();
  wrefresh(win);
  delwin(win);
}

void Renderer::renderCharAt(int y, int x, char ch) {
  mvwaddch(mainWindow, y, x, ch);
}

void Renderer::renderEntity(Entity &entity, char ch, int color) {
  mvwaddch(mainWindow, entity.m_pos.y, entity.m_pos.x, ch | color);
}

} // namespace Atlas
