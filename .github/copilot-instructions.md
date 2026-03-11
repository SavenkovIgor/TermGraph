# Copilot Instructions

This file provides guidance to GitHub Copilot when working in this repository.

## Project Overview

TermGraph is a Qt/QML C++ knowledge graph builder that visualizes hierarchical term definitions as 2D diagrams. It targets both Linux desktop (via SQLite) and WebAssembly (via static JSON). This project is intentionally used as a playground for modern C++ features.

## Build Commands

The primary build interface is `project.py` at the repo root. All builds target Linux (desktop uses Clang 20, WASM uses Emscripten).

```bash
# Install Conan dependencies + build + run (desktop release)
./project.py --deps-install --build --run

# Build only, specify preset
./project.py --build --preset desktop_dev

# Full rebuild from scratch
./project.py --rebuild --preset desktop_dev

# Initialize git submodules (required once after clone)
./project.py --init-submodules
```

Available presets: `desktop_dev`, `desktop_release`, `wasm_dev`, `wasm_release`

## Running Tests

Tests are desktop-only (GTest via Conan). WASM builds have no tests.

```bash
# Run all tests
./project.py --test --preset desktop_dev

# Run a specific test by name pattern (from build dir)
cd build/desktop_dev && ctest -R <test_name_pattern> --verbose

# Run a specific GTest case directly
./build/desktop_dev/TermGraphTest --gtest_filter=TestClass.TestName
```

Test files are in `test/`. Note: `DbWorksTest` and `ConnectionWorksTest` are currently disabled.

## Linting / Formatting

Clang-format 20 is used. No integrated lint command in `project.py` - run manually:

```bash
find source -name "*.cpp" -o -name "*.h" | xargs clang-format-20 -i
```

CI runs a clang-format check on `wasm_release` builds only.

CMake flags strict warnings: `-Werror -Wall -Wextra -Wpedantic -Wconversion` and more. All warnings are errors.

## Architecture

### Storage backends (platform-split)

- **Desktop:** SQLite via `Qt::Sql` - implemented in `source/TermDataStorage/`. Tables: `TermTable`, `TermGroupTable`, `AppConfigTable`.
- **WASM / offline:** JSON files in `data/` - implemented in `source/staticDataStorage/`. Compile-time define `STATIC_STORAGE` selects this path.
- Both implement the abstract `DataStorageInterface` from `source/TermDataInterface/`.

### Graph core (`source/graph/`)

Template-based graph theory primitives using C++20 modules and `std::ranges`. `Graph<NodeT, EdgeT>` holds collections of `Node`/`Edge` typed via template params. `Forest` represents a set of disconnected trees within a graph.

### Model layer (`source/model/`)

Qt models (`TermsModel`, `EdgesModel`) expose graph data to QML. `TermGroup` handles layout algorithms. `GraphTerm` and `GraphEdge` are the rendered scene items. `PaintedForest` orchestrates rendering.

### Managers (`source/managers/`)

Business logic lives here: `GroupsManager` (CRUD on groups), `NetworkManager` (device sync), `NotificationManager`, `LinkHardeningManager` (auto-detection of term links using stemming).

### Text processing (`source/Text/`)

`TextCursor`, `TextRange`, `Stemmer` (wrapping `third_party/stemming` submodule), and text utilities power the semi-automatic link markup feature.

### QML UI - Atomic Design

UI follows atomic design in the `source/` tree:

- `Atoms/` - primitive components (buttons, text)
- `Molecules/` - composite components (drawers, cards)
- `Pages/` - full screens (`MapPage`, `SettingsPage`)
- `Theme/` - centralized colors, icons, fonts

### Entry points

- `source/main.cpp` -> creates Qt app, loads QML engine
- `source/Main.qml` -> root QML component
- `source/MainScene.cpp/h` -> C++ scene orchestrator with `Q_INVOKABLE` methods
- `source/Api.cpp/h` -> primary QML-C++ bridge
- `source/DataProvider.cpp/h` -> coordinates data loading

### Platform abstraction

`source/CommonTools/Platform.h` detects build target. `source/CommonTools/desktop/` and `source/CommonTools/wasm/` provide platform-specific `PlatformTypes.h`. UUID types (`TermUuid`, `GroupUuid`) live in `source/CommonTools/`.

## Key Development Notes

- **Submodule:** `third_party/stemming` must be initialized before building.
- **Qt version:** Pinned to 6.8.3; set `QT_ROOT` and `QT_VERSION` environment variables.
- **C++ standard:** C++23 with C++20 modules actively used.
- **CMake minimum:** 4.0 (note: higher than the 3.22 stated in README).
- **Compiler:** Clang 20 for desktop; Emscripten 3.1.56 for WASM.
- **Static data categories** (WASM knowledge graphs): `Asm`, `Cpp`, `Global`, `GraphTheory`, `InformationTechnologies`, `Qml`, `Rationality`, etc. - JSON files in `data/`.