#pragma once

#include "../libs/atlas/Application.h"

class Game : public Atlas::Application {
public:
  Game();
  ~Game();

  virtual void Init() override;
  virtual void Run() override;
};
