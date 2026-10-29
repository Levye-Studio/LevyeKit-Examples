#include "LevelManager.hpp"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>

#include "LevelLoader.hpp"

namespace Platformer3D {

LevelManager::LevelManager(LevelState& state, EditableLevel& activeLevel)
    : m_State(state), m_ActiveLevel(activeLevel) {}

void LevelManager::StartLevel(int id, PlayerState& player) {
  EditableLevel loaded{};
  LevelSource source = LevelSource::Default;

  if (!LevelLoader::Load(id, loaded, source)) return;

  m_ActiveLevel = loaded;

  const Level level = GetCurrentLevel();

  m_State.currentLevel = id;
  m_State.checkpointActivated = false;
  m_State.completed = false;
  m_State.respawnPosition = level.spawn;

  player.position = level.spawn;
  player.velocity = Vector3Zero();
  player.grounded = false;
  player.jumpRequested = false;
}

void LevelManager::Update(PlayerState& player) {
  const Level level = GetCurrentLevel();

  if (!m_State.checkpointActivated && Vector3Distance(player.position, level.checkpoint) < 1.5f) {
    m_State.checkpointActivated = true;
    m_State.respawnPosition = level.checkpoint;
  }

  if (!m_State.completed && Vector3Distance(player.position, level.finish) < 1.5f) {
    m_State.completed = true;
    m_State.unlockedLevel = std::max(m_State.unlockedLevel, std::min(m_State.currentLevel + 1, 2));
  }
}

void LevelManager::Draw() const { DrawLevel(GetCurrentLevel()); }

Level LevelManager::GetCurrentLevel() const { return RestoreLevel(m_ActiveLevel); }

EditableLevel& LevelManager::GetEditableLevel() { return m_ActiveLevel; }

bool LevelManager::ReplaceLevel(const Level& level) { return StoreLevel(level, m_ActiveLevel); }

}  // namespace Platformer3D
