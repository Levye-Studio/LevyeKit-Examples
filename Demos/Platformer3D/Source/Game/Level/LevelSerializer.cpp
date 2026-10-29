
#include "LevelSerializer.hpp"

#include <cmath>
#include <cstring>

namespace Platformer3D {
namespace {

bool Equal(float a, float b) { return std::fabs(a - b) <= 0.0001f; }

bool Equal(Vector3 a, Vector3 b) { return Equal(a.x, b.x) && Equal(a.y, b.y) && Equal(a.z, b.z); }

bool EqualLevels(const EditableLevel& a, const EditableLevel& b) {
  if (a.loaded != b.loaded || a.id != b.id || std::strcmp(a.name, b.name) != 0 ||
      a.platformCount != b.platformCount || !Equal(a.spawn, b.spawn) ||
      !Equal(a.checkpoint, b.checkpoint) || !Equal(a.finish, b.finish))
    return false;

  for (int i = 0; i < a.platformCount; ++i) {
    const auto& x = a.platforms[i];
    const auto& y = b.platforms[i];

    if (!Equal(x.collider.center, y.collider.center) || !Equal(x.collider.size, y.collider.size))
      return false;

    if (x.color.r != y.color.r || x.color.g != y.color.g || x.color.b != y.color.b ||
        x.color.a != y.color.a)
      return false;
  }

  return true;
}

}  // namespace

bool LevelSerializer::Save(const EditableLevel& level, const char* path,
                           SerializationMethod method) {
  switch (method) {
    case SerializationMethod::Manual:
      return SaveManual(level, path);

    case SerializationMethod::Archive:
      return SaveArchive(level, path);
  }

  return false;
}

bool LevelSerializer::Load(const char* path, EditableLevel& destination,
                           SerializationMethod method) {
  switch (method) {
    case SerializationMethod::Manual:
      return LoadManual(path, destination);

    case SerializationMethod::Archive:
      return LoadArchive(path, destination);
  }

  return false;
}

bool LevelSerializer::CompareMethods(const EditableLevel& level) {
  if (!level.loaded) return false;

  constexpr const char* manualPath = "Assets/Levels/compare_manual.yaml";

  constexpr const char* archivePath = "Assets/Levels/compare_archive.yaml";

  if (!Save(level, manualPath, SerializationMethod::Manual) ||
      !Save(level, archivePath, SerializationMethod::Archive))
    return false;

  EditableLevel mm{};
  EditableLevel aa{};
  EditableLevel ma{};
  EditableLevel am{};

  return Load(manualPath, mm, SerializationMethod::Manual) &&
         Load(archivePath, aa, SerializationMethod::Archive) &&
         Load(manualPath, ma, SerializationMethod::Archive) &&
         Load(archivePath, am, SerializationMethod::Manual) && EqualLevels(level, mm) &&
         EqualLevels(level, aa) && EqualLevels(level, ma) && EqualLevels(level, am);
}

}  // namespace Platformer3D
