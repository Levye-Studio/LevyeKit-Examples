
#include "Platformer.hpp"

#include <imgui.h>
#include <raymath.h>

#include <Levye/Debug/Log.hpp>
#include <Levye/Modules/Animation/Animation.hpp>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "Debug/DebugPanel.hpp"
#include "Level/LevelEditor.hpp"

namespace Platformer3D {

namespace {

struct Platform {
  PlatformCollider collider;
  Color color;
};

constexpr Platform kPlatforms[] = {{{{0.0f, -0.5f, 0.0f}, {12.0f, 1.0f, 12.0f}}, GRAY},
                                   {{{0.0f, 0.5f, 8.0f}, {4.0f, 1.0f, 4.0f}}, BLUE},
                                   {{{3.0f, 1.5f, 13.0f}, {4.0f, 1.0f, 4.0f}}, SKYBLUE},
                                   {{{0.0f, 2.5f, 18.0f}, {4.0f, 1.0f, 4.0f}}, GREEN}};

constexpr int kPlatformCount = sizeof(kPlatforms) / sizeof(kPlatforms[0]);

void DrawCenteredText(const char* text, int y, int fontSize, Color color) {
  const int width = MeasureText(text, fontSize);

  DrawText(text, (GetScreenWidth() - width) / 2, y, fontSize, color);
}

}  // namespace

Platformer::Platformer(PlatformerState& state) : m_State(state) {}

void Platformer::Load() {
  const char* modelPath = "Assets/Models/robot.glb";
  Player player(m_State.player);
  player.Reset();

  PlayerAnimator animator(m_State.animator);
  if (!animator.Initialize(modelPath, 2, 6, 3, 11)) {
    Levye::Log::Error("Failed to initialize robot animations");
  }

  PlayerModel playerModel(m_State.playerModel);

  if (!playerModel.Load(modelPath)) {
    Levye::Log::Error("Failed to load robot model.");
  }

  CameraController camera(m_State.camera);
  camera.Initialize(m_State.player.position);
}

void Platformer::BeforeReload() {
  // Release module-owned resources when necessary.
}

void Platformer::AfterReload() {
  // Persistent state is already preserved by LevyeKit.
}

void Platformer::StartGame() { m_State.screen = GameScreen::LevelSelect; }

void Platformer::Update(float dt) {
  if (m_State.screen == GameScreen::MainMenu) {
    if (IsKeyPressed(KEY_ENTER))
      StartGame();
    else if (IsKeyPressed(KEY_ESCAPE)) {
      CloseWindow();
    }
    return;
  }

  if (m_State.screen == GameScreen::LevelComplete) {
    if (IsKeyPressed(KEY_ENTER)) {
      if (m_State.level.currentLevel < 2)
        StartLevel(m_State.level.currentLevel + 1);
      else
        m_State.screen = GameScreen::MainMenu;
    }

    return;
  }

  if (m_State.screen == GameScreen::LevelSelect) {
    if (IsKeyPressed(KEY_ONE)) {
      StartLevel(1);
    } else if (IsKeyPressed(KEY_TWO) && m_State.level.unlockedLevel >= 2) {
      StartLevel(2);
    } else if (IsKeyPressed(KEY_ESCAPE)) {
      m_State.screen = GameScreen::MainMenu;
    }

    return;
  }

  if (IsKeyPressed(KEY_ESCAPE)) {
    m_State.screen =
        m_State.screen == GameScreen::Paused ? GameScreen::Playing : GameScreen::Paused;
  }

  if (m_State.screen != GameScreen::Playing) return;

  CameraController camera(m_State.camera);
  camera.Update(dt, m_State.player.position);

  Player player(m_State.player);
  player.Update(dt, camera.Forward(), camera.Right());

  PlayerAnimator animator(m_State.animator);
  animator.Update(m_State.player, dt);

  if (m_State.editor.enabled && !ImGui::GetIO().WantCaptureMouse &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    const Vector2 mouse = GetMousePosition();

    const Ray ray = GetScreenToWorldRay(mouse, m_State.camera.camera);

    m_State.editor.selectedPlatform = LevelEditor::PickPlatform(m_State.activeLevel, ray);
  }
}

void Platformer::FixedUpdate(float dt) {
  if (m_State.screen != GameScreen::Playing) return;

  LevelManager levels(m_State.level, m_State.activeLevel);
  const Level level = levels.GetCurrentLevel();

  std::vector<PlatformCollider> colliders;
  colliders.reserve(level.platforms.size());

  for (const auto& platform : level.platforms) colliders.push_back(platform.collider);

  Player player(m_State.player);
  player.FixedUpdate(dt, colliders.data(), static_cast<int>(colliders.size()),
                     m_State.level.respawnPosition);

  levels.Update(m_State.player);

  if (m_State.level.completed) m_State.screen = GameScreen::LevelComplete;
}

void Platformer::DrawMainMenu() const {
  DrawCenteredText("PLATFORMER 3D", GetScreenHeight() / 2 - 100, 48, RAYWHITE);

  DrawCenteredText("ENTER - Start", GetScreenHeight() / 2, 24, SKYBLUE);

  DrawCenteredText("ESC - Quit", GetScreenHeight() / 2 + 45, 20, LIGHTGRAY);
}

void Platformer::DrawWorld() const {
  CameraController camera(m_State.camera);

  BeginMode3D(camera.GetCamera());

  LevelManager levels(m_State.level, m_State.activeLevel);
  levels.Draw();

  if (m_State.editor.enabled) {
    LevelEditor::DrawSelection(m_State.activeLevel, m_State.editor.selectedPlatform);
  }

  PlayerModel playerModel(m_State.playerModel);
  playerModel.Draw(m_State.player, m_State.animator);

  EndMode3D();
}

void Platformer::DrawHUD() const {
  DrawText("WASD - Move", 20, 20, 20, WHITE);
  DrawText("SPACE - Jump", 20, 48, 20, WHITE);
  DrawText("RMB + Mouse - Rotate Camera", 20, 76, 20, WHITE);
  DrawText("Wheel - Zoom", 20, 104, 20, WHITE);
  DrawText("ESC - Pause", 20, 132, 20, WHITE);
}

void Platformer::DrawPauseMenu() const {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.65f));

  DrawCenteredText("PAUSED", GetScreenHeight() / 2 - 40, 48, WHITE);

  DrawCenteredText("ESC - Resume", GetScreenHeight() / 2 + 30, 22, LIGHTGRAY);
}

void Platformer::Draw() {
  ClearBackground({25, 30, 45, 255});

  if (m_State.screen == GameScreen::MainMenu) {
    DrawMainMenu();
    return;
  }

  if (m_State.screen == GameScreen::LevelSelect) {
    DrawLevelSelect();
    return;
  }

  DrawWorld();
  DrawHUD();

  if (m_State.screen == GameScreen::Paused) DrawPauseMenu();

  if (m_State.screen == GameScreen::LevelComplete) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    DrawCenteredText("LEVEL COMPLETE!", GetScreenHeight() / 2 - 40, 44, GOLD);

    DrawCenteredText("ENTER - Continue", GetScreenHeight() / 2 + 35, 22, WHITE);
  }

  DebugPanel::Draw(m_State);
}

void Platformer::StartLevel(int id) {
  LevelManager levels(m_State.level, m_State.activeLevel);
  levels.StartLevel(id, m_State.player);

  CameraController camera(m_State.camera);
  camera.Initialize(m_State.player.position);

  m_State.screen = GameScreen::Playing;
}

void Platformer::DrawLevelSelect() const {
  DrawCenteredText("SELECT LEVEL", GetScreenHeight() / 2 - 130, 42, WHITE);

  DrawCenteredText("1 - First Steps", GetScreenHeight() / 2 - 30, 26, SKYBLUE);

  DrawCenteredText(
      m_State.level.unlockedLevel >= 2 ? "2 - The Crossing" : "2 - The Crossing [LOCKED]",
      GetScreenHeight() / 2 + 20, 26, m_State.level.unlockedLevel >= 2 ? GREEN : GRAY);

  DrawCenteredText("ESC - Back", GetScreenHeight() / 2 + 100, 20, LIGHTGRAY);
}

void Platformer::Shutdown() {
  PlayerAnimator animator(m_State.animator);
  animator.Shutdown();

  PlayerModel playerModel(m_State.playerModel);
  playerModel.Shutdown();
}

}  // namespace Platformer3D
