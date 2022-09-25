<!-- 
TODO:
1 Philosophy

2 Key features
- Comparison with other projects

3 Installation

4 Build
- Configuration
- Depends from Qt 6.3
- Platforms list (Android, Wasm, Desktop)
- Description of modules (with table)
- Scripts for building main apps

5 Tests
- How to start them
- CTest api

8 Screenshots
- Make them svg
- Add screenshots
- Redraw app icon (golden ratio for hexagons?)

9 Badges
- https://forthebadge.com/
- linkedin profile
- license
- CI
- Tests

10 Contents
- Make proper contents links
- Think about what must be there

11 Recomendations
- Check different github themes
- Width 100 characters
- Contribution guideline
- Project page with GitHub pages?
- Meetup speech
- Changelog
- Project news
- Make fixes to my personal profile page on github 

12 Add quote
13 Main site link
14 How to Use? Maybe wiki?
15 Installation
16 Built with ❤️ by ...
18 Support
- GitHub?
- Patreon?
- Other?

19 Icons 
- ✔️❌🎉⭐️🥳🔍
- https://gist.github.com/rxaviers/7360908

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

[Description](#%EF%B8%8F-description) • [Key features](#-key-features) • [Installation](#-installation) • [Roadmap](#-roadmap) • [Tech stack](#-tech-stack) • [Project structure](#-project-structure)
<!-- • Philosophy-->
<!-- • Build-->
<!-- • Tests-->

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


<!-- # 📜 Philosophy -->
# 🌿 Key features
- Builds on Desktop, Wasm and Android
- Supports big maps (at least 1500 nodes)
- Devices synchronization
- Semi-automatic links markup

# 🚀 Installation
<a href='https://play.google.com/store/apps/details?id=app.termgraph&pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' width="160" height="60" src='https://play.google.com/intl/en_us/badges/static/images/badges/en_badge_web_generic.png'/></a>
- Desktop: build from sources on ubuntu (yet)
- Wasm frontend and data backend: build from sources

<!-- # ⚙️ Build -->

<!-- # ☕️ Tests -->

# 🗺 Roadmap
- [x] Make "hard/quick links" with uuid
- [ ] [Fully automatic link search](https://github.com/SavenkovIgor/TermGraph/issues/5)
- [ ] [Map layers](https://github.com/SavenkovIgor/TermGraph/issues/6)

# 🧬 Tech stack
- <img src="https://cmake.org/wp-content/uploads/2019/05/cropped-cmake_512-32x32.png" width="16" height="16"> Cmake 3.22+
- <img src="https://www.qt.io/hubfs/2016_Qt_Logo/qt_logo_green_rgb_16x16.png" width="16" height="16"> Qt 6.3 | Qml
- <img src="https://conan.io/favicon.png" width="16" height="16"> Conan 1.51+
- [RESTinio lib](https://github.com/Stiffstream/restinio) (for REST server)
- [GTest lib](https://github.com/google/googletest) (tests)
- [Outcome lib](https://github.com/ned14/outcome) (would be replaced with std::expected when available)

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

<!-- # Syntax examples
## Center image

<br> <br />

<div align="center"><a href=""><img width="128" alt="Alt text" src="img/img.png"></a></div>

<h1>h1</h1>
<h2>h2</h2>
<h3>h3</h3>
<h4>h4</h4>
<h5>h5</h5>
<h6>h6</h6>
<strong>strong</strong>

<details>
  <summary>Summary</summary>
  <ol>
    <li><a href="#-some">List 1</a></li>
    <li><a href="#-some">List 2</a></li>
  </ol>
</details>

[External link description](https://link.io/).
[Local file link](LICENSE)
![Some more local file](./local_file.png)
![Img](https://link.io/file.svg "Some info")

```bash
code example 
```
> ItemAngle
+ ItemPlus
- ItemMinus
>- ItemMinus
>+ ItemPlus

> **Warning**

> **Note**

**Option** -->
