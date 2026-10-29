#pragma once

#include <Levye/Modules/Serialization/Serialization.hpp>

#include "GameSave.hpp"

namespace Platformer3D {

bool WriteGameDataManual(Levye::DocumentHandle document, const char* root,
                         const GameSaveData& data);

bool ReadGameDataManual(Levye::DocumentHandle document, const char* root, GameSaveData& data);

}  // namespace Platformer3D
