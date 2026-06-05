# Data model, DOD finite-volume mesh

## Strong IDs

Use strong ID types, not raw integers everywhere.

Example:

```cpp
struct CellId {
    std::uint64_t value;
};

struct FaceId {
    std::uint64_t value;
};

struct NodeId {
    std::uint64_t value;
};

struct SiteId {
    std::uint64_t value;
};

struct BoundaryPatchId {
    std::uint64_t value;
};
```

Later, consider generational IDs:

```cpp
struct CellId {
    std::uint32_t index;
    std::uint32_t generation;
};
```

Generational IDs help detect stale handles after compaction or remeshing.

## NodeTable2D

```cpp
struct NodeTable2D {
    std::vector<Real> x;
    std::vector<Real> y;
};
```

## CellTable2D

```cpp
struct CellTable2D {
    std::vector<Real> centre_x;
    std::vector<Real> centre_y;
    std::vector<Real> area;

    std::vector<SiteId> site_id;

    std::vector<std::uint64_t> face_offset;
    std::vector<std::uint32_t> face_count;
};
```

## FaceTable2D

```cpp
struct FaceTable2D {
    std::vector<CellId> owner;
    std::vector<CellId> neighbour;

    std::vector<Real> centre_x;
    std::vector<Real> centre_y;

    std::vector<Real> normal_x;
    std::vector<Real> normal_y;

    std::vector<Real> length;

    std::vector<BoundaryPatchId> patch_id;

    std::vector<NodeId> node0;
    std::vector<NodeId> node1;
};
```

Use an invalid neighbour for boundary faces.

Use an invalid patch for internal faces.

## BoundaryPatchTable

```cpp
enum class BoundaryPatchType : std::uint8_t {
    internal,
    wall,
    island,
    inlet,
    outlet,
    open_boundary,
    land_boundary,
    no_flux,
    bottom,
    surface
};

struct BoundaryPatchTable {
    std::vector<BoundaryPatchId> id;
    std::vector<std::string> name;
    std::vector<BoundaryPatchType> type;
};
```

## CellFaceConnectivity

```cpp
struct CellFaceConnectivity {
    std::vector<FaceId> face_ids;
};
```

For cell c:

```text
begin = cell.face_offset[c]
count = cell.face_count[c]
faces = face_ids[begin : begin + count]
```

## FiniteVolumeMesh2D

```cpp
struct FiniteVolumeMesh2D {
    NodeTable2D nodes;
    CellTable2D cells;
    FaceTable2D faces;
    BoundaryPatchTable patches;
    CellFaceConnectivity cell_faces;
};
```

## Why not object graphs

Avoid:

```cpp
struct Cell {
    std::vector<Face*> faces;
    std::vector<Cell*> neighbours;
};
```

This creates fragmented memory, many allocations and hard-to-audit index behaviour.

## Internal fields

Keep fields in separate tables:

```cpp
struct CellFieldTable {
    std::vector<std::string> names;
    std::vector<std::uint64_t> offsets;
    std::vector<Real> values;
};
```

Do not store arbitrary fields inside each Cell object.

## Future 3D

Use analogous DOD tables:

- `NodeTable3D`,
- `CellTable3D`,
- `FaceTable3D`,
- face-to-node polygon offsets,
- cell volumes,
- face areas,
- face roles.

For 3D general polyhedral faces, use offsets:

```cpp
struct FaceNodeConnectivity3D {
    std::vector<std::uint64_t> node_offset;
    std::vector<std::uint32_t> node_count;
    std::vector<NodeId> node_ids;
};
```
