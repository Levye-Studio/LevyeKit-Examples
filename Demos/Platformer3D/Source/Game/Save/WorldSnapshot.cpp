
#include "WorldSnapshot.hpp"

#include <Levye/Modules/Serialization/Serialization.hpp>

#include "../Level/EditableLevel.hpp"
#include "../Level/LevelSerializationDocument.hpp"
#include "GameSaveDocument.hpp"
#include "GameSaveState.hpp"
#include "WorldSnapshotSerialization.hpp"

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

bool WriteManual(DocumentHandle doc, const WorldSnapshotData& snapshot) {
  EditableLevel editable{};

  if (!StoreLevel(snapshot.level, editable)) return false;

  return Serialization::SetInt(doc, "snapshot.version", snapshot.version) &&
         WriteGameDataManual(doc, "snapshot.game", snapshot.game) &&
         WriteLevelManual(doc, "snapshot.level", editable);
}

bool ReadManual(DocumentHandle doc, WorldSnapshotData& destination) {
  if (!Serialization::ContainsPath(doc, "snapshot.version")) return false;

  if (Serialization::GetInt(doc, "snapshot.version", -1) != 1) return false;

  WorldSnapshotData loaded{};
  EditableLevel editable{};

  if (!ReadGameDataManual(doc, "snapshot.game", loaded.game) ||
      !ReadLevelManual(doc, "snapshot.level", editable))
    return false;

  loaded.level = RestoreLevel(editable);
  destination = loaded;
  return true;
}

bool ValidateSnapshot(const WorldSnapshotData& snapshot) {
  if (snapshot.version != 1 || !ValidateGameSave(snapshot.game)) return false;

  if (snapshot.level.id != snapshot.game.progress.currentLevel) return false;

  if (snapshot.level.platforms.size() > kMaxLevelPlatforms) return false;

  EditableLevel checked{};
  return StoreLevel(snapshot.level, checked);
}

}  // namespace

bool WorldSnapshot::Save(const PlatformerState& state, const char* path,
                         SerializationMethod method) {
  if (!Serialization::Available() || !path || !*path || !state.activeLevel.loaded) return false;

  WorldSnapshotData snapshot{};
  snapshot.game = CaptureGameSave(state);
  snapshot.level = RestoreLevel(state.activeLevel);

  if (!ValidateSnapshot(snapshot)) return false;

  const auto doc = Serialization::Create();

  if (!Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  bool written = false;

  switch (method) {
    case SerializationMethod::Manual:
      written = WriteManual(doc, snapshot);
      break;

    case SerializationMethod::Archive:
      written = Serialization::Write(doc, "snapshot", snapshot);
      break;

    default:
      return false;
  }

  return written && Serialization::Save(doc, path);
}

bool WorldSnapshot::Load(const char* path, PlatformerState& state, SerializationMethod method) {
  if (!Serialization::Available() || !path || !*path) return false;

  const auto doc = Serialization::Load(path);

  if (!Serialization::Contains(doc)) return false;

  DocumentGuard guard{doc};

  WorldSnapshotData snapshot{};
  bool read = false;

  switch (method) {
    case SerializationMethod::Manual:
      read = ReadManual(doc, snapshot);
      break;

    case SerializationMethod::Archive:
      read = Serialization::Read(doc, "snapshot", snapshot);
      break;

    default:
      return false;
  }

  if (!read || !ValidateSnapshot(snapshot)) return false;

  EditableLevel loadedLevel{};

  if (!StoreLevel(snapshot.level, loadedLevel)) return false;

  // Commit only after the entire snapshot is valid.
  state.activeLevel = loadedLevel;
  ApplyGameSave(snapshot.game, state);
  state.screen = GameScreen::Playing;

  return true;
}

}  // namespace Platformer3D
