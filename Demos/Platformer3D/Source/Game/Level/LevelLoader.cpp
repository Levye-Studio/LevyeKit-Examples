#include "LevelLoader.hpp"

#include <string>

#include "Level.hpp"

namespace Platformer3D {
namespace {

std::string LevelPath(int levelId, const char* suffix) {
  std::string number = std::to_string(levelId);

  if (levelId < 10) number = "0" + number;

  return "Assets/Levels/level_" + number + suffix;
}

}  // namespace

bool LevelLoader::Load(int levelId, EditableLevel& destination, LevelSource& source) {
  if (levelId < 1) return false;

  EditableLevel loaded{};

  const std::string archivePath = LevelPath(levelId, "_archive.yaml");

  if (LevelSerializer::Load(archivePath.c_str(), loaded, SerializationMethod::Archive) &&
      loaded.id == levelId) {
    destination = loaded;
    source = LevelSource::ArchiveFile;
    return true;
  }

  const std::string manualPath = LevelPath(levelId, "_manual.yaml");

  if (LevelSerializer::Load(manualPath.c_str(), loaded, SerializationMethod::Manual) &&
      loaded.id == levelId) {
    destination = loaded;
    source = LevelSource::ManualFile;
    return true;
  }

  if (!ResetToDefault(levelId, destination)) return false;

  source = LevelSource::Default;
  return true;
}

bool LevelLoader::ResetToDefault(int levelId, EditableLevel& destination) {
  if (levelId < 1 || levelId > 2) return false;

  const Level level = CreateLevel(levelId);

  EditableLevel loaded{};

  if (!StoreLevel(level, loaded)) return false;

  destination = loaded;
  return true;
}

}  // namespace Platformer3D
