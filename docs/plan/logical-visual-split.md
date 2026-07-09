# Logical / Visual Split: Architecture Plan

> Goal: C++ owns logical graph structure (which term is at which level/column).
> QML owns pixel geometry and all animations.

## Current Architecture (as-is)

### Layer Stack

```plaintext
source/graph/                           ← pure graph theory, zero Qt/visual coupling
  Forest<NodeT, EdgeT>                    BFS levels, cycle removal, connected components
  Graph<NodeT, EdgeT>                     bondedSubgraphs() per connected component

source/model/                           ← where the two worlds collide
  PaintedTerm  : Node<TermData>           + GraphicItem (hand-rolled scene proxy)
  PaintedEdge  : Edge<PaintedTerm, …>     + GraphicItem
  PaintedForest: Forest<PaintedTerm, …>   + NodeVerticalStack[] (one stack per BFS level)
  TermGroup                               god-class: builds graph + computes pixel layout

TermsModel / EdgesModel                 ← QAbstractListModel bridge
  setGroup() → beginResetModel()          destroys ALL QML delegates on every group switch

QML
  NodesScene.qml                          Repeater over TermsModel; imperative edge rebuild
  Term.qml                                x: rect.x; y: rect.y  (final pixels from C++)
  Edge.qml                                ShapePath with pt1/pt2 (final pixels from C++)
```

### What C++ Sends to QML Today

| Model        | Role                                                         | Value |
|---           |---                                                           |---    |
| `TermsModel` | `rect`                                                       | `QRectF(scenePos, pixelSize)` — final pixels |
| `TermsModel` | `radius`, `type`, `area`, `isSelected`, `term`, `definition` | display data |
| `EdgesModel` | `pt1`, `pt2`                                                 | `QPointF` scene centers — final pixels |
| `EdgesModel` | `isSelected`, `edgeType`, `edgeSelection`                    | display data |
| `MainScene`  | `sceneRect`                                                  | total canvas bounding box — final pixels |

### TermGroup Constructor Flow

```plaintext
TermGroup(info, termData)
  ├─ create PaintedTerm list
  ├─ collapseSynonyms()
  ├─ searchAllConnections()   → edges, ghost nodes
  ├─ bondedSubgraphs()        → one PaintedForest per connected component
  ├─ allBrokenEdges() pruning
  ├─ addTreeRectsToScene()    → GraphicItem parent chain
  ├─ addOrphansToParents()
  ├─ setTreeCoords()          → PaintedForest::setTreeNodeCoords()
  │    └─ NodeVerticalStack::placeTerms() → term->setPos(QPointF)   ← pixels set here
  ├─ setOrphCoords()          → orphan row-wrap layout → term->setPos()
  ├─ updateRectsPositions()   → positions each forest rect inside group
  └─ updateBaseRectSize()     → total group bounding box
```

### Edge Rendering Anti-Pattern

Edges are **not** in a `Repeater`. On every `edgesChanged` signal, `NodesScene.qml` does:

```js
function updateEdges() {
    // destroy all existing ShapePath objects
    for (let i = 0; i < data.length; ++i) data[i].destroy();
    // read all edge data imperatively via raw role indices
    data = edgesData().filter(...).map(e => edgeComponent.createObject(parent, e));
}
```

Full rebuild on every signal. Zero animation possible with this approach.

## Problems

### 1. `TermGroup` God-Class

One class handles: graph construction, synonym collapsing, BFS subgraph decomposition, per-forest pixel layout, orphan row-wrap layout, group bounding rect computation — all in the constructor.
Logical structure and pixel geometry are inseparable.

### 2. `GraphicItem` Is Mute

`GraphicItem::mPosition` is a plain `QPointF` — no `QProperty`, no signal on change.
QML learns about position updates only via `beginResetModel` (= destroy all delegates).

### 3. `beginResetModel` Blocks Animation

`TermsModel::setGroup()` calls `beginResetModel` / `endResetModel` → the QML `Repeater` tears down
and recreates **all** delegates. No transition animation is possible: old delegates are gone before new ones appear.

### 4. Imperative Edge Rebuild

`createObject` / `destroy` cycle on every `edgesChanged`.
Delegates have no stable lifetime — impossible to animate `pt1`/`pt2` between states.

## Layout Hierarchy (Two Levels)

A `TermGroup` contains **multiple** forests (one per connected component) **plus** orphan terms:

```plaintext
TermGroup
  ├── PaintedForest 0   (connected component, left-to-right tree)
  │     ├── NodeVerticalStack[0]  (depth 0 — roots)
  │     ├── NodeVerticalStack[1]  (depth 1)
  │     └── NodeVerticalStack[N]  (depth N — leaves)
  ├── PaintedForest 1
  ├── …
  └── Orphans           (isolated terms, row-wrapped)
```

This means layout has two distinct responsibilities:

- `ForestLayout` — assigns `{depth, stackPos}` per term *within one forest*.
  Extracted from `PaintedForest::setTreeNodeCoords()` + `NodeVerticalStack::placeTerms()`.

- `GroupLayout` — stacks multiple forest rects vertically inside the group + positions orphan block.
  Extracted from `TermGroup::updateRectsPositions()` + `setOrphCoords()`.

## Target Architecture

### Core Idea

C++ computes **logical position** only
QML translates logical position → pixel position and owns all animations

```plaintext
graph::Forest<TermData, EdgeData>        ← unchanged, pure logic

ForestLayout                             ← NEW: per-forest logical layout
  input:  Forest<TermData, EdgeData>
  output: map<uuid, {depth, stackPos}>

GroupLayout                              ← NEW: group-level arrangement
  input:  vector<ForestLayout> + orphan list
  output: GroupPos per term (`ForestsPos` or `OrphanPos`)

TermsModel                               ← CHANGED: stable identity, logical roles
  roles: uuid, forestIndex, depth, stackPos,
         text, termAndSynonyms, definition, nodeType, area, isSelected
  update strategy: dataChanged() per item, never beginResetModel

QML: Repeater { model: Api.scene.terms }
  Term.qml:
    x: model.depth      * (nodeWidth  + depthSpacing)
    y: model.stackPos   * (nodeHeight + stackSpacing)
         + model.forestIndex * forestVerticalOffset
    Behavior on x { NumberAnimation { duration: 300; easing.type: Easing.InOutCubic } }
    Behavior on y { NumberAnimation { duration: 300; easing.type: Easing.InOutCubic } }
```

### `ForestPos`, `OrphanPos`, `GroupPos` Structs

```cpp
struct ForestPos {
  size_t depth;       // column (left-to-right depth in tree)
  size_t stackPos;    // row within column (vertical position in stack)
};
```

```cpp
struct OrphanPos {
  size_t index; // ordinal position inside orphan list
};

using ForestsPos = std::pair<size_t, ForestPos>; // {forestIndex, localPos}

struct GroupPos {
  std::variant<ForestsPos, OrphanPos> pos;
};
```

### What Stays in C++

- Forest topology: which term is at which `ForestsPos {forestIndex, depth, stackPos}` or `OrphanPos {index}`
- Node identity (uuid), text, definition, type, area
- Edge topology: `{rootUuid, leafUuid, edgeType}`
- Selection state

### What Moves to QML

- Pixel spacing constants (`nodeWidth`, `nodeHeight`, `depthSpacing`, `stackSpacing`, `forestVerticalSpacing`)
- `x = depth * (nodeWidth + depthSpacing)`
- `y = stackPos * (nodeHeight + stackSpacing) + forestIndex * forestVerticalSpacing`
- `sceneRect`: derived from max `depth` and `stackPos` across all terms
- All colors, radius, corner styles (already in QML)
- All animations (`Behavior on x/y/width/height`, edge `pt1`/`pt2`)

### Edge Endpoint Resolution

Eventual goal: `EdgesModel` exposes `{rootUuid, leafUuid}`.
QML resolves pixel endpoints from the live positions of term delegates by uuid.
(Intermediate step: C++ can still provide `pt1`/`pt2` derived from logical coords while QML transitions.)

## Migration Plan

Each phase leaves the application in a working, buildable state.

### Phase A — `ForestPos` + `GroupPos` + `ForestLayout` + `GroupLayout` (C++ only)

- [x] Create lightweight position structs (no C++ modules):
  `source/model/group/ForestPos.hpp`, `source/model/group/OrphanPos.hpp`, `source/model/group/GroupPos.hpp`
  `GroupPos` stores `variant<ForestsPos, OrphanPos>`, where `ForestsPos = pair<size_t, ForestPos>`
  and all structs are Qt meta-object compatible (`Q_GADGET`, `Q_DECLARE_METATYPE`).
- [x] Add new position headers to `target_sources` in `CMakeLists.txt` for stable Qt build integration.
- [ ] Create `ForestLayout` class: takes one `Forest<>`, returns `map<uuid, {depth, stackPos}>`
      Extract logic from `PaintedForest::setTreeNodeCoords()` + `NodeVerticalStack::placeTerms()`
- [ ] Create `GroupLayout` class: takes `vector<ForestLayout>` + orphan list, assigns `ForestsPos`
  Extract logic from `TermGroup::updateRectsPositions()` + `setOrphCoords()`
  Orphans get `OrphanPos { index }`
- [ ] `TermGroup` delegates to `ForestLayout` + `GroupLayout`, still calls `GraphicItem::setPos()` (backward compat)
- [ ] Add unit tests for `ForestLayout` in `test/`

**Verify:** `./project.py --test --preset desktop_dev` green, visual output unchanged.

### Phase B — Expose Logical Roles in `TermsModel`

- [ ] Add roles `forestIndex`, `depth`, `stackPos` to `TermsModel`
      alongside existing `rect` role — fully backward compatible
- [ ] QML still uses `rect.x`/`rect.y` — no visual change

**Verify:** build passes, roles queryable from QML debug console.

### Phase C — Split `MainScene`

`MainScene` currently owns: group loading, model management, selection state, hit-testing, search, term name↔uuid resolution.

- [ ] Extract `SelectionManager`: holds selected term, hit-test (`setMouseClick`), `getTermPosition`, `search`, `termUuidToName`, `termNameToUuid`
- [ ] `MainScene` delegates to `SelectionManager`; becomes thinner: group loading + model ownership only
- [ ] Expose `SelectionManager` as `Api.scene.selection` or keep behind `MainScene` — decide at implementation time

**Verify:** no QML changes needed, all existing invokables still work.

### Phase D — `TermsModel` with Stable Identity

This phase **unblocks animations**.

- [ ] Track current term UUIDs in `TermsModel`
- [ ] On `setGroup()`: diff old vs new uuid sets
  - removed terms → `beginRemoveRows` / `endRemoveRows`
  - added terms → `beginInsertRows` / `endInsertRows`
  - changed `ForestPos`/`GroupPos` → `emit dataChanged(forestIndex, depth, stackPos roles)`
- [ ] `Repeater` delegates survive group switches; only truly new/removed terms are created/destroyed

**Verify:** switch groups repeatedly — delegates reuse is visible via `Component.onCompleted` logging.

### Phase E — QML Computes Geometry

- [ ] `NodesScene.qml`: define spacing constants
      `readonly property real nodeWidth: 180`, `nodeHeight: 60`, `depthSpacing: 40`, `stackSpacing: 20`, `forestVerticalSpacing: 60`
- [ ] `Term.qml`: replace `x: rect.x; y: rect.y` with:

  ```qml
  x: model.depth     * (NodesScene.nodeWidth  + NodesScene.depthSpacing)
  y: model.stackPos  * (NodesScene.nodeHeight + NodesScene.stackSpacing)
       + Math.max(0, model.forestIndex) * NodesScene.forestVerticalSpacing
  ```
  
- [ ] `NodesScene.qml`: derive `width`/`height` from `maxDepth`/`maxStackPos` properties on `TermsModel`
      (or compute in QML via `Repeater` childrenRect)
      Remove `Api.scene.sceneRect` dependency
- [ ] Remove `rect` role from `TermsModel` once QML no longer reads it

**Verify:** pixel geometry matches previous layout (tune spacing constants to match `AppStyle::Sizes` values).

### Phase F — `EdgesModel` uuid-Based + Edge `Repeater`

- [ ] Add roles `rootUuid`, `leafUuid` to `EdgesModel` (alongside existing `pt1`/`pt2`)
- [ ] `NodesScene.qml`: replace `createObject`/`destroy` imperative loop with `Repeater { model: Api.scene.edges }`
- [ ] `Edge.qml`: resolve `pt1`/`pt2` from term delegate positions by uuid
      (use a `Map` in `NodesScene` keyed by uuid → delegate item, or a `ListModel` lookup)
- [ ] Remove `pt1`/`pt2` roles from `EdgesModel` once QML no longer reads them

**Verify:** all edges render correctly with Repeater.

### Phase G — Animations (future)

- [ ] `Term.qml`: add `Behavior on x { NumberAnimation { duration: 300; easing.type: Easing.InOutCubic } }`
- [ ] `Term.qml`: add `Behavior on y { … }`
- [ ] `Edge.qml`: animate `pt1`/`pt2` (requires uuid-based endpoint resolution from Phase F)
- [ ] Verify graph transition animation when switching groups

### Phase H — Optional: `QCanvasPainter` Acceleration Track (Qt 6.12+)

Use this phase only if profiling still shows edge rendering as the main bottleneck after Phases D-F.
Reference (applies to this phase only): Qt Blog, "Qt Canvas Painter: Accelerated performance using paths" — https://www.qt.io/blog/qt-canvas-painter-accelerated-performance-using-paths

- [ ] Upgrade runtime/tooling to Qt 6.12+ in a dedicated branch and validate desktop build + tests
  (`QCanvasPainter`/`QCanvasPath` are not available in current Qt 6.11.x baseline)
- [ ] Introduce custom edge renderer item (`QCanvasPainterItem`-based) while keeping terms in current QML delegates
- [ ] Build and cache edge geometry via `QCanvasPath` instead of rebuilding JS/QML `ShapePath` objects
- [ ] Split path groups by update frequency, e.g. `STATIC_EDGES` and `SELECTED_EDGES`
- [ ] Invalidate/rebuild cached paths only for changed groups (selection change, graph diff), not for whole scene
- [ ] Keep pan/zoom and color animation on GPU-side transforms/brush changes without path rebuild
- [ ] Add feature flag to switch between current QML edge renderer and CanvasPainter renderer for A/B checks

**Verify:** compare FPS and frame time on large groups, confirm lower CPU usage and no visual regressions.

**Exit criteria for keeping Phase H:** measurable gain against Phase F baseline (for example, stable 60 FPS on target scenes and lower average frame time).

---

## Files Affected

| File | Change |
|---|---|
| `source/model/group/ForestPos.hpp` | **NEW** — forest-local logical position (`depth`, `stackPos`) |
| `source/model/group/OrphanPos.hpp` | **NEW** — orphan-list logical position (`index`) |
| `source/model/group/GroupPos.hpp` | **NEW** — variant of `ForestsPos` and `OrphanPos` |
| `source/model/group/ForestLayout.hpp/.cpp` | **NEW** — per-forest layout |
| `source/model/group/GroupLayout.hpp/.cpp` | **NEW** — group-level arrangement |
| `source/model/group/termgroup.cpp` | delegate to ForestLayout + GroupLayout |
| `source/model/group/paintedforest.cpp` | extract layout logic into ForestLayout |
| `source/model/group/nodeverticalstack.cpp` | extract into ForestLayout or keep as impl detail |
| `source/model/term/termsmodel.hpp/.cpp` | add logical roles; replace beginResetModel with diff |
| `source/model/edge/edgesmodel.hpp/.cpp` | add rootUuid/leafUuid roles |
| `source/MainScene.hpp/.cpp` | extract SelectionManager |
| `source/Molecules/NodesScene.qml` | spacing constants; edge Repeater; sceneRect from QML |
| `source/Atoms/Term.qml` | x/y from depth/stackPos; add Behavior (Phase G) |
| `source/Atoms/Edge.qml` | uuid-based pt1/pt2 resolution; animate (Phase G) |
| `source/*` (new custom renderer files) | optional `QCanvasPainterItem` edge renderer (Phase H) |
| `CMakeLists.txt` | includes new position headers in `target_sources` |
| build/toolchain files (`project.py`, CI, devcontainer) | optional Qt 6.12+ migration work for Phase H |
| `test/` | unit tests for ForestLayout |

> `source/graph/` — **not touched**. Already clean.
> `source/model/graphicitem/` — can be retired incrementally as QML takes over geometry.
