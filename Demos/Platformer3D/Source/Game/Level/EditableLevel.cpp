#include "EditableLevel.hpp"

#include <algorithm>
#include <cstring>
#include <utility>

namespace Platformer3D {

bool StoreLevel(const Level& source, EditableLevel& destination) {
  if (source.platforms.size() > kMaxLevelPlatforms) return false;

  EditableLevel result{};

  result.id = source.id;
  result.spawn = source.spawn;
  result.checkpoint = source.checkpoint;
  result.finish = source.finish;

  std::strncpy(result.name, source.name.c_str(), sizeof(result.name) - 1);

  result.platformCount = static_cast<int>(source.platforms.size());

  std::copy(source.platforms.begin(), source.platforms.end(), result.platforms);

  result.loaded = true;
  destination = result;

  return true;
}

Level RestoreLevel(const EditableLevel& source) {
  Level result{};

  result.id = source.id;
  result.name = source.name;
  result.spawn = source.spawn;
  result.checkpoint = source.checkpoint;
  result.finish = source.finish;

  const int count = std::clamp(source.platformCount, 0, kMaxLevelPlatforms);

  result.platforms.assign(source.platforms, source.platforms + count);

  return result;
}

}  // namespace Platformer3D
