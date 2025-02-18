#include "Component.h"
#include "Application.h"
#include "Entity.h"
#include "curses.h"
#include <cstdlib>

Component::~Component() {}

void Component::update(Entity &entity) {}

// ----InputComponent--------------------------------------
void InputComponent::update(Entity &entity) {
  int op = Application::curentInput;

  switch (op) {
  case 'w':
    entity.m_pos.y -= 1;
    break;
  case 'a':
    entity.m_pos.x -= 1;
    break;
  case 's':
    entity.m_pos.y += 1;
    break;
  case 'd':
    entity.m_pos.x += 1;
    break;
  }
}

// ----RenderComponent--------------------------------------
RenderComponent::RenderComponent(char ch, int color) : m_ch(ch), m_Color(color) {}

void RenderComponent::update(Entity &entity) {
  mvaddch(entity.m_pos.y, entity.m_pos.x, m_ch | m_Color);
}

// ----MoveRandomComponent--------------------------------------
void MoveRandomComponent::update(Entity &entity) {
  int randDir = rand() % 4;

  switch (randDir) {
  case 0:
    entity.m_pos.y -= 1;
    break;
  case 1:
    entity.m_pos.x -= 1;
    break;
  case 2:
    entity.m_pos.y += 1;
    break;
  case 3:
    entity.m_pos.x += 1;
    break;
  }
}
