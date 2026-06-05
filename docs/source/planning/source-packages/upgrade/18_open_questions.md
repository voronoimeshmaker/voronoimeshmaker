# Open questions

These questions are intentionally left open for later design decisions.

## Mesh dimensionality

Should `FiniteVolumeMesh2D` and `FiniteVolumeMesh3D` be separate concrete types, or should there also be a dimension-generic template?

Recommendation for now: implement concrete 2D first.

## ID generation

Should IDs be plain contiguous indices or generation-based ids?

Recommendation for now: contiguous strong ids, design so generation-based ids can be added later.

## HDF5 schema evolution

Should VMM write only MohidNG schema or also a VMM-native HDF5 schema?

Recommendation for now: write MohidNG schema and optional VMM audit groups.

## GIS first format

Which GIS vector format should be first?

Recommendation: GeoPackage if GDAL support is available, Shapefile as fallback.

## Geometry kernel

Should VMM use CGAL everywhere internally for geometry, or convert to VMM-owned types between operations?

Recommendation: keep CGAL inside backend and convert to VMM-owned types at boundaries.

## Internal constraints

How should internal lines that are not holes be represented?

Possible roles:

- refinement_feature,
- internal_interface,
- breakline,
- no_flux_barrier,
- open_internal_boundary.

Recommendation: solve holes and islands first.

## Boundary projection use in operators

Should boundary orthogonal projections be used by default for numerical distances?

Recommendation: store and audit first. Do not use automatically until numerical implications are tested.

## Internal face anomaly handling

Should anomalous internal faces trigger remeshing automatically?

Recommendation: initially report only. Later remeshing algorithms may use this as input.

## 3D first mode

Should 3D begin with full Voronoi 3D or extruded 2D?

Recommendation: extruded 2D first.

## Solver-neutral stencil storage

Should stencil graph be stored in HDF5 by default?

Recommendation: optional at first. The canonical representation remains FaceTable.
