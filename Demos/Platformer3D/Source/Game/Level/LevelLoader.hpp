#pragma once

#include "EditableLevel.hpp"
#include "LevelSerializer.hpp"

namespace Platformer3D {

enum class LevelSource { Default, ManualFile, ArchiveFile };

class LevelLoader {
 public:
  static bool Load(int levelId, EditableLevel& destination, LevelSource& source);

  static bool ResetToDefault(int levelId, EditableLevel& destination);
};
}  // namespace Platformer3D
