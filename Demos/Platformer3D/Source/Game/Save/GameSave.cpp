
#include "GameSave.hpp"

#include <Levye/Modules/Serialization/Serialization.hpp>
#include <string>

#include "../Level/LevelLoader.hpp"
#include "GameSaveDocument.hpp"
#include "GameSaveSerialization.hpp"
#include "GameSaveState.hpp"

namespace Platformer3D {
namespace {

using Levye::DocumentHandle;
using Levye::Serialization;

struct DocumentGuard {
  DocumentHandle handle;

  ~DocumentGuard() {
    if (Serialization::Contains(handle)) (void)Serialization::Destroy(handle);
  }
};

}  // namespace

bool GameSave::Save(const PlatformerState& state, const char* path, SerializationMethod method) {
  if (!Serialization::Available() || !path || !*path) return false;

  GameSaveData data = CaptureGameSave(state);

  if (!ValidateGameSave(data)) return false;

  const auto doc = Serialization::Create();

  if (!Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  bool written = false;

  switch (method) {
    case SerializationMethod::Manual:
      written = WriteGameDataManual(doc, "data", data);
      break;

    case SerializationMethod::Archive:
      written = Serialization::Write(doc, "data", data);
      break;

    default:
      return false;
  }

  return written && Serialization::Save(doc, path);
}

bool GameSave::Load(const char* path, PlatformerState& state, SerializationMethod method) {
  if (!Serialization::Available() || !path || !*path) return false;

  const auto doc = Serialization::Load(path);

  if (!Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};
  GameSaveData data{};

  bool read = false;

  switch (method) {
    case SerializationMethod::Manual:
      read = ReadGameDataManual(doc, "data", data);
      break;

    case SerializationMethod::Archive:
      read = Serialization::Read(doc, "data", data);
      break;

    default:
      return false;
  }

  if (!read || !ValidateGameSave(data)) return false;

  // Load level geometry before applying gameplay state.
  EditableLevel loadedLevel{};
  LevelSource source = LevelSource::Default;

  if (!LevelLoader::Load(data.progress.currentLevel, loadedLevel, source)) return false;

  state.activeLevel = loadedLevel;
  ApplyGameSave(data, state);

  // CameraController can rebuild its camera on the next update.
  // Preserve its initialization flag unless your controller
  // explicitly requires reinitialization after restoring angles.

  state.screen = GameScreen::Playing;
  return true;
}

}  // namespace Platformer3D
