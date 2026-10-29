#pragma once

#include <raylib.h>

namespace Platformer3D {

struct CameraState {
  Camera3D camera{};

  float yaw = 0.0f;
  float pitch = 0.35f;
  float distance = 0.0f;

  bool initialized = false;
};

class CameraController {
 public:
  explicit CameraController(CameraState& state);

  void Initialize(const Vector3& target);
  void Update(float deltaTime, const Vector3& target);

  [[nodiscard]] Vector3 Forward() const;
  [[nodiscard]] Vector3 Right() const;

  [[nodiscard]] const Camera3D& GetCamera() const;

 private:
  void UpdateTransform(float deltaTime, const Vector3& target);

  CameraState& m_State;
};
}  // namespace Platformer3D
