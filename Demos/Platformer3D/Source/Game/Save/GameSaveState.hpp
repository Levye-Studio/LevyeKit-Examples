#pragma once

#include <cmath>

#include "GameSave.hpp"

namespace Platformer3D {

inline GameSaveData CaptureGameSave(const PlatformerState& state) {
  GameSaveData save{};

  save.player.position = state.player.position;
  save.player.velocity = state.player.velocity;
  save.player.facingAngle = state.player.facingAngle;
  save.player.grounded = state.player.grounded;

  save.camera.yaw = state.camera.yaw;
  save.camera.pitch = state.camera.pitch;
  save.camera.distance = state.camera.distance;

  save.progress.currentLevel = state.level.currentLevel;
  save.progress.unlockedLevel = state.level.unlockedLevel;
  save.progress.checkpointActivated = state.level.checkpointActivated;
  save.progress.completed = state.level.completed;
  save.progress.respawnPosition = state.level.respawnPosition;

  return save;
}

inline bool ValidateGameSave(const GameSaveData& save) {
  const auto finite = [](float value) { return std::isfinite(value); };

  const auto validVector = [&](Vector3 v) { return finite(v.x) && finite(v.y) && finite(v.z); };

  return save.version == 1 && save.progress.currentLevel >= 1 && save.progress.currentLevel <= 2 &&
         save.progress.unlockedLevel >= save.progress.currentLevel &&
         save.progress.unlockedLevel <= 2 && validVector(save.player.position) &&
         validVector(save.player.velocity) && finite(save.player.facingAngle) &&
         validVector(save.progress.respawnPosition) && finite(save.camera.yaw) &&
         finite(save.camera.pitch) && finite(save.camera.distance) &&
         save.camera.distance >= 2.0f && save.camera.distance <= 30.0f;
}

inline void ApplyGameSave(const GameSaveData& save, PlatformerState& state) {
  state.player.position = save.player.position;
  state.player.velocity = save.player.velocity;
  state.player.facingAngle = save.player.facingAngle;
  state.player.grounded = save.player.grounded;
  state.player.jumpRequested = false;

  state.camera.yaw = save.camera.yaw;
  state.camera.pitch = save.camera.pitch;
  state.camera.distance = save.camera.distance;

  state.level.currentLevel = save.progress.currentLevel;
  state.level.unlockedLevel = save.progress.unlockedLevel;
  state.level.checkpointActivated = save.progress.checkpointActivated;
  state.level.completed = save.progress.completed;
  state.level.respawnPosition = save.progress.respawnPosition;
}

}  // namespace Platformer3D
