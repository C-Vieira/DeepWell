#pragma once

namespace Atlas {

class Entity;

// Generic Component class
class Component {
public:
  virtual ~Component();
  virtual void update(Entity &entity);
};

class InputComponent : public Component {
public:
  int m_Input;

  virtual void update(Entity &entity) override;
};

class RenderComponent : public Component {
public:
  char m_ch;
  int m_Color;
  RenderComponent(char ch, int color);

  virtual void update(Entity &entity) override;
};

// Testing
class MoveRandomComponent : public Component {
public:
  virtual void update(Entity &entity) override;
};

} // namespace Atlas
