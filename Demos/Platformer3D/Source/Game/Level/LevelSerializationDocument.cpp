#include "LevelSerializationDocument.hpp"

#include <cstring>
#include <string>

namespace Platformer3D {
namespace {

using Levye::DocumentHandle;
using Levye::Serialization;

std::string PlatformPath(const char* root, int index) {
  return std::string(root) + ".platforms[" + std::to_string(index) + "]";
}

bool ReadVector(DocumentHandle doc, const std::string& path, Vector3& output) {
  for (const char* component : {"x", "y", "z"}) {
    if (!Serialization::ContainsPath(doc, (path + "." + component).c_str())) return false;
  }

  output = Serialization::GetVector3(doc, path);
  return true;
}

bool WritePlatform(DocumentHandle doc, const std::string& path, const LevelPlatform& platform) {
  const auto& c = platform.color;

  return Serialization::SetVector3(doc, path + ".position", platform.collider.center) &&
         Serialization::SetVector3(doc, path + ".size", platform.collider.size) &&
         Serialization::SetInt(doc, (path + ".color.r").c_str(), c.r) &&
         Serialization::SetInt(doc, (path + ".color.g").c_str(), c.g) &&
         Serialization::SetInt(doc, (path + ".color.b").c_str(), c.b) &&
         Serialization::SetInt(doc, (path + ".color.a").c_str(), c.a);
}

bool ReadPlatform(DocumentHandle doc, const std::string& path, LevelPlatform& platform) {
  if (!ReadVector(doc, path + ".position", platform.collider.center) ||
      !ReadVector(doc, path + ".size", platform.collider.size))
    return false;

  unsigned char* channels[] = {&platform.color.r, &platform.color.g, &platform.color.b,
                               &platform.color.a};

  const char* names[] = {"r", "g", "b", "a"};

  for (int i = 0; i < 4; ++i) {
    const std::string key = path + ".color." + names[i];

    if (!Serialization::ContainsPath(doc, key.c_str())) return false;

    const auto value = Serialization::GetInt(doc, key.c_str(), -1);

    if (value < 0 || value > 255) return false;

    *channels[i] = static_cast<unsigned char>(value);
  }

  return true;
}

}  // namespace

bool WriteLevelManual(Levye::DocumentHandle doc, const char* root, const EditableLevel& source) {
  if (!root || !*root || !source.loaded || source.platformCount < 0 ||
      source.platformCount > kMaxLevelPlatforms)
    return false;

  const std::string base(root);
  const std::string platformsPath = base + ".platforms";

  if (!Serialization::SetInt(doc, (base + ".id").c_str(), source.id) ||
      !Serialization::SetString(doc, (base + ".name").c_str(), source.name) ||
      !Serialization::SetVector3(doc, (base + ".spawn").c_str(), source.spawn) ||
      !Serialization::SetVector3(doc, (base + ".checkpoint").c_str(), source.checkpoint) ||
      !Serialization::SetVector3(doc, (base + ".finish").c_str(), source.finish) ||
      !Serialization::CreateArray(doc, platformsPath.c_str()))
    return false;

  for (int i = 0; i < source.platformCount; ++i) {
    if (!Serialization::AppendObject(doc, platformsPath.c_str())) return false;

    if (!WritePlatform(doc, PlatformPath(root, i), source.platforms[i])) return false;
  }

  return true;
}

bool ReadLevelManual(Levye::DocumentHandle doc, const char* root, EditableLevel& destination) {
  if (!root || !*root) return false;

  const std::string base(root);

  if (!Serialization::ContainsPath(doc, (base + ".id").c_str()) ||
      !Serialization::ContainsPath(doc, (base + ".name").c_str()) ||
      !Serialization::IsArray(doc, (base + ".platforms").c_str()))
    return false;

  const auto count = Serialization::GetArraySize(doc, (base + ".platforms").c_str());

  if (count > kMaxLevelPlatforms) return false;

  EditableLevel loaded{};

  const auto id = Serialization::GetInt(doc, (base + ".id").c_str(), -1);

  const std::string name = Serialization::GetString(doc, (base + ".name").c_str());

  if (id < 1 || id > 1000000 || name.empty() || name.size() >= sizeof(loaded.name)) return false;

  loaded.id = static_cast<int>(id);

  std::memcpy(loaded.name, name.c_str(), name.size() + 1);

  if (!ReadVector(doc, base + ".spawn", loaded.spawn) ||
      !ReadVector(doc, base + ".checkpoint", loaded.checkpoint) ||
      !ReadVector(doc, base + ".finish", loaded.finish))
    return false;

  loaded.platformCount = static_cast<int>(count);

  for (int i = 0; i < loaded.platformCount; ++i) {
    if (!ReadPlatform(doc, PlatformPath(root, i), loaded.platforms[i])) return false;
  }

  loaded.loaded = true;
  destination = loaded;
  return true;
}

}  // namespace Platformer3D
