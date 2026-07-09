# Plan: Multi-Group Graph with Visibility Filters

## Goal

Load all JSON data files at startup, merge all terms into a single unified graph,
preserve group membership on each node, and provide a per-group visibility filter
so the user can show/hide groups interactively.

## Current State

| Aspect | Current behavior |
|---|---|
| Data loading | `GroupsManager` loads one group at a time on demand |
| Scene | `MainScene::currentGroup` holds a single `TermGroup*` |
| Cross-group links | Not resolved — ghost nodes created for foreign terms |
| Filter | None; only selection-based highlighting exists |
| Group membership on node | `TermData::groupUuid` field already exists |

Key files:

- `source/staticDataStorage/StaticDataStorage.cpp` — loads all JSONs from QRC
- `source/managers/groupsmanager.hpp` — manages group lifecycle
- `source/model/group/termgroup.cpp` — builds forest, resolves links, positions nodes
- `source/MainScene.hpp` — scene controller exposed to QML
- `source/model/term/termsmodel.hpp` / `source/model/edge/edgesmodel.hpp` — Qt models for QML
- `source/Molecules/NodesScene.qml` — renders nodes and edges

## Approach

Merge all groups into **one unified `TermGroup`** at startup.
`TermData::groupUuid` already exists on every node, so group membership is preserved.
`searchAllConnections()` inside `TermGroup` will then naturally resolve cross-group links.
A new `GroupFilterModel` tracks per-group visibility; `TermsModel` / `EdgesModel` respect it.

This approach reuses almost all existing machinery and minimises new abstractions.

## Phases

### Phase 1 — Load everything at startup

**Goal:** All terms from all JSON files are available in memory from the moment the app starts.

**Changes:**

1. **`GroupsManager`** — add `loadAllGroups() -> QList<TermGroup*>` (or return merged data):
   - Call `StaticDataStorage::groups()` which already returns all groups.
   - Collect `TermData::List` from every group.
   - Collect all `GroupSummary` objects.
   - Keep them ready; do not create individual `TermGroup` objects yet.

2. **`GroupsManager`** — add `buildUnifiedGraph() -> TermGroup*`:
   - Concatenate all `TermData::List` from all loaded groups.
   - Create a single `TermGroup(syntheticSummary, allTerms)`.
   - The synthetic summary can have an empty UUID and name like `"(All groups)"`.

3. **`MainScene`**:
   - On startup, call `GroupsManager::buildUnifiedGraph()`.
   - Store the result as `mUnifiedGroup`.
   - `currentGroup` property now always returns `mUnifiedGroup`.
   - Remove the per-group selection flow (`selectGroup()`), or keep it as a filter shortcut.

**No changes to `TermGroup` constructor** — it already accepts any `TermData::List`.

### Phase 2 — Group-aware layout

**Goal:** When rendered together, terms from the same source group stay visually clustered.

**Changes:**

1. **`TermGroup` positioning** (`setTreeCoords`, `setOrphCoords`):
   - After `bondedSubgraphs()`, group the resulting forests by `term->data().groupUuid`.
   - Arrange group clusters in a grid/row layout (e.g., one row per source group).
   - Each cluster gets an offset so groups don't overlap.
   - Add helper: `QPointF groupOffset(GroupUuid)` — maps a group UUID to its top-left corner.

2. **`TermGroup`** — add `QRectF groupRect(GroupUuid) const`:
   - Returns the bounding rect of all nodes in that group.
   - Useful for "jump to group" navigation and for the filter highlight.

Layout algorithm sketch:

```plaintext
For each source group G (in load order):
    collect all forests whose roots belong to G
    lay them out (existing logic, unchanged)
    place the result at offsetX += previousGroupWidth + MARGIN
```

This is the most complex part of the plan. Isolate it behind a feature flag
(`USE_GROUPED_LAYOUT`) initially so you can fall back to the flat layout.

### Phase 3 — Cross-group edge resolution

**Goal:** Links that point to terms in other groups are resolved, not ghost-nodes.

**Assessment:** This is **free** once Phase 1 is done.

`TermGroup::searchAllConnections()` iterates over all terms in `mGraphData` and matches
links by UUID or text. With all terms merged into one `TermGroup`, cross-group matches
will succeed without any code changes.

**Verify** by checking that ghost-node count drops when loading multiple groups.

**Edge type:** Consider adding a new `EdgeType::CrossGroup` variant in
`source/enums/edge_type.cpp` so cross-group edges can be styled differently (e.g., dashed).
This requires:

- Adding the enum value.
- Tagging the edge at creation time: `if (root->groupUuid() != leaf->groupUuid()) type = CrossGroup`.
- Styling in `NodesScene.qml` / `Edge.qml`.

### Phase 4 — Group filter model

**Goal:** A Qt model that lists all source groups and exposes a per-group `visible` flag.

**New class: `GroupFilterModel`** (`source/model/filter/groupfiltermodel.hpp`):

```cpp
class GroupFilterModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { UuidRole, NameRole, VisibleRole, SizeRole, ColorRole };

    // QAbstractListModel interface
    int rowCount(...) const override;
    QVariant data(const QModelIndex&, int role) const override;
    bool setData(const QModelIndex&, const QVariant&, int role) override;  // for VisibleRole

    Q_INVOKABLE void setVisible(QUuid groupUuid, bool visible);
    Q_INVOKABLE void showAll();
    Q_INVOKABLE void hideAll();
    Q_INVOKABLE void showOnly(QUuid groupUuid);

    // Returns the set of currently visible group UUIDs
    QSet<QUuid> visibleGroups() const;

signals:
    void visibilityChanged();

private:
    struct Entry { GroupSummary summary; bool visible = true; QColor color; };
    QList<Entry> mEntries;
};
```

Populate it from `GroupsManager` after `loadAllGroups()`.
Assign a distinct color to each group (palette of ~10 colors, cycling).

`MainScene` — add property:

```cpp
Q_PROPERTY(GroupFilterModel* groupFilter READ groupFilter CONSTANT)
```

Wire up: when `groupFilter::visibilityChanged()` fires → update `TermsModel` + `EdgesModel`.

### Phase 5 — Filtered models

**Goal:** `TermsModel` and `EdgesModel` only expose nodes/edges that belong to visible groups.

#### Option A — Proxy model (preferred, less invasive)

Wrap both models with `QSortFilterProxyModel` subclasses:

- **`VisibleTermsProxy`**: `filterAcceptsRow()` checks `term->data().groupUuid` against `GroupFilterModel::visibleGroups()`.
- **`VisibleEdgesProxy`**: accepts edge if **both** endpoints are visible (or at least one — expose as a setting).

`MainScene` exposes the proxy models instead of the raw models to QML. Raw models remain unchanged.

#### Option B — In-place flag

Add `bool mVisible` to `PaintedTerm`. `TermsModel::data()` skips invisible terms.
Simpler but mixes filter state into the painted term.

**Recommendation:** Option A (proxy model). It keeps `PaintedTerm` clean and makes
the filter easy to remove or extend later.

### Phase 6 — QML filter panel

**Goal:** User can toggle group visibility from the UI.

**New component: `GroupFilterPanel.qml`** (`source/Molecules/GroupFilterPanel.qml`):

```qml
Drawer {
    id: filterDrawer
    edge: Qt.RightEdge

    ListView {
        model: Api.scene.groupFilter
        delegate: RowLayout {
            // colored dot  |  group name  |  Switch
            Rectangle { color: model.color; radius: width/2 }
            Text { text: model.name }
            Switch { checked: model.visible
                     onToggled: Api.scene.groupFilter.setVisible(model.uuid, checked) }
        }
    }

    footer: RowLayout {
        Button { text: "All";  onClicked: Api.scene.groupFilter.showAll()  }
        Button { text: "None"; onClicked: Api.scene.groupFilter.hideAll() }
    }
}
```

`MainScene.qml` changes:

- Add a filter button (funnel icon) in the toolbar.
- `onClicked: filterDrawer.open()`.
- Import and instantiate `GroupFilterPanel`.

## Work Order and Dependencies

```plaintext
Phase 1  ──►  Phase 3 (free, just verify)
   │
   └──►  Phase 2 (layout)  ─────────────────────────────────────────────►  (cosmetic)
   │
   └──►  Phase 4 (GroupFilterModel)
              │
              └──►  Phase 5 (proxy models)
                         │
                         └──►  Phase 6 (QML panel)
```

Phases 1 → 4 → 5 → 6 are the critical path.
Phase 2 (layout) and Phase 3 (cross-group styling) can be done independently afterwards.

## Files to Create

| File | Purpose |
|---|---|
| `source/model/filter/groupfiltermodel.hpp` | GroupFilterModel declaration |
| `source/model/filter/groupfiltermodel.cpp` | GroupFilterModel implementation |
| `source/model/filter/visibletermsproxy.hpp` | QSortFilterProxyModel for terms |
| `source/model/filter/visibleedgesproxy.hpp` | QSortFilterProxyModel for edges |
| `source/Molecules/GroupFilterPanel.qml` | Filter drawer UI |

## Files to Modify

| File                                    | What changes                                              |
|---                                      |---                                                        |
| `source/managers/groupsmanager.hpp/cpp` | Add `loadAllGroups()`, `buildUnifiedGraph()`              |
| `source/MainScene.hpp/cpp`              | Add `mUnifiedGroup`, `groupFilter` property, startup load |
| `source/model/group/termgroup.cpp`      | Group-aware layout (Phase 2)                              |
| `source/enums/edge_type.cpp`            | Add `CrossGroup` edge type (Phase 3, optional)            |
| `source/Molecules/NodesScene.qml`       | Expose proxy models, style CrossGroup edges               |
| `source/Pages/MainScene.qml`            | Add filter button + GroupFilterPanel                      |
| `source/Api.hpp`                        | Possibly expose GroupFilterModel at top level             |
| Relevant `CMakeLists.txt` files         | Register new source files and QML components              |

## Open Questions

1. Layout strategy for merged graph: Should all groups be laid out as one big forest,
   or should each group be positioned as an independent cluster?
   → Recommend independent clusters (Phase 2). The flat layout will be very wide/messy.

2. Edge visibility when one endpoint is hidden: Hide the edge entirely, or show a stub?
   → Start with "hide edge if either endpoint is hidden". Revisit if users want stubs.

3. Performance: With all groups merged, the graph may have thousands of nodes.
   Check if `TermsModel`/`EdgesModel` need pagination or virtualization.

4. Search scope: Currently `MainScene::search()` searches within `currentGroup`.
   After merging, it will search all groups — is that desired, or should it respect the filter?

5. Node editing: Currently editing works on the active group. With a merged graph,
   saving edits must still write to the correct source group's JSON. Verify `GroupsManager`
   routes writes via `TermData::groupUuid`.

## Out of Scope (for this plan)

- Network-backed data sources (already handled separately by `NetworkManager`)
- Dynamic reloading / hot-reload of JSON files
- Per-node tags (beyond group membership)
- Graph analytics / statistics
