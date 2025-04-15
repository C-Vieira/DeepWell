#pragma once

namespace Atlas {

  typedef struct _win WINDOW;
  class Entity;

class Renderer {
public:
  static Renderer &getInstance();

  WINDOW *createWindow(int height, int width, int starty, int startx);
  WINDOW *getMainWindow();
  void setMainWindow(WINDOW *win);
  void clearWindow(WINDOW *win);
  void destroyWindow(WINDOW *win);

  void renderCharAt(int y, int x, char ch);
  void renderEntity(Entity &entity, char ch, int color);

private:
  Renderer();

  WINDOW *mainWindow;
};

} // namespace Atlas
