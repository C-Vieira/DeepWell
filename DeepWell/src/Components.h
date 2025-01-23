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

// Tags
struct TakesInputTag {};
struct FollowsAITag {};