#pragma once

#include <Levye/Modules/Animation/Animation.hpp>

#include "Player.hpp"

namespace Platformer3D {

enum class PlayerAnimation { Idle, Run, Jump, Fall };

struct PlayerAnimatorState {
  Levye::AnimationControllerHandle controller = Levye::INVALID_ANIMATION_CONTROLLER;

  Levye::AnimationResourceHandle idle = Levye::INVALID_ANIMATION_RESOURCE;

  Levye::AnimationResourceHandle run = Levye::INVALID_ANIMATION_RESOURCE;

  Levye::AnimationResourceHandle jump = Levye::INVALID_ANIMATION_RESOURCE;

  Levye::AnimationResourceHandle fall = Levye::INVALID_ANIMATION_RESOURCE;

  PlayerAnimation current = PlayerAnimation::Idle;

  bool wasGrounded = true;
  float airborneTime = 0.0f;
  float landingTime = 0.0f;
};

class PlayerAnimator {
 public:
  explicit PlayerAnimator(PlayerAnimatorState& state);

  bool Initialize(const char* modelPath, int idleIndex, int runIndex, int jumpIndex, int fallIndex);

  void Update(const PlayerState& player, float dt);
  bool Apply(Model& model) const;

  void Shutdown();

 private:
  PlayerAnimatorState& m_State;
};

}  // namespace Platformer3D
