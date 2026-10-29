#pragma once

#include <Levye/Modules/Serialization/Serialization.hpp>

#include "EditableLevel.hpp"

namespace Platformer3D {

bool WriteLevelManual(Levye::DocumentHandle document, const char* root, const EditableLevel& level);

bool ReadLevelManual(Levye::DocumentHandle document, const char* root, EditableLevel& level);

}  // namespace Platformer3D
