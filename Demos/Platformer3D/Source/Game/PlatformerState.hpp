
#pragma once

#include "Camera/CameraController.hpp"
#include "Level/EditableLevel.hpp"
#include "Level/LevelManager.hpp"
#include "Player/Player.hpp"
#include "Player/PlayerAnimator.hpp"
#include "Player/PlayerModel.hpp"

namespace Platformer3D {

enum class GameScreen : int { MainMenu, LevelSelect, Playing, Paused, LevelComplete };

struct LevelEditorState {
  bool enabled = false;
  int selectedPlatform = -1;
};

struct PlatformerState {
  GameScreen screen = GameScreen::MainMenu;

  PlayerState player{};
  PlayerAnimatorState animator{};
  PlayerModelState playerModel{};

  CameraState camera{};
  LevelState level{};
  EditableLevel activeLevel{};
  LevelEditorState editor{};
};

}  // namespace Platformer3D
