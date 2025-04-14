#include "Component.h"
#include "Application.h"
#include "Entity.h"
#include "Renderer.h"
#include <cstdlib>
#include <curses.h>

void clearPrevPos(Entity &entity) {
  Renderer::getInstance().renderCharAt(entity.m_pos.y, entity.m_pos.x, ' ');
}

Component::~Component() {}

void Component::update(Entity &entity) {}

// ----InputComponent--------------------------------------
void InputComponent::update(Entity &entity) {
  int op = Application::curentInput;

  int maxY, maxX;
  getmaxyx(Renderer::getInstance().getMainWindow(), maxY, maxX);

  switch (op) {
  case 'w':
    clearPrevPos(entity);
    entity.m_pos.y -= 1;
    if (entity.m_pos.y < 1)
      entity.m_pos.y = 1;
    break;
  case 'a':
    clearPrevPos(entity);
    entity.m_pos.x -= 1;
    if (entity.m_pos.x < 1)
      entity.m_pos.x = 1;
    break;
  case 's':
    clearPrevPos(entity);
    entity.m_pos.y += 1;
    if (entity.m_pos.y > maxY - 2)
      entity.m_pos.y = maxY - 2;
    break;
  case 'd':
    clearPrevPos(entity);
    entity.m_pos.x += 1;
    if (entity.m_pos.x > maxX - 2)
      entity.m_pos.x = maxX - 2;
    break;
  }
}

// ----RenderComponent--------------------------------------
RenderComponent::RenderComponent(char ch, int color)
    : m_ch(ch), m_Color(color) {}

void RenderComponent::update(Entity &entity) {
  Renderer::getInstance().renderEntity(entity, m_ch, m_Color);
}

// ----MoveRandomComponent----------------------------------
void MoveRandomComponent::update(Entity &entity) {
  int randDir = rand() % 4;

  int maxY, maxX;
  getmaxyx(Renderer::getInstance().getMainWindow(), maxY, maxX);

  switch (randDir) {
  case 0:
    clearPrevPos(entity);
    entity.m_pos.y -= 1;
    if (entity.m_pos.y < 1)
      entity.m_pos.y = 1;
    break;
  case 1:
    clearPrevPos(entity);
    entity.m_pos.x -= 1;
    if (entity.m_pos.x < 1)
      entity.m_pos.x = 1;
    break;
  case 2:
    clearPrevPos(entity);
    entity.m_pos.y += 1;
    if (entity.m_pos.y > maxY - 2)
      entity.m_pos.y = maxY - 2;
    break;
  case 3:
    clearPrevPos(entity);
    entity.m_pos.x += 1;
    if (entity.m_pos.x > maxX - 2)
      entity.m_pos.x = maxX - 2;
    break;
  }
}
