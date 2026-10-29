#include "LevelEditor.hpp"

#include <limits>

namespace Platformer3D {

int LevelEditor::PickPlatform(const EditableLevel& level, Ray ray) {
  if (!level.loaded) return -1;

  int selected = -1;
  float closestDistance = std::numeric_limits<float>::max();

  for (int i = 0; i < level.platformCount; ++i) {
    const auto& platform = level.platforms[i];

    const Vector3 half{platform.collider.size.x * 0.5f, platform.collider.size.y * 0.5f,
                       platform.collider.size.z * 0.5f};

    const Vector3 center = platform.collider.center;

    const BoundingBox bounds{{center.x - half.x, center.y - half.y, center.z - half.z},
                             {center.x + half.x, center.y + half.y, center.z + half.z}};

    const RayCollision hit = GetRayCollisionBox(ray, bounds);

    if (hit.hit && hit.distance < closestDistance) {
      closestDistance = hit.distance;
      selected = i;
    }
  }

  return selected;
}

void LevelEditor::DrawSelection(const EditableLevel& level, int selectedIndex) {
  if (!level.loaded || selectedIndex < 0 || selectedIndex >= level.platformCount) return;

  const auto& platform = level.platforms[selectedIndex];

  const Vector3 center = platform.collider.center;
  const Vector3 size = platform.collider.size;

  DrawCubeWiresV(center, {size.x + 0.06f, size.y + 0.06f, size.z + 0.06f}, YELLOW);
}

}  // namespace Platformer3D
