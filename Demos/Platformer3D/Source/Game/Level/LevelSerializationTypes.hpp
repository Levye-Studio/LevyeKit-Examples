
#pragma once

#include <Levye/Modules/Serialization/Archive.hpp>
#include <vector>

#include "Level.hpp"

namespace Platformer3D {

struct LevelFile {
  int version = 1;
  Level level{};
};

inline bool Serialize(Levye::WriteArchive& archive, LevelPlatform& platform) {
  return archive.Field("position", platform.collider.center) &&
         archive.Field("size", platform.collider.size) && archive.Field("color", platform.color);
}

inline bool Serialize(Levye::ReadArchive& archive, LevelPlatform& platform) {
  return archive.RequiredField("position", platform.collider.center) &&
         archive.RequiredField("size", platform.collider.size) &&
         archive.RequiredField("color", platform.color);
}

inline bool Serialize(Levye::WriteArchive& archive, Level& level) {
  return archive.Field("id", level.id) && archive.Field("name", level.name) &&
         archive.Field("spawn", level.spawn) && archive.Field("checkpoint", level.checkpoint) &&
         archive.Field("finish", level.finish) && archive.Field("platforms", level.platforms);
}

inline bool Serialize(Levye::ReadArchive& archive, Level& level) {
  return archive.RequiredField("id", level.id) && archive.RequiredField("name", level.name) &&
         archive.RequiredField("spawn", level.spawn) &&
         archive.RequiredField("checkpoint", level.checkpoint) &&
         archive.RequiredField("finish", level.finish) &&
         archive.RequiredField("platforms", level.platforms);
}

inline bool Serialize(Levye::WriteArchive& archive, LevelFile& file) {
  return archive.Field("version", file.version) && archive.Field("level", file.level);
}

inline bool Serialize(Levye::ReadArchive& archive, LevelFile& file) {
  return archive.RequiredField("version", file.version) &&
         archive.RequiredField("level", file.level);
}

}  // namespace Platformer3D
