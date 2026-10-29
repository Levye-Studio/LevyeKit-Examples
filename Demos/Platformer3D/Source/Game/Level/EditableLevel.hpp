#pragma once

#include <cstddef>

#include "Level.hpp"

namespace Platformer3D {

constexpr int kMaxLevelPlatforms = 128;

struct EditableLevel {
  int id = 1;
  char name[64]{};

  Vector3 spawn{};
  Vector3 checkpoint{};
  Vector3 finish{};

  LevelPlatform platforms[kMaxLevelPlatforms]{};
  int platformCount = 0;

  bool loaded = false;
};

bool StoreLevel(const Level& source, EditableLevel& destination);
Level RestoreLevel(const EditableLevel& source);

}  // namespace Platformer3D
