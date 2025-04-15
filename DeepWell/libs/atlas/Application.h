#pragma once

#include "Entity.h"
#include <vector>

// Color pairs
#define WHITE_BLACK 1
#define BLUE_BLACK 2
#define RED_BLACK 3
#define YELLOW_BLACK 4
#define GREEN_BLACK 5
#define BLACK_BLACK 6
#define CYAN_BLACK 7
#define MAGENTA_BLACK 8

namespace Atlas {

class Application {
public:
  static char curentInput;

  std::vector<Entity> m_Entities;

  Application();
  ~Application();

  virtual void Init() = 0;
  virtual void Run() = 0;

protected:
  bool m_Running = true;
};

} // namespace Atlas
