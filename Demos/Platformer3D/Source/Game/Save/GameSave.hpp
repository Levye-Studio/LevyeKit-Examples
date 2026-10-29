#pragma once

#include "../Level/LevelSerializer.hpp"
#include "../PlatformerState.hpp"

namespace Platformer3D {

struct SavedPlayer {
  Vector3 position{};
  Vector3 velocity{};
  float facingAngle = 0.0f;
  bool grounded = false;
};

struct SavedCamera {
  float yaw = 0.0f;
  float pitch = 0.35f;
  float distance = 8.0f;
};

struct SavedProgress {
  int currentLevel = 1;
  int unlockedLevel = 1;

  bool checkpointActivated = false;
  bool completed = false;

  Vector3 respawnPosition{};
};

struct GameSaveData {
  int version = 1;
  SavedPlayer player{};
  SavedCamera camera{};
  SavedProgress progress{};
};

class GameSave {
 public:
  static bool Save(const PlatformerState& state, const char* path, SerializationMethod method);

  static bool Load(const char* path, PlatformerState& state, SerializationMethod method);
};
}  // namespace Platformer3D
