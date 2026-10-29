#include "PlayerAnimator.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace Platformer3D {

PlayerAnimator::PlayerAnimator(PlayerAnimatorState& state) : m_State(state) {}

bool PlayerAnimator::Initialize(const char* modelPath, int idleIndex, int runIndex, int jumpIndex,
                                int fallIndex) {
  if (!Levye::Animation::IsAvailable()) return false;

  const int count = Levye::Animation::GetAnimationCount(modelPath);

  if (count <= 0) return false;

  const int indices[] = {idleIndex, runIndex, jumpIndex, fallIndex};

  for (int index : indices) {
    if (index < 0 || index >= count) return false;
  }

  std::vector<Levye::AnimationResourceHandle> clips(count);

  if (Levye::Animation::LoadAnimations(modelPath, clips.data(), count) != count) return false;

  m_State.idle = clips[idleIndex];
  m_State.run = clips[runIndex];
  m_State.jump = clips[jumpIndex];
  m_State.fall = clips[fallIndex];

  m_State.controller = Levye::Animation::CreateController();

  if (m_State.controller == Levye::INVALID_ANIMATION_CONTROLLER) return false;

  const auto controller = m_State.controller;

  const bool registered =
      Levye::Animation::AddState(controller, "Idle", m_State.idle, 0.2f) &&
      Levye::Animation::AddState(controller, "Run", m_State.run, 0.2f) &&
      Levye::Animation::AddState(controller, "Jump", m_State.jump, 0.1f, 1.0f, true) &&
      Levye::Animation::AddState(controller, "Fall", m_State.fall, 0.15f);

  if (!registered) {
    Shutdown();
    return false;
  }

  m_State.current = PlayerAnimation::Idle;

  if (!Levye::Animation::SetState(controller, "Idle")) {
    Shutdown();
    return false;
  }

  return true;
}

void PlayerAnimator::Update(const PlayerState& player, float dt) {
  if (!Levye::Animation::IsControllerValid(m_State.controller)) return;

  constexpr float kRunEnterSpeed = 0.25f;
  constexpr float kRunExitSpeed = 0.12f;
  constexpr float kFallThreshold = -0.3f;
  constexpr float kMinAirborneTime = 0.08f;
  constexpr float kLandingDuration = 0.10f;

  const float horizontalSpeed =
      std::sqrt(player.velocity.x * player.velocity.x + player.velocity.z * player.velocity.z);

  const bool justLanded = !m_State.wasGrounded && player.grounded;

  if (!player.grounded) {
    m_State.airborneTime += dt;
    m_State.landingTime = 0.0f;
  } else {
    m_State.airborneTime = 0.0f;

    if (justLanded)
      m_State.landingTime = kLandingDuration;
    else
      m_State.landingTime = std::max(0.0f, m_State.landingTime - dt);
  }

  PlayerAnimation desired = m_State.current;

  if (!player.grounded) {
    if (player.velocity.y < kFallThreshold && m_State.airborneTime >= kMinAirborneTime) {
      desired = PlayerAnimation::Fall;
    } else if (m_State.current != PlayerAnimation::Fall) {
      desired = PlayerAnimation::Jump;
    }
  } else if (m_State.landingTime > 0.0f) {
    // Hold the current animation briefly on landing.
    desired = m_State.current;
  } else {
    const bool running = m_State.current == PlayerAnimation::Run ? horizontalSpeed > kRunExitSpeed
                                                                 : horizontalSpeed > kRunEnterSpeed;

    desired = running ? PlayerAnimation::Run : PlayerAnimation::Idle;
  }

  if (desired != m_State.current) {
    const char* name = "Idle";

    switch (desired) {
      case PlayerAnimation::Idle:
        name = "Idle";
        break;
      case PlayerAnimation::Run:
        name = "Run";
        break;
      case PlayerAnimation::Jump:
        name = "Jump";
        break;
      case PlayerAnimation::Fall:
        name = "Fall";
        break;
    }

    if (Levye::Animation::SetState(m_State.controller, name)) m_State.current = desired;
  }

  m_State.wasGrounded = player.grounded;
}

bool PlayerAnimator::Apply(Model& model) const {
  return Levye::Animation::ApplyBoundAnimation(m_State.controller, &model);
}

void PlayerAnimator::Shutdown() {
  if (Levye::Animation::IsControllerValid(m_State.controller))
    Levye::Animation::DestroyController(m_State.controller);

  m_State.controller = Levye::INVALID_ANIMATION_CONTROLLER;

  const Levye::AnimationResourceHandle clips[] = {m_State.idle, m_State.run, m_State.jump,
                                                  m_State.fall};

  for (int i = 0; i < 4; ++i) {
    if (clips[i] == Levye::INVALID_ANIMATION_RESOURCE) continue;

    bool alreadyUnloaded = false;
    for (int j = 0; j < i; ++j) alreadyUnloaded |= clips[j] == clips[i];

    if (!alreadyUnloaded) (void)Levye::Animation::UnloadAnimation(clips[i]);
  }

  m_State.idle = Levye::INVALID_ANIMATION_RESOURCE;
  m_State.run = Levye::INVALID_ANIMATION_RESOURCE;
  m_State.jump = Levye::INVALID_ANIMATION_RESOURCE;
  m_State.fall = Levye::INVALID_ANIMATION_RESOURCE;
}

}  // namespace Platformer3D
