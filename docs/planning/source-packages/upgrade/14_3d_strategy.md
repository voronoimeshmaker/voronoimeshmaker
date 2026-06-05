# 3D strategy

## First 3D target

The first 3D target shall be vertical extrusion of a robust 2D finite-volume mesh.

Do not start with general polyhedral Voronoi 3D.

## Extruded 3D workflow

```text
FiniteVolumeMesh2D
    -> bathymetry/topography
    -> vertical layer specification
    -> extrusion
    -> FiniteVolumeMesh3D
    -> 3D audit
    -> 3D HDF5 export
```

## Extruded 3D entities

Generate:

- 3D nodes,
- 3D cells,
- lateral faces,
- bottom faces,
- top or surface faces,
- owner,
- neighbour,
- face area,
- face normal,
- cell volume,
- face role,
- patch id.

## Face roles

```cpp
enum class FaceRole3D : std::uint8_t {
    lateral,
    bottom,
    top,
    surface,
    internal,
    wall,
    island
};
```

## Ordering for text audit

For extruded 3D:

```text
lateral faces in clockwise order inherited from the 2D cell
bottom face
top face
```

For general 3D:

```text
order by face_id or deterministic geometric ordering
```

## Island in 3D

A 2D island becomes a vertically extruded solid column excluded from the fluid mesh.

The island boundary becomes lateral wall faces in all wet layers.

## General Voronoi 3D

General polyhedral Voronoi 3D is a future phase.

Possible backends:

- CGAL,
- Voro++,
- JIGSAW.

## Voro++ future role

Voro++ can be an optional backend for 3D cell-by-cell Voronoi computation, especially for convex or simple domains.

It does not replace:

- GIS processing,
- patch assignment,
- global face reconciliation,
- owner/neighbour audit,
- HDF5 export,
- stencil graph construction.
