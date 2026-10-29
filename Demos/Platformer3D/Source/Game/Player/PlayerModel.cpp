#include "PlayerModel.hpp"

#include <rlgl.h>

#include <Levye/Modules/Animation/Animation.hpp>

namespace Platformer3D {

PlayerModel::PlayerModel(PlayerModelState& state) : m_State(state) {}

bool PlayerModel::Load(const char* path) {
  if (m_State.loaded) return true;

  m_State.model = LoadModel(path);

  if (!IsModelValid(m_State.model)) return false;

  const BoundingBox bounds = GetModelBoundingBox(m_State.model);

  const float modelHeight = bounds.max.y - bounds.min.y;
  constexpr float targetHeight = 1.8f;

  if (modelHeight <= 0.0001f) {
    UnloadModel(m_State.model);
    m_State.model = {};
    return false;
  }

  m_State.scale = targetHeight / modelHeight;
  m_State.groundOffset = -bounds.min.y * m_State.scale;
  m_State.loaded = true;
  m_State.modelMinY = bounds.min.y;
  m_State.groundOffset = -m_State.modelMinY * m_State.scale;

  return true;
}

void PlayerModel::Draw(const PlayerState& player, const PlayerAnimatorState& animator) {
  if (!m_State.loaded) return;

  // Apply the active skeletal animation before drawing.
  if (Levye::Animation::IsControllerValid(animator.controller)) {
    (void)Levye::Animation::ApplyBoundAnimation(animator.controller, &m_State.model);
  }

  rlPushMatrix();

  const float groundOffset = -m_State.modelMinY * m_State.scale;

  rlTranslatef(player.position.x, player.position.y - 0.9f + groundOffset, player.position.z);

  rlRotatef(player.facingAngle * RAD2DEG, 0.0f, 1.0f, 0.0f);

  DrawModel(m_State.model, {0.0f, 0.0f, 0.0f}, m_State.scale, WHITE);

  rlPopMatrix();
}

void PlayerModel::Shutdown() {
  if (m_State.loaded) {
    UnloadModel(m_State.model);
    m_State.model = {};
    m_State.loaded = false;
  }
}
}  // namespace Platformer3D
