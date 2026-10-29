#pragma once

#include <Levye/Modules/Serialization/Archive.hpp>

#include "GameSave.hpp"

namespace Platformer3D {

template <typename Archive>
bool Serialize(Archive& ar, SavedPlayer& p) {
  return ar.Field("position", p.position) && ar.Field("velocity", p.velocity) &&
         ar.Field("facingAngle", p.facingAngle) && ar.Field("grounded", p.grounded);
}

template <typename Archive>
bool Serialize(Archive& ar, SavedCamera& c) {
  return ar.Field("yaw", c.yaw) && ar.Field("pitch", c.pitch) && ar.Field("distance", c.distance);
}

template <typename Archive>
bool Serialize(Archive& ar, SavedProgress& p) {
  return ar.Field("currentLevel", p.currentLevel) && ar.Field("unlockedLevel", p.unlockedLevel) &&
         ar.Field("checkpointActivated", p.checkpointActivated) &&
         ar.Field("completed", p.completed) && ar.Field("respawnPosition", p.respawnPosition);
}

template <typename Archive>
bool Serialize(Archive& ar, GameSaveData& save) {
  return ar.Field("version", save.version) && ar.Field("player", save.player) &&
         ar.Field("camera", save.camera) && ar.Field("progress", save.progress);
}

}  // namespace Platformer3D
