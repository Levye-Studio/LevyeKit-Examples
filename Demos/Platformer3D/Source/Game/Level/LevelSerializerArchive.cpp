
#include <Levye/Modules/Serialization/Serialization.hpp>

#include "LevelSerializationTypes.hpp"
#include "LevelSerializer.hpp"

namespace Platformer3D {
namespace {

struct DocumentGuard {
  Levye::DocumentHandle handle;

  ~DocumentGuard() {
    if (Levye::Serialization::Contains(handle)) (void)Levye::Serialization::Destroy(handle);
  }
};

}  // namespace

bool LevelSerializer::SaveArchive(const EditableLevel& source, const char* path) {
  if (!Levye::Serialization::Available() || !source.loaded || !path || !*path ||
      source.platformCount < 0 || source.platformCount > kMaxLevelPlatforms)
    return false;

  LevelFile file{};
  file.level = RestoreLevel(source);

  const auto doc = Levye::Serialization::Create();

  if (!Levye::Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  if (!Levye::Serialization::Write(doc, "data", file)) return false;

  return Levye::Serialization::Save(doc, path);
}

bool LevelSerializer::LoadArchive(const char* path, EditableLevel& destination) {
  if (!Levye::Serialization::Available() || !path || !*path) return false;

  const auto doc = Levye::Serialization::Load(path);

  if (!Levye::Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  LevelFile file{};

  if (!Levye::Serialization::Read(doc, "data", file)) return false;

  if (file.version != 1) return false;

  EditableLevel loaded{};

  if (!StoreLevel(file.level, loaded)) return false;

  destination = loaded;
  return true;
}

}  // namespace Platformer3D
