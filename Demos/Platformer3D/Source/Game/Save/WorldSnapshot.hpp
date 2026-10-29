#pragma once

#include "../Level/Level.hpp"
#include "GameSave.hpp"

namespace Platformer3D {

struct WorldSnapshotData {
  int version = 1;
  GameSaveData game{};
  Level level{};
};

class WorldSnapshot {
 public:
  static bool Save(const PlatformerState& state, const char* path, SerializationMethod method);

  static bool Load(const char* path, PlatformerState& state, SerializationMethod method);
};

}  // namespace Platformer3D
