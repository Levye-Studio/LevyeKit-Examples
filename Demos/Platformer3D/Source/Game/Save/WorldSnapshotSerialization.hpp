#pragma once

#include "../Level/LevelSerializationTypes.hpp"
#include "GameSaveSerialization.hpp"
#include "WorldSnapshot.hpp"

namespace Platformer3D {

inline bool Serialize(Levye::WriteArchive& archive, WorldSnapshotData& snapshot) {
  return archive.Field("version", snapshot.version) && archive.Field("game", snapshot.game) &&
         archive.Field("level", snapshot.level);
}

inline bool Serialize(Levye::ReadArchive& archive, WorldSnapshotData& snapshot) {
  return archive.RequiredField("version", snapshot.version) &&
         archive.RequiredField("game", snapshot.game) &&
         archive.RequiredField("level", snapshot.level);
}

}  // namespace Platformer3D
