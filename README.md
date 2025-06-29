<div align="center">
    <a href="https://savenkovigor.github.io/TermGraph/"><img
      width="100"
      alt="TermGraph"
      src="resources/applicationIcons/appIcon128x128.png"></a>
    <h1>TermGraph</h1>
</div>

<div align="center">

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)](https://www.qt.io/)
[![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![Conan](https://img.shields.io/badge/Conan-6699cb?style=for-the-badge&logo=conan&logoColor=white)](https://conan.io/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)]()
[![WebAssembly](https://img.shields.io/badge/WASM-654FF0?style=for-the-badge&logo=WebAssembly&logoColor=white)](https://emscripten.org/index.html)

</div>

<div align="center">

[Description](#%EF%B8%8F-description) • [Key features](#-key-features) • [SetUp](#-setup) • [Roadmap](#-roadmap) •
[Tech stack](#-tech-stack)

[Build](#hammer_and_wrench-build) • [Tests](#alembic-tests) • [Project structure](#microscope-project-structure) • [Credits](#pushpin-credits)

</div>

# ⚡️ Description

TermGraph is a terms diagram builder. It helps to make a hierarchical projection of a terms-space onto 2d plane and build a map of definitions.
Like this, but bigger:

```mermaid
graph RL
classDef stdN fill:#1f5f5f,stroke-width:0px;
Node("<b><i>Node</i></b></br>some point in space"):::stdN
Edge("<b><i>Edge</i></b></br>line, connecting two {nodes}"):::stdN
Arc("<b><i>Arc</i></b></br>{edge}, that has a direction"):::stdN
Graph("fa:fa-diagram-project <b><i>Graph</i></b></br>set of {nodes} and associated with them {edges}"):::stdN

Edge --> Node
Arc --> Edge
Graph -.-> Node
Graph --> Edge
```

## What for?

**Short answer:** This will help you learn something new faster and with deep understanding.

**Long answer:**
In all well-structured areas of knowledge, it is easy to see how explanations are built up on each other.

The most basic concepts are easily accessible to any beginner. More complex concepts require more effort and are always explained in terms of simpler ones.

If you try to draw the first few "layers" of these terms, it will look like a ladder or lasagna) If you draw them all, you will get a knowledge area map, which can be very useful.

### Benefits for beginners (if you don't have the map yet)

- Its construction guarantees learning with active memorization
- Wikipedia wandering can become a good teacher with this approach (I have checked)

### Benefits for beginners (if you already have such map)

- Understanding what to learn and in what order
- Easier to google something
- If you forgot the term, but remember it's connections - you can easily find it
- You see the "big picture" of knowledge

### Benefits for the experienced

- Blind spots of your knowledge become visible
- Fights the illusion of knowledge
- The learning curve gets higher

### Benefits for experts

- Assistance in the transfer of experience
- Understanding how your expertise relates to other areas of knowledge

## 🌿 Key features

- Builds on Desktop and Wasm
- Supports big maps (at least 1500 nodes)
- Devices synchronization
- Semi-automatic links markup

# 🚀 SetUp

- Desktop: build from sources on ubuntu (yet)
- Wasm: build from sources or available [here](https://termgraph.app)

## 🗺 Roadmap

- [x] Make "hard/quick links" with uuid
- [ ] [Fully automatic link search](https://github.com/SavenkovIgor/TermGraph/issues/5)
- [ ] [Map layers](https://github.com/SavenkovIgor/TermGraph/issues/6)

## 🧬 Tech stack

- <img src="https://isocpp.org/favicon.ico" width="16" height="16"> C++ 20
- <img src="https://www.qt.io/hubfs/2016_Qt_Logo/qt_logo_green_rgb_16x16.png" width="16" height="16"> Qt 6.8 | Qml
- <img src="https://cmake.org/wp-content/uploads/2019/05/cropped-cmake_512-32x32.png" width="16" height="16"> Cmake 3.22+
- <img src="https://conan.io/favicon.png" width="16" height="16"> Conan 2
- [GTest lib](https://github.com/google/googletest) (tests)

# :hammer_and_wrench: Build

[![Build app](https://github.com/SavenkovIgor/TermGraph/actions/workflows/AppBuild.yml/badge.svg)](https://github.com/SavenkovIgor/TermGraph/actions/workflows/AppBuild.yml)

## Requirements for ubuntu hosted build

- [Conan](https://docs.conan.io/en/latest/installation.html)
- [Cmake](https://cmake.org/) + Ninja
- [Qt + Qml](https://www.qt.io/download-qt-installer)

You can build project with QtCreator, using CMake Presets (which QtCreator more or less understands).

Also you can build it with python script at repository root.

Try `./project.py --deps-install --build --run`, it would build and run desktop version

## :alembic: Tests

Tests working only with desktop target. GTest library installed via conan, only for desktop target.

You can start tests from:

- QtCreator **or**
- run `ctest ./` in build folder at `build/desktop_(dev|release)` **or**
- run `./project.py --test` at repository root

## :microscope: Project structure

```bash
├── .github/workflows/       # Github actions
├── conanfiles/              # Conan profiles, and generated stuff
├── doc/                     # Documents
├── data/                    # Folder with static data
├── qml/                     # Main.qml file
│   ├── Atoms/               # Qml atoms components     (atomic design)
│   ├── Molecules/           # Qml molecules components (atomic design)
│   ├── Pages/               # Qml pages
│   ├── Theme/               # Qml theme. Colors, icons, fonts etc...
│   ├── Tools/               # Some qml debugging tools
├── resources/               # Resources: app icons, main html wrapper etc...
├── source/                  # Main source code
│   ├── CommonTools/         # Some common tools with wide reuse
│   ├── graph/               # Graph headers
│   ├── QrcHelpImages/       # Help images resources
│   ├── staticDataStorage/   # Static data storage class
│   ├── TermDataInterface/   # Interface, describing data connection interface
│   ├── TermDataStorage/     # Data storage sqlite
│   └── Text                 # Text processing
├── tests/                   # Tests
├── tools/                   # Some side stuff, need from time to time
├── CMakeLists.txt           # Main CMake file
├── CMakePresets.json        # CMake presets
├── conanfile.txt            # Conan dependencies (gtest)
└── project.py               # Python script for building and running project
```

## :pushpin: Credits

[![LinkedIn](https://img.shields.io/badge/LinkedIn-SavenkovIgor-555555?style=for-the-badge&logo=linkedin&logoColor=white&labelColor=0077B5)](https://www.linkedin.com/in/savenkovigor-dev/)

[![Built with ❤️](https://img.shields.io/badge/Coded%20with-%E2%9D%A4%EF%B8%8F-e36d25?style=for-the-badge)](https://github.com/SavenkovIgor/TermGraph)
