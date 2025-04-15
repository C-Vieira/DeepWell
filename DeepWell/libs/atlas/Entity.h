#pragma once

#include "Component.h"
#include <vector>

namespace Atlas {

struct Position {
  int x, y;
};

class Entity {
public:
  Position m_pos;
  std::vector<Component *> m_Components;

  Entity(Position pos);

  void update();

  void addComponent(Component *component);
};

} // namespace Atlas
