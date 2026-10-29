#pragma once

#include <raylib.h>

#include "EditableLevel.hpp"

namespace Platformer3D {

class LevelEditor {
 public:
  // Returns the index of the platform hit by the ray.
  // Returns -1 if no playform was hit.
  static int PickPlatform(const EditableLevel& level, Ray ray);

  static void DrawSelection(const EditableLevel& level, int SelectedIndex);
};
}  // namespace Platformer3D
