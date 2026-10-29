#include "Level.hpp"

namespace Platformer3D {

Level CreateLevel(int id) {
  Level level;
  level.id = id;

  if (id == 1) {
    level.name = "First Steps";
    level.spawn = {0.0f, 0.9f, 0.0f};
    level.checkpoint = {0.0f, 2.4f, 8.0f};
    level.finish = {0.0f, 4.4f, 18.0f};

    level.platforms = {{{{0.0f, -0.5f, 0.0f}, {12.0f, 1.0f, 12.0f}}, GRAY},
                       {{{0.0f, 0.5f, 8.0f}, {4.0f, 1.0f, 4.0f}}, BLUE},
                       {{{3.0f, 1.5f, 13.0f}, {4.0f, 1.0f, 4.0f}}, SKYBLUE},
                       {{{0.0f, 2.5f, 18.0f}, {4.0f, 1.0f, 4.0f}}, GREEN}};
  } else {
    level.name = "The Crossing";
    level.spawn = {0.0f, 0.9f, 0.0f};
    level.checkpoint = {-2.0f, 3.4f, 12.0f};
    level.finish = {0.0f, 5.4f, 24.0f};

    level.platforms = {{{{0.0f, -0.5f, 0.0f}, {8.0f, 1.0f, 8.0f}}, GRAY},
                       {{{2.0f, 0.5f, 6.0f}, {3.0f, 1.0f, 3.0f}}, BLUE},
                       {{{-2.0f, 1.5f, 12.0f}, {3.0f, 1.0f, 3.0f}}, SKYBLUE},
                       {{{2.0f, 2.5f, 18.0f}, {3.0f, 1.0f, 3.0f}}, PURPLE},
                       {{{0.0f, 3.5f, 24.0f}, {4.0f, 1.0f, 4.0f}}, GREEN}};
  }

  return level;
}

void DrawLevel(const Level& level) {
  for (const LevelPlatform& platform : level.platforms) {
    DrawCubeV(platform.collider.center, platform.collider.size, platform.color);

    DrawCubeWiresV(platform.collider.center, platform.collider.size, DARKGRAY);
  }

  DrawCylinder(level.checkpoint, 0.6f, 0.6f, 2.0f, 16, YELLOW);

  DrawCylinder(level.finish, 0.8f, 0.8f, 3.0f, 16, GOLD);
}
}  // namespace Platformer3D
