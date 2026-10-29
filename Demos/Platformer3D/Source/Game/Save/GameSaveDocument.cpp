

#include "GameSaveDocument.hpp"

#include <Levye/Modules/Serialization/Serialization.hpp>
#include <string>

namespace Platformer3D {
namespace {

using Levye::DocumentHandle;
using Levye::Serialization;

std::string FieldPath(const char* root, const char* field) {
  return std::string(root) + "." + field;
}

bool HasVector3(DocumentHandle doc, const std::string& path) {
  return Serialization::ContainsPath(doc, (path + ".x").c_str()) &&
         Serialization::ContainsPath(doc, (path + ".y").c_str()) &&
         Serialization::ContainsPath(doc, (path + ".z").c_str());
}

}  // namespace

bool WriteGameDataManual(Levye::DocumentHandle doc, const char* root, const GameSaveData& s) {
  if (!root || !*root) return false;

  const std::string base(root);

  return Serialization::SetInt(doc, (base + ".version").c_str(), s.version) &&

         Serialization::SetVector3(doc, (base + ".player.position").c_str(), s.player.position) &&

         Serialization::SetVector3(doc, (base + ".player.velocity").c_str(), s.player.velocity) &&

         Serialization::SetFloat(doc, (base + ".player.facingAngle").c_str(),
                                 s.player.facingAngle) &&

         Serialization::SetBool(doc, (base + ".player.grounded").c_str(), s.player.grounded) &&

         Serialization::SetFloat(doc, (base + ".camera.yaw").c_str(), s.camera.yaw) &&

         Serialization::SetFloat(doc, (base + ".camera.pitch").c_str(), s.camera.pitch) &&

         Serialization::SetFloat(doc, (base + ".camera.distance").c_str(), s.camera.distance) &&

         Serialization::SetInt(doc, (base + ".progress.currentLevel").c_str(),
                               s.progress.currentLevel) &&

         Serialization::SetInt(doc, (base + ".progress.unlockedLevel").c_str(),
                               s.progress.unlockedLevel) &&

         Serialization::SetBool(doc, (base + ".progress.checkpointActivated").c_str(),
                                s.progress.checkpointActivated) &&

         Serialization::SetBool(doc, (base + ".progress.completed").c_str(),
                                s.progress.completed) &&

         Serialization::SetVector3(doc, (base + ".progress.respawnPosition").c_str(),
                                   s.progress.respawnPosition);
}

bool ReadGameDataManual(Levye::DocumentHandle doc, const char* root, GameSaveData& destination) {
  if (!root || !*root) return false;

  const std::string base(root);

  const char* required[] = {"version",
                            "player.facingAngle",
                            "player.grounded",
                            "camera.yaw",
                            "camera.pitch",
                            "camera.distance",
                            "progress.currentLevel",
                            "progress.unlockedLevel",
                            "progress.checkpointActivated",
                            "progress.completed"};

  for (const char* field : required) {
    const std::string path = FieldPath(root, field);

    if (!Serialization::ContainsPath(doc, path.c_str())) return false;
  }

  if (!HasVector3(doc, base + ".player.position") || !HasVector3(doc, base + ".player.velocity") ||
      !HasVector3(doc, base + ".progress.respawnPosition"))
    return false;

  GameSaveData loaded{};

  loaded.version = static_cast<int>(Serialization::GetInt(doc, (base + ".version").c_str(), -1));

  loaded.player.position = Serialization::GetVector3(doc, (base + ".player.position").c_str());

  loaded.player.velocity = Serialization::GetVector3(doc, (base + ".player.velocity").c_str());

  loaded.player.facingAngle =
      static_cast<float>(Serialization::GetFloat(doc, (base + ".player.facingAngle").c_str()));

  loaded.player.grounded = Serialization::GetBool(doc, (base + ".player.grounded").c_str());

  loaded.camera.yaw =
      static_cast<float>(Serialization::GetFloat(doc, (base + ".camera.yaw").c_str()));

  loaded.camera.pitch =
      static_cast<float>(Serialization::GetFloat(doc, (base + ".camera.pitch").c_str()));

  loaded.camera.distance =
      static_cast<float>(Serialization::GetFloat(doc, (base + ".camera.distance").c_str()));

  loaded.progress.currentLevel =
      static_cast<int>(Serialization::GetInt(doc, (base + ".progress.currentLevel").c_str(), -1));

  loaded.progress.unlockedLevel =
      static_cast<int>(Serialization::GetInt(doc, (base + ".progress.unlockedLevel").c_str(), -1));

  loaded.progress.checkpointActivated =
      Serialization::GetBool(doc, (base + ".progress.checkpointActivated").c_str());

  loaded.progress.completed = Serialization::GetBool(doc, (base + ".progress.completed").c_str());

  loaded.progress.respawnPosition =
      Serialization::GetVector3(doc, (base + ".progress.respawnPosition").c_str());

  // Only replace destination after reading all required fields.
  destination = loaded;
  return true;
}

}  // namespace Platformer3D
