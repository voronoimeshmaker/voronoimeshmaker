# Non-functional requirements

## NFR-001, Data-Oriented Design

The VMM core shall follow Data-Oriented Design.

Use:

- contiguous arrays,
- Structure of Arrays,
- CSR-style offsets,
- std::span or equivalent views,
- batch operations,
- predictable memory layouts.

Avoid:

- per-cell object graphs,
- pointer networks between cells, faces and nodes,
- std::vector inside each cell,
- runtime polymorphism per mesh entity,
- dynamic allocation per face or cell.

## NFR-002, Solver independence

The VMM core shall not depend on PETSc, Trilinos, Eigen, Ginkgo or another solver backend.

The VMM shall produce solver-neutral geometry and connectivity.

## NFR-003, External backend isolation

External types shall not appear in the public core API.

Keep isolated:

- CGAL in geometry backend,
- GDAL/OGR in GIS backend,
- PROJ in CRS backend,
- GEOS in topology-cleaning backend,
- HDF5 in I/O backend,
- Voro++ and JIGSAW in optional tessellation backends.

## NFR-004, Geometric robustness

The VMM shall handle or report:

- small cells,
- small faces,
- islands,
- internal holes,
- concave corners,
- narrow channels,
- irregular GIS boundaries,
- near-duplicate points,
- short segments,
- internal face anomalies,
- boundary projection anomalies.

## NFR-005, Mandatory validation

Every mesh exported as valid shall pass:

- index audit,
- connectivity audit,
- geometry audit,
- patch audit,
- positive measure audit.

Fatal errors shall block export.

## NFR-006, Performance

The VMM shall be suitable for large meshes.

Design choices shall favour:

- contiguous memory,
- low per-entity overhead,
- preallocation,
- batch operations,
- avoiding virtual calls in internal loops.

## NFR-007, Future parallelism

The architecture shall allow future parallelisation of:

- cell area and centroid computation,
- face reconstruction,
- face audit,
- boundary projection,
- stencil graph construction,
- raster interpolation,
- export.

TBB, OpenMP or MPI may be considered later.

## NFR-008, Reproducibility

With the same input and options, VMM shall produce deterministic results where possible.

This includes:

- stable cell ordering,
- stable face ordering,
- reproducible ids,
- text reports suitable for diff,
- deterministic audit summaries.

## NFR-009, Portability

The core shall be C++20.

The build system shall be CMake.

Linux and WSL are primary development targets.

## NFR-010, Documentation

The VMM documentation shall have an institutional structure similar to MohidNG, but with VMM-specific content:

- overview,
- relation to MohidNG,
- GIS data,
- CRS,
- tessellation,
- remeshing,
- finite-volume mesh,
- audit,
- HDF5 export,
- text audit format,
- DOD policy,
- backend policy,
- licensing.

## NFR-011, Testability

Every block shall have tests.

Minimum tests:

- simple rectangular domain,
- domain with island,
- open boundary,
- face reconstruction,
- neighbour reciprocity,
- index audit,
- HDF5 export,
- MohidNG read compatibility,
- text audit output,
- solver-neutral stencil graph.

## NFR-012, Incremental evolution

The project shall evolve by stable blocks.

Do not implement advanced GIS, general 3D Voronoi or local remeshing before the explicit 2D finite-volume mesh is robust.
