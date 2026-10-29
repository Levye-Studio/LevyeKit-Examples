#pragma once

#include "PlatformerState.hpp"

namespace Platformer3D {

class Platformer {
 public:
  explicit Platformer(PlatformerState& state);

  void Load();
  void BeforeReload();
  void AfterReload();

  void Update(float deltaTime);
  void FixedUpdate(float fixedDeltaTime);
  void Draw();

  void StartLevel(int id);
  void DrawLevelSelect() const;

  void Shutdown();

 private:
  void StartGame();

  void DrawMainMenu() const;
  void DrawWorld() const;
  void DrawHUD() const;
  void DrawPauseMenu() const;

  PlatformerState& m_State;
};
}  // namespace Platformer3D
