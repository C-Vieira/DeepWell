#include "Map.h"

void setupMapTileEntities(flecs::world world, flecs::entity scene)
{
    int wallWidth = rand() % 8 + 1;

    for (int y = 0; y < LINES; y++) {
        for (int x = 0; x < COLS; x++) {
            world.entity()
                .insert([&](PositionComponent& pos, CharComponent& cha, ColorComponent& c, WalkableComponent& w) {
                pos = { y, x };
                if ((y > (wallWidth - 1) && y < LINES - wallWidth) && (x > (wallWidth - 1) && x < COLS - wallWidth)) {
                    cha = { ' ' };
                    w = { true };
                }
                else {
                    cha = { '#' };
                    w = { false };
                }
                c = { COLOR_PAIR(BLUE_BLACK) };
                })
                .child_of(scene)
                .add(world.component<TileEntityTag>());
        }
    }
}
