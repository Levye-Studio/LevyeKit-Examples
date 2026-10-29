#pragma once

#include "EditableLevel.hpp"
#include "Level.hpp"

namespace Platformer3D {

struct LevelState {
  int currentLevel = 1;
  int unlockedLevel = 1;

  bool checkpointActivated = false;
  bool completed = false;

  Vector3 respawnPosition{0.0f, 0.9f, 0.0f};
};

class LevelManager {
 public:
  LevelManager(LevelState& state, EditableLevel& activeLevel);

  void StartLevel(int id, PlayerState& player);
  void Update(PlayerState& player);
  void Draw() const;

  Level GetCurrentLevel() const;
  EditableLevel& GetEditableLevel();

  bool ReplaceLevel(const Level& level);

 private:
  LevelState& m_State;
  EditableLevel& m_ActiveLevel;
};

}  // namespace Platformer3D
