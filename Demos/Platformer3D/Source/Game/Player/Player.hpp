#pragma once

#include <raylib.h>

namespace Platformer3D {

struct PlayerState {
  Vector3 position{0.0f, 0.9f, 0.0f};
  Vector3 velocity{};

  float facingAngle = 0.0f;

  bool grounded = true;
  bool jumpRequested = false;
};

struct PlatformCollider {
  Vector3 center;
  Vector3 size;
};

class Player {
 public:
  explicit Player(PlayerState& state);

  void Reset();

  void Update(float deltaTime, const Vector3& cameraForward, const Vector3& cameraRight);
  void FixedUpdate(float fixedDeltaTime, const PlatformCollider* platforms, int platformCount,
                   const Vector3& respawnPosition);

  void Draw() const;

 private:
  PlayerState& m_State;
};
void DrawPlayer(const PlayerState& state);
}  // namespace Platformer3D
