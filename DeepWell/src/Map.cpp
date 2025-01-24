#include "Map.h"

void setupMapTileEntities(flecs::world world)
{
    for (int y = 0; y < LINES; y++) {
        for (int x = 0; x < COLS; x++) {
            world.entity()
                .insert([&](PositionComponent& pos, CharComponent& cha, ColorComponent& c, WalkableComponent& w) {
                pos = { y, x };
                if ((y > 1 && y < LINES - 2) && (x > 1 && x < COLS - 2)) {
                    cha = { ' ' };
                    w = { true };
                }
                else {
                    cha = { '#' };
                    w = { false };
                }
                c = { COLOR_PAIR(BLUE_BLACK) };
            }).add(world.component<TileEntityTag>());
        }
    }
}
