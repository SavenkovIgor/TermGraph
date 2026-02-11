# TermGraph Agent Guidelines

## Style

- **C++**: Use 4 spaces for indentation. Functions and variables use `snake_case`; classes use `CamelCase`.
- **Python**: Use 4 spaces for indentation. Follow `snake_case` for variables and functions, and `CamelCase` for classes.

## Build

- Build the project in the `desktop_dev` configuration using:
  `./project.py --build --preset desktop_dev`

## Testing

- Run C++ tests with `./project.py --test --preset desktop_dev`

## Pull Requests

- Include a concise summary of changes.
- Mention test results in a **Testing** section.
- Branch names SHOULD be ASCII/English ONLY

## Project Dependencies

### Where to Find Dependency Versions

**Framework & Language:**

- **C++ Standard**: See `CMakeLists.txt` (CXX_STANDARD), `conanfiles/profile/base/clang_compiler` (compiler.cppstd)
- **Qt**: See `project.py`, `tools/bootstrap.py` (QT_VERSION), `.github/workflows/ci.yml`, `.devcontainer/Dockerfile`

**Qt Modules Used:**

- All builds: `Core`, `Gui`, `Qml`, `Quick`, `QuickControls2`, `Network`, `Svg` (see `CMakeLists.txt` find_package)
- Desktop only: `Sql`
- Additional Qt archives: `qtimageformats`, `qtshadertools`, `qttranslations`, `qttools`, `qtsvg`, `qtdeclarative`, `qtbase`, `icu`
  - Full list in `.github/workflows/ci.yml` (install-qt-action steps) and `.devcontainer/Dockerfile` (aqt commands)

**Conan Packages:**

- See `conanfile.py` for all Conan dependencies
- Currently: **gtest** (test_requires, desktop only)
- Required: Conan profiles MUST include `compiler.cppstd=23` for modern gtest compatibility

**Git Submodules:**

- See `.gitmodules` for direct submodules
- `third_party/stemming` → [OleanderStemmingLibrary](https://github.com/Blake-Madden/OleanderStemmingLibrary) (header-only stemming)
  - Has test submodules: `snowball-data`, `utfcpp` (with `ftest`) - see `third_party/stemming/.gitmodules`

### Build Infrastructure Versions

**Core Toolchain:**

- **CMake**: See `CMakeLists.txt` (cmake_minimum_required), `CMakePresets.json` (cmakeMinimumRequired)
- **Ninja**: See `.devcontainer/Dockerfile` (download URL with version)
- **Clang**: See `tools/bootstrap.py` (CLANG_VERSION), `.github/workflows/ci.yml`, `.devcontainer/Dockerfile`
- **Conan**: See `tools/bootstrap.py` (CONAN_VERSION), `.github/workflows/ci.yml`, `.devcontainer/Dockerfile`
- **Emscripten**: See `.github/workflows/ci.yml` (EMSDK_VERSION), `.devcontainer/Dockerfile`

**Utilities:**

- **aqtinstall**: See `tools/bootstrap.py` (AQT_VERSION), `.github/workflows/ci.yml`, `.devcontainer/Dockerfile`
- **ccache**: CI only, version in `.github/workflows/ci.yml`
- **uv**: Latest from install script (`.devcontainer/Dockerfile`)
- **Python 3**: System default, used by `project.py`, `tools/bootstrap.py`

**System Environment:**

- Base Docker image: See first line of `.devcontainer/Dockerfile`
- System packages: See `.devcontainer/Dockerfile` and `tools/bootstrap.py` (BASE_PACKAGES)

### Important Notes

- Desktop builds include SQL support and testing; WASM builds exclude both
- Conan profiles must specify `compiler.cppstd=23` (see `conanfiles/profile/base/clang_compiler`)
- Git submodules initialized automatically by `project.py` on first build
- CI/CD and DevContainer use identical dependency versions - check both for consistency
