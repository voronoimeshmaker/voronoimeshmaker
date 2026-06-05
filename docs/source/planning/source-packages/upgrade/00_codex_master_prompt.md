# Master prompt for Codex

You are working on the C++20 library VoronoiMeshMaker, VMM. The project is being upgraded to support MohidNG and future GIS-driven Voronoi finite-volume mesh workflows.

Your task is not to make a small patch. Your task is to help evolve the library architecture while preserving the design rules below.

## Core design rules

1. The VMM core must follow Data-Oriented Design.
2. Prefer Structure of Arrays, SoA, contiguous vectors, offsets, spans and batch processing.
3. Avoid object graphs such as `Cell` owning vectors of `Face*`, `Node*` or `Cell*`.
4. Avoid runtime polymorphism per cell, face, node or stencil entry.
5. Runtime polymorphism, Strategy or Factory is acceptable only at the algorithm selection boundary.
6. The public API must not expose CGAL, GDAL, OGR, GEOS, PROJ, PETSc, Trilinos, Voro++ or JIGSAW types.
7. External libraries must be isolated inside backend or I/O modules.
8. The VMM does not solve PDEs and does not assemble physical matrices.
9. The VMM produces geometry, topology, patches, audit data and solver-neutral finite-volume stencil connectivity.
10. MohidNG or another consumer adapts this data to PETSc, Trilinos or another backend.
11. Every operation that can change indices must produce auditable results.
12. A mesh with invalid indices, inconsistent owner/neighbour relations or invalid patches must not be exported as valid.

## Main target

Create a robust 2D finite-volume Voronoi mesh pipeline first:

GIS or synthetic domain
    -> site generation
    -> Delaunay/Voronoi tessellation
    -> clipping by domain and holes
    -> explicit finite-volume mesh
    -> nodes, cells, faces, owner, neighbour, patches
    -> audit
    -> text audit report
    -> HDF5 package for MohidNG
    -> solver-neutral stencil graph

Do not start with full 3D Voronoi, local remeshing or advanced GIS. The first milestone is an explicit, auditable, DOD 2D finite-volume mesh.

## First implementation priority

Implement the following first:

1. Strong ID types, `CellId`, `FaceId`, `NodeId`, `SiteId`, `BoundaryPatchId`.
2. DOD mesh tables, `NodeTable2D`, `CellTable2D`, `FaceTable2D`, `BoundaryPatchTable`.
3. `FiniteVolumeMesh2D`.
4. `CellFaceConnectivity`, using offsets and contiguous `FaceId` arrays.
5. Face reconstruction from clipped polygonal Voronoi cells.
6. `MeshIndexAudit2D`.
7. `MeshConnectivityAudit2D`.
8. Column-aligned text audit writer.
9. MohidNG HDF5 writer for 2D.
10. Minimal integration test that MohidNG can read the produced HDF5 package.

Use the files in this ZIP as the design source of truth.
