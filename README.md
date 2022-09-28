<!-- TODO:
4 Build
- Scripts for building main apps

5 Tests
- CTest api

8 Screenshots
- Make them svg
- Add screenshots

9 Badges
- https://forthebadge.com/
- CI
- Tests

11 Recomendations
- Check different github themes
- Width 100 characters
- Contribution guideline
- Project page with GitHub pages?
- Meetup speech
- Changelog
- Project news
- Make fixes to my personal profile page on github 

14 How to Use? Maybe wiki?
18 Support
- GitHub?
- Patreon?
- Other?

20 Info
- Readme gives x5 to project stars
- Talk to every community individually
- Try to be in Trending section, but topics is also ok
-->

<div align="center">
    <img 
      width="100" 
      alt="TermGraph" 
      src="apps/TermGraphApp/resources/applicationIcons/appIcon128x128.png">
    <h1>TermGraph</h1>
</div>


<div align="center">

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)](https://www.qt.io/)
[![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![Conan](https://img.shields.io/badge/Conan-6699cb?style=for-the-badge&logo=conan&logoColor=white)](https://conan.io/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)]()
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://play.google.com/store/apps/details?id=app.termgraph&pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1)
[![WebAssembly](https://img.shields.io/badge/WebAssembly-654FF0?style=for-the-badge&logo=WebAssembly&logoColor=white)]()

</div>

<div align="center">

[⚡️ Description](#%EF%B8%8F-description) • [🌿 Key features](#-key-features) • [🚀 Installation](#-installation) • [🗺 Roadmap](#-roadmap) • [🧬 Tech stack](#-tech-stack) • [⚙️ Build](#%EF%B8%8F-build) • [💠 Project structure](#-project-structure) • [☕️ Tests](#%EF%B8%8F-tests)

</div>

# ⚡️ Description
TermGraph is a terms diagram builder. This app helps to arrange terms from simple to complex and build a map of definitions.

## What for?
**Short answer:** This will help you learn something new faster and with deep understanding.

**Long anser:**
In all well-structured areas of knowledge, it is easy to see how explanations are built up on each other. <br>
The most basic concepts are easily accessible to any beginner. More complex concepts require more effort and are always explained in terms of simpler ones.

If you try to draw the first few "layers" of these terms, it will look like a ladder or lasagna) If you draw them all, you will get a knowledge area map, which can be very useful.

<img>

### Benefits for beginners (if you don't have the map yet)
- Its construction guaranteed learning with active memorization
- Wikipedia wandering can become a good teacher with this approach (I have checked)

### Benefits for beginners (if you already have such map):
- Understanding what to learn and in what order
- Easier to google something
- If you forgot the term, but remember it's connections - you can easily find it
- You see the "big picture" of knowledge

### Benefits for the experienced:
- Blind spots of your knowledge become visible
- Fights the illusion of knowledge
- The learning curve gets higher

### Benefits for experts:
- Assistance in the transfer of experience
- Understanding how your expertise relates to other areas of knowledge

# 🌿 Key features
- Builds on Desktop, Wasm and Android
- Supports big maps (at least 1500 nodes)
- Devices synchronization
- Semi-automatic links markup

# 🚀 Installation
<a href='https://play.google.com/store/apps/details?id=app.termgraph&pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' width="160" height="60" src='https://play.google.com/intl/en_us/badges/static/images/badges/en_badge_web_generic.png'/></a>
- Desktop: build from sources on ubuntu (yet)
- Wasm frontend and data backend: build from sources

# 🗺 Roadmap
- [x] Make "hard/quick links" with uuid
- [ ] [Fully automatic link search](https://github.com/SavenkovIgor/TermGraph/issues/5)
- [ ] [Map layers](https://github.com/SavenkovIgor/TermGraph/issues/6)

# 🧬 Tech stack
- <img src="https://isocpp.org/favicon.ico" width="16" height="16"> C++ 20
- <img src="https://www.qt.io/hubfs/2016_Qt_Logo/qt_logo_green_rgb_16x16.png" width="16" height="16"> Qt 6.3 | Qml
- <img src="https://cmake.org/wp-content/uploads/2019/05/cropped-cmake_512-32x32.png" width="16" height="16"> Cmake 3.22+
- <img src="https://conan.io/favicon.png" width="16" height="16"> Conan 1.51+
- [RESTinio lib](https://github.com/Stiffstream/restinio) (for REST server)
- [GTest lib](https://github.com/google/googletest) (tests)
- [Outcome lib](https://github.com/ned14/outcome) (would be replaced with std::expected when available)

# ⚙️ Build
Build for ubuntu:
- Install conan [here](https://docs.conan.io/en/latest/installation.html)
- Install Qt + Qml via [online installer](https://www.qt.io/download-qt-installer)
- Build in QtCreator with Desktop Kit

Build for ubuntu from CLI will appear later

# 💠 Project structure
```bash
├── apps/                
│   ├── TermGraphApp/    # Main application. Buils for desktop, android and wasm(for now) 
│   ├── TermGraphBack/   # Rest api data storage. For server only
│   └── TermGraphFront/  # Reserved folder. Not used yet
├── cmake/             # Cmake tools
├── conanfiles/        # Additional conan files
├── libs/                
│   ├── CommonTools/          # Tools common for all apps
│   ├── QmlAtoms/             # Qml atoms components     (atomic design)
│   ├── QmlMolecules/         # Qml molecules components (atomic design)
│   ├── QmlPages/             # Qml pages
│   ├── QmlTheme/             # Qml theme. Colors, icons, fonts etc...
│   ├── QmlTools/             # Some qml debugging tools
│   ├── QrcHelpImages/        # Help images resources
│   ├── TermDataConnection/   # Library, connecting to REST api server
│   ├── TermDataInterface/    # Interface, describing data connection interface 
│   └── TermDataStorage/      # Data storage sqlite library
├── tools/             # Some useful scripts (build of server, clang fromat etc...)
├── CMakeLists.txt     # Root cmake list
└── conanfile.py       # conanfile for dependency description
```

# ☕️ Tests
Tests working only with desktop target. GTest library installed via conan. Start tests from QtCreator
Tests for ubuntu from CLI with CTest will appear later.

<br/>
<br/>

[![LinkedIn](https://img.shields.io/badge/LinkedIn-SavenkovIgor-555555?style=for-the-badge&logo=linkedin&logoColor=white&labelColor=0077B5)](https://www.linkedin.com/in/savenkovigor-dev/)
<br/>
[![Built with ❤️](https://img.shields.io/badge/Coded%20with-%E2%9D%A4%EF%B8%8F-e36d25?style=for-the-badge)](https://github.com/SavenkovIgor/TermGraph)

