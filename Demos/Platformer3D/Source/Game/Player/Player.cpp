#include "Player.hpp"

#include <raymath.h>
#include <rlgl.h>

#include <cmath>

namespace Platformer3D {

namespace {

constexpr float kHeight = 1.8f;
constexpr float kRadius = 0.35f;

constexpr float kMoveSpeed = 6.0f;
constexpr float kJumpSpeed = 8.0f;
constexpr float kGravity = 20.0f;

}  // namespace

Player::Player(PlayerState& state) : m_State(state) {}

void Player::Reset() {
  m_State.position = {0.0f, 0.9f, 0.0f};
  m_State.velocity = {};
  m_State.facingAngle = 0.0f;
  m_State.grounded = true;
  m_State.jumpRequested = false;
}

void Player::Update(float deltaTime, const Vector3& cameraForward, const Vector3& cameraRight) {
  (void)deltaTime;

  Vector3 movement{};

  if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, cameraForward);

  if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, cameraForward);

  if (IsKeyDown(KEY_D)) movement = Vector3Subtract(movement, cameraRight);

  if (IsKeyDown(KEY_A)) movement = Vector3Add(movement, cameraRight);

  movement.y = 0.0f;

  if (Vector3LengthSqr(movement) > 0.0001f) {
    movement = Vector3Normalize(movement);

    m_State.velocity.x = movement.x * kMoveSpeed;
    m_State.velocity.z = movement.z * kMoveSpeed;

    m_State.facingAngle = std::atan2(movement.x, movement.z);
  } else {
    m_State.velocity.x = 0.0f;
    m_State.velocity.z = 0.0f;
  }

  if (IsKeyPressed(KEY_SPACE)) m_State.jumpRequested = true;
}

void Player::FixedUpdate(float dt, const PlatformCollider* platforms, int platformCount,
                         const Vector3& respawnPosition) {
  if (m_State.jumpRequested && m_State.grounded) {
    m_State.velocity.y = kJumpSpeed;
    m_State.grounded = false;
  }

  m_State.jumpRequested = false;

  const float previousFeet = m_State.position.y - kHeight * 0.5f;

  m_State.velocity.y -= kGravity * dt;

  m_State.position = Vector3Add(m_State.position, Vector3Scale(m_State.velocity, dt));

  m_State.grounded = false;

  const float currentFeet = m_State.position.y - kHeight * 0.5f;

  for (int i = 0; i < platformCount; ++i) {
    const PlatformCollider& platform = platforms[i];

    const float top = platform.center.y + platform.size.y * 0.5f;

    const bool overlapsX =
        std::abs(m_State.position.x - platform.center.x) <= platform.size.x * 0.5f + kRadius;

    const bool overlapsZ =
        std::abs(m_State.position.z - platform.center.z) <= platform.size.z * 0.5f + kRadius;

    if (!overlapsX || !overlapsZ) continue;

    if (m_State.velocity.y <= 0.0f && previousFeet >= top - 0.02f && currentFeet <= top) {
      m_State.position.y = top + kHeight * 0.5f;
      m_State.velocity.y = 0.0f;
      m_State.grounded = true;
    }
  }

  if (m_State.position.y < -15.0f) {
    m_State.position = respawnPosition;
    m_State.velocity = {};
    m_State.grounded = false;
    m_State.jumpRequested = false;
  }
}

void Player::Draw() const { DrawPlayer(m_State); }

void DrawPlayer(const PlayerState& state) {
  rlPushMatrix();

  rlTranslatef(state.position.x, state.position.y, state.position.z);

  rlRotatef(state.facingAngle * RAD2DEG, 0.0f, 1.0f, 0.0f);

  DrawCubeV({0.0f, 0.0f, 0.0f}, {0.7f, kHeight, 0.7f}, ORANGE);

  // Indicates which direction the player faces.
  DrawCubeV({0.0f, 0.2f, 0.4f}, {0.25f, 0.25f, 0.2f}, RED);

  rlPopMatrix();
}
}  // namespace Platformer3D
