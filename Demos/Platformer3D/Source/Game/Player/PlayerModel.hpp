#pragma once

#include <raylib.h>

#include "Player.hpp"
#include "PlayerAnimator.hpp"

namespace Platformer3D {

struct PlayerModelState {
  Model model{};
  bool loaded = false;

  float scale = 1.0f;
  float groundOffset = 0.0f;
  float modelMinY = 0.0f;
};

class PlayerModel {
 public:
  explicit PlayerModel(PlayerModelState& state);

  bool Load(const char* path);
  void Draw(const PlayerState& player, const PlayerAnimatorState& animator);
  void Shutdown();

 private:
  PlayerModelState& m_State;
};
}  // namespace Platformer3D
