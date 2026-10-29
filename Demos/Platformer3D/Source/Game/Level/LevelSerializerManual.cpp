#include <Levye/Modules/Serialization/Serialization.hpp>

#include "LevelSerializationDocument.hpp"
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

bool LevelSerializer::SaveManual(const EditableLevel& source, const char* path) {
  if (!Levye::Serialization::Available() || !path || !*path) return false;

  const auto doc = Levye::Serialization::Create();

  if (!Levye::Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  return Levye::Serialization::SetInt(doc, "data.version", 1) &&
         WriteLevelManual(doc, "data.level", source) && Levye::Serialization::Save(doc, path);
}

bool LevelSerializer::LoadManual(const char* path, EditableLevel& destination) {
  if (!Levye::Serialization::Available() || !path || !*path) return false;

  const auto doc = Levye::Serialization::Load(path);

  if (!Levye::Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  if (!Levye::Serialization::ContainsPath(doc, "data.version") ||
      Levye::Serialization::GetInt(doc, "data.version", -1) != 1)
    return false;

  return ReadLevelManual(doc, "data.level", destination);
}

}  // namespace Platformer3D
