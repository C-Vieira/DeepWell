#include "Entity.h"

Entity::Entity(Position pos) : m_pos(pos) {}

void Entity::update() {
  for (Component *component : m_Components) {
    component->update(*this);
  }
}

void Entity::addComponent(Component *component) {
  m_Components.push_back(component);
}
