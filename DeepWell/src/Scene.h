#pragma once

#include "Engine.h"

void resetScene(flecs::world& world);

void initScenes(flecs::world& world);

void setupMenuScene(flecs::iter& it, size_t, ActiveScene);

void setupGameScene(flecs::iter& it, size_t, ActiveScene);