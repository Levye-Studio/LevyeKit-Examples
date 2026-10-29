
#include "DebugPanel.hpp"

#include <Levye/Debug/Log.hpp>

#include "../Level/LevelLoader.hpp"
#include "../Level/LevelSerializer.hpp"
#include "../Save/GameSave.hpp"
#include "../Save/WorldSnapshot.hpp"

#if defined(LEVYE_WITH_IMGUI) && !defined(NDEBUG)
#include <imgui.h>
#endif

namespace Platformer3D {

void DebugPanel::Draw(PlatformerState& state) {
#if defined(LEVYE_WITH_IMGUI) && !defined(NDEBUG)

  if (ImGui::GetCurrentContext() == nullptr) return;

  if (!ImGui::Begin("Platformer Debug")) {
    ImGui::End();
    return;
  }

  ImGui::Text("Level: %d", state.level.currentLevel);
  ImGui::Text("Unlocked: %d", state.level.unlockedLevel);

  ImGui::SeparatorText("Player");

  ImGui::DragFloat3("Position", &state.player.position.x, 0.05f);

  ImGui::Text("Velocity: %.2f, %.2f, %.2f", state.player.velocity.x, state.player.velocity.y,
              state.player.velocity.z);

  ImGui::Text("Grounded: %s", state.player.grounded ? "Yes" : "No");

  if (ImGui::Button("Respawn")) {
    state.player.position = state.level.respawnPosition;
    state.player.velocity = {};
    state.player.grounded = false;
    state.player.jumpRequested = false;
  }

  ImGui::SeparatorText("Camera");

  ImGui::SliderFloat("Distance", &state.camera.distance, 3.0f, 14.0f);

  ImGui::SliderFloat("Pitch", &state.camera.pitch, -0.25f, 1.15f);

  ImGui::SeparatorText("Level");

  ImGui::Text("Checkpoint: %s", state.level.checkpointActivated ? "Activated" : "Inactive");

  ImGui::Text("Completed: %s", state.level.completed ? "Yes" : "No");

  ImGui::Text("FPS: %d", GetFPS());

  ImGui::SeparatorText("Player Model");

  ImGui::Text("Model scale: %.3f", state.playerModel.scale);

  ImGui::SliderFloat("Scale multiplier", &state.playerModel.scale, 0.05f, 2.0f);

  ImGui::SeparatorText("Level Editor");

  auto& level = state.activeLevel;

  ImGui::Text("Level: %s", level.name);
  ImGui::Text("Platforms: %d", level.platformCount);

  for (int i = 0; i < level.platformCount; ++i) {
    ImGui::PushID(i);

    if (ImGui::TreeNode("Platform")) {
      auto& platform = level.platforms[i];

      ImGui::DragFloat3("Position", &platform.collider.center.x, 0.1f);

      ImGui::DragFloat3("Size", &platform.collider.size.x, 0.1f, 0.1f, 100.0f);

      ImGui::TreePop();
    }

    ImGui::PopID();
  }

  ImGui::SeparatorText("LevyeKit Serialization");

  static int methodIndex = 0;

  ImGui::RadioButton("Manual API", &methodIndex, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Archive API", &methodIndex, 1);

  const auto method = methodIndex == 0 ? SerializationMethod::Manual : SerializationMethod::Archive;

  const std::string suffix = methodIndex == 0 ? "_manual.yaml" : "_archive.yaml";

  std::string number = std::to_string(state.level.currentLevel);

  if (state.level.currentLevel < 10) number = "0" + number;

  const std::string path = "Assets/Levels/level_" + number + suffix;

  if (ImGui::Button("Save Level")) {
    if (!LevelSerializer::Save(state.activeLevel, path.c_str(), method))
      Levye::Log::Error("Level serialization failed.");
  }

  ImGui::SameLine();

  if (ImGui::Button("Load Level")) {
    if (!LevelSerializer::Load(path.c_str(), state.activeLevel, method))
      Levye::Log::Error("Level deserialization failed.");
  }

  ImGui::Separator();

  if (ImGui::Button("Test Both Methods")) {
    const bool passed = LevelSerializer::CompareMethods(state.activeLevel);

    if (passed)
      Levye::Log::Info("Serialization: all four round-trips passed.");
    else
      Levye::Log::Error("Serialization: round-trip test failed.");
  }

  if (ImGui::Button("Reset Level to Default")) {
    if (!LevelLoader::ResetToDefault(state.level.currentLevel, state.activeLevel)) {
      Levye::Log::Error("Failed to reset level.");
    }
  }

  ImGui::SeparatorText("Game Save");

  const auto saveMethod =
      methodIndex == 0 ? SerializationMethod::Manual : SerializationMethod::Archive;

  const char* savePath = methodIndex == 0 ? "Saves/game_manual.yaml" : "Saves/game_archive.yaml";

  if (ImGui::Button("Save Game")) {
    if (GameSave::Save(state, savePath, saveMethod))
      Levye::Log::Info("Game saved.");
    else
      Levye::Log::Error("Game save failed.");
  }

  ImGui::SameLine();

  if (ImGui::Button("Load Game")) {
    if (GameSave::Load(savePath, state, saveMethod))
      Levye::Log::Info("Game loaded.");
    else
      Levye::Log::Error("Game load failed.");
  }

  ImGui::SeparatorText("Full World Snapshot");

  const auto snapshotMethod =
      methodIndex == 0 ? SerializationMethod::Manual : SerializationMethod::Archive;

  const char* snapshotPath =
      methodIndex == 0 ? "Saves/world_manual.yaml" : "Saves/world_archive.yaml";

  if (ImGui::Button("Save World")) {
    if (WorldSnapshot::Save(state, snapshotPath, snapshotMethod))
      Levye::Log::Info("World snapshot saved.");
    else
      Levye::Log::Error("World snapshot save failed.");
  }

  ImGui::SameLine();

  if (ImGui::Button("Load World")) {
    if (WorldSnapshot::Load(snapshotPath, state, snapshotMethod))
      Levye::Log::Info("World snapshot loaded.");
    else
      Levye::Log::Error("World snapshot load failed.");
  }

  ImGui::SeparatorText("Cross-Method Compatibility");

  if (ImGui::Button("Manual File -> Archive Reader")) {
    if (WorldSnapshot::Load("Saves/world_manual.yaml", state, SerializationMethod::Archive))
      Levye::Log::Info("Manual to Archive: PASS");
    else
      Levye::Log::Error("Manual to Archive: FAIL");
  }

  if (ImGui::Button("Archive File -> Manual Reader")) {
    if (WorldSnapshot::Load("Saves/world_archive.yaml", state, SerializationMethod::Manual))
      Levye::Log::Info("Archive to Manual: PASS");
    else
      Levye::Log::Error("Archive to Manual: FAIL");
  }

  ImGui::SeparatorText("Level Editor");

  ImGui::Checkbox("Enable 3D Editing", &state.editor.enabled);

  if (state.editor.enabled) {
    ImGui::Text("Selected Platform: %d", state.editor.selectedPlatform);

    const int index = state.editor.selectedPlatform;

    if (index >= 0 && index < state.activeLevel.platformCount) {
      auto& platform = state.activeLevel.platforms[index];

      ImGui::DragFloat3("Position", &platform.collider.center.x, 0.1f);

      ImGui::DragFloat3("Size", &platform.collider.size.x, 0.1f, 0.1f, 100.0f);

      float color[4] = {platform.color.r / 255.0f, platform.color.g / 255.0f,
                        platform.color.b / 255.0f, platform.color.a / 255.0f};

      if (ImGui::ColorEdit4("Color", color)) {
        platform.color = {static_cast<unsigned char>(color[0] * 255.0f),
                          static_cast<unsigned char>(color[1] * 255.0f),
                          static_cast<unsigned char>(color[2] * 255.0f),
                          static_cast<unsigned char>(color[3] * 255.0f)};
      }
    }
  }

  ImGui::End();

#else
  (void)state;
#endif
}

}  // namespace Platformer3D
