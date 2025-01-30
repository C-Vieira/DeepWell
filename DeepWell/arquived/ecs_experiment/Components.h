#pragma once

// Components
struct PositionComponent {
	int y, x;
};

struct DirectionComponent {
	int y, x;
};

struct CharComponent {
	char ch;
};

struct ColorComponent {
	int color;
};

struct KeyInputComponent {
	int key;
};

struct WalkableComponent {
	bool walkable;
};

// -Scenes
using Pipeline = flecs::entity;
struct MenuScene { Pipeline pip; };
struct GameScene { Pipeline pip; };

// Tags
struct TakesInputTag {};
struct FollowsAITag {};
struct TileEntityTag {};

// -Scene Tags/Relationships
struct ActiveScene {}; // Represents the current scene
struct SceneRoot {};   // Parent for all entities unique to the scene