#pragma once

#include <raylib.h>

#include <string>
#include <vector>

#include "../Player/Player.hpp"

namespace Platformer3D {

struct LevelPlatform {
  PlatformCollider collider;
  Color color = GRAY;
};

struct Level {
  int id = 1;
  std::string name;

  Vector3 spawn{0.0f, 0.9f, 0.0f};
  Vector3 checkpoint{};
  Vector3 finish{};

  std::vector<LevelPlatform> platforms;
};

Level CreateLevel(int id);

void DrawLevel(const Level& level);
}  // namespace Platformer3D
