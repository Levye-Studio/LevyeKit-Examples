#include "CameraController.hpp"

#include <raymath.h>

#include <algorithm>
#include <cmath>

namespace Platformer3D {

namespace {
constexpr float kMouseSensitivity = 0.003f;
constexpr float kZoomSensitivity = 1.0f;
constexpr float kCameraSmoothness = 10.0f;

constexpr float kMinPitch = -0.25f;
constexpr float kMaxPitch = 1.15f;

constexpr float kMinDistance = 3.0f;
constexpr float kMaxDistance = 14.0f;

constexpr float kTargetHeight = 1.0f;
}  // namespace

CameraController::CameraController(CameraState& state) : m_State(state) {}

void CameraController::Initialize(const Vector3& target) {
  m_State.camera.up = {0.0f, 1.0f, 0.0f};
  m_State.camera.fovy = 55.0f;
  m_State.camera.projection = CAMERA_PERSPECTIVE;

  m_State.yaw = 0.0f;
  m_State.pitch = 0.35f;
  m_State.distance = 8.0f;

  m_State.initialized = false;

  UpdateTransform(1.0f, target);
}

void CameraController::Update(float deltaTime, const Vector3& target) {
  const Vector2 mouseDelta = GetMouseDelta();

  // Rotate while holding the right mouse button.
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    m_State.yaw -= mouseDelta.x * kMouseSensitivity;
    m_State.pitch += mouseDelta.y * kMouseSensitivity;

    m_State.pitch = std::clamp(m_State.pitch, kMinPitch, kMaxPitch);
  }

  m_State.distance -= GetMouseWheelMove() * kZoomSensitivity;

  m_State.distance = std::clamp(m_State.distance, kMinDistance, kMaxDistance);

  UpdateTransform(deltaTime, target);
}

void CameraController::UpdateTransform(float deltaTime, const Vector3& target) {
  const Vector3 focus = Vector3Add(target, {0.0f, kTargetHeight, 0.0f});

  const float horizontalDistance = std::cos(m_State.pitch) * m_State.distance;

  const Vector3 offset{std::sin(m_State.yaw) * horizontalDistance,
                       std::sin(m_State.pitch) * m_State.distance,
                       -std::cos(m_State.yaw) * horizontalDistance};

  const Vector3 desiredPosition = Vector3Add(focus, offset);

  if (!m_State.initialized) {
    m_State.camera.position = desiredPosition;
    m_State.initialized = true;
  } else {
    const float blend = std::clamp(deltaTime * kCameraSmoothness, 0.0f, 1.0f);

    m_State.camera.position = Vector3Lerp(m_State.camera.position, desiredPosition, blend);
  }

  m_State.camera.target = focus;
}

Vector3 CameraController::Forward() const {
  // Horizontal forward vector from camera toward the player.
  const Vector3 forward = Vector3Subtract(m_State.camera.target, m_State.camera.position);

  Vector3 flat{forward.x, 0.0f, forward.z};

  if (Vector3LengthSqr(flat) < 0.0001f) return {0.0f, 0.0f, 1.0f};

  return Vector3Normalize(flat);
}

Vector3 CameraController::Right() const {
  const Vector3 forward = Forward();

  return Vector3Normalize(Vector3CrossProduct({0.0f, 1.0f, 0.0f}, forward));
}

const Camera3D& CameraController::GetCamera() const { return m_State.camera; }
}  // namespace Platformer3D
