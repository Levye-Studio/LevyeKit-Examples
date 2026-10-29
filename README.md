# LevyeKit-Examples

Example projects demonstrating how to build games with [LevyeKit](https://github.com/Levye-Studio/LevyeKit), a lightweight C++20 framework built on [raylib](https://www.raylib.com/).

These examples focus on practical integration of LevyeKit systems, including native game-module hot reload and optional framework modules.

## Examples

### Platformer3D

**Location:** [`Demos/Platformer3D`](Demos/Platformer3D)

A third-person 3D platformer used to demonstrate how multiple LevyeKit systems work together in a game.

**Features**

- Third-person character movement, jumping, collision, checkpoints, and two playable levels.
- Skeletal robot animation, including idle, running, jumping, and falling states.
- Optional **Animation** module for playback and state transitions.
- Optional **Dear ImGui** debug interface for inspecting and editing gameplay values.
- Optional **Serialization** module for YAML save/load, using both the low-level Document API and the higher-level Archive API.
- In-game platform selection and editing through the debug interface.
- Native C++ game-module hot reload with compatible game state preserved.

The demo is intended to showcase the integration of **Animation + ImGui + Serialization**, rather than serve as a full game or editor.

## Getting Started

### Requirements

- Git
- CMake
- A C++20-compatible compiler
- A working LevyeKit build environment and its dependencies

### Clone the repositories

Place the framework and examples repositories alongside one another:

```text
Projects/
├── LevyeKit/
└── LevyeKit-Examples/
```

```bash
git clone https://github.com/Levye-Studio/LevyeKit.git
git clone https://github.com/Levye-Studio/LevyeKit-Examples.git
```

### Build Platformer3D

From the `LevyeKit-Examples` repository root:

**macOS / Linux (single-configuration generators):**

```bash
cmake -S Demos/Platformer3D -B Demos/Platformer3D/Build/Debug -DCMAKE_BUILD_TYPE=Debug
cmake --build Demos/Platformer3D/Build/Debug --parallel
```

**Windows (Visual Studio 2022, x64):**

```powershell
cmake -S Demos/Platformer3D -B Demos/Platformer3D/Build/Windows -G "Visual Studio 17 2022" -A x64
cmake --build Demos/Platformer3D/Build/Windows --config Debug --parallel
```

The Platformer3D CMake configuration expects a sibling `LevyeKit` checkout by default. If yours is elsewhere, specify its path during configuration:

```bash
cmake -S Demos/Platformer3D -B Demos/Platformer3D/Build/Debug -DLEVYE_ROOT=/absolute/path/to/LevyeKit -DCMAKE_BUILD_TYPE=Debug
```

Run the generated Platformer3D executable from the build's output location, with its required assets available. Exact executable locations depend on the generator and build configuration.

### Hot Reload

Start Platformer3D, edit its reloadable game-module source, and rebuild the game module while the application is running. LevyeKit detects and loads a compatible rebuilt module without restarting the host.

Changes to the persistent `GameState` layout require a restart; the hot-reload system rejects incompatible state sizes.

## Cross-Platform Verification

For the LevyeKit v0.4.0 release, the framework and Platformer3D were built, run, and hot-reloaded on:

| Platform | Toolchain | Build | Run | Hot reload |
| --- | --- | --- | --- | --- |
| macOS | Apple Silicon development environment | Verified | Verified | Verified |
| Linux | Arch Linux / GCC | Verified | Verified | Verified |
| Windows | Visual Studio 2022 / MSVC | Verified | Verified | Verified |

The ImGui, Animation, and both manual and Archive-based Serialization workflows were also exercised on Linux. The cross-platform results above do not imply that every individual feature received identical manual testing on every OS.

## Related Links

- [LevyeKit framework](https://github.com/Levye-Studio/LevyeKit)
- [LevyeKit documentation](https://github.com/Levye-Studio/LevyeKit#readme)
- [LevyeKit releases](https://github.com/Levye-Studio/LevyeKit/releases)
- [Levye Studio](https://levye.studio)

---

**Different by Design.**  
Levye Studio
