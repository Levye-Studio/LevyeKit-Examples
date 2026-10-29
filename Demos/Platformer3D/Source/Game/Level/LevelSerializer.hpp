#pragma once

#include "EditableLevel.hpp"

namespace Platformer3D {

enum class SerializationMethod { Manual, Archive };

class LevelSerializer {
 public:
  static bool Save(const EditableLevel& level, const char* path, SerializationMethod method);

  static bool Load(const char* path, EditableLevel& destination, SerializationMethod method);

  static bool CompareMethods(const EditableLevel& level);

 private:
  static bool SaveManual(const EditableLevel&, const char*);
  static bool LoadManual(const char*, EditableLevel&);

  static bool SaveArchive(const EditableLevel&, const char*);
  static bool LoadArchive(const char*, EditableLevel&);
};

}  // namespace Platformer3D
