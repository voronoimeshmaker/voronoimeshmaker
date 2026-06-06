Block Roadmap Review
====================

Purpose
-------

This page reviews the proposed block roadmap against the current repository
state. It records adjustments before implementation starts.

Current repository state
------------------------

The current repository already has:

* ``VMMLib/include/vmm`` as the public include root;
* CGAL isolated under private backend source directories;
* 2D domain geometry and clipped Voronoi cells;
* a DOD/CSR ``MeshTopology`` for cell-centred topology;
* VTK diagnostics for current 2D outputs;
* placeholder or explicit-failure exporters for formats that require faces;
* empty reserved files such as ``Mesh.hpp``, ``MeshBuilder.hpp``, ``MeshValidation.hpp``, and ``Handles.hpp``.

The main gap is explicit face-centred finite-volume geometry. OpenFOAM,
UGRID, text audit, and finite-volume stencil export all depend on this gap being
closed first.

Review conclusion
-----------------

The proposed roadmap is technically sound, but it should be adapted to the
existing repository in three ways:

* keep the current ``VMMLib/include/vmm`` layout instead of moving to a fresh top-level ``include`` directory now;
* split the original Block 1 into a small foundation block and a finite-volume table block, because strong IDs and sentinel policy are cross-cutting API decisions;
* move HDF5 dependency integration after text audit and basic audits are stable, because CMake currently does not find HDF5.

Adjusted block sequence
-----------------------

Block 0, Planning and documentation capture
   Store proposal source material, consolidate decisions, review the roadmap, and
   define Block 1. This page and its siblings are the exit artefacts.

Block 1, Strong IDs and finite-volume DOD 2D tables
   Introduce strong ID types, invalid sentinel policy, canonical 2D FV tables,
   basic size/range helpers, and focused unit tests. Do not connect CGAL yet.

Block 2, Index audit for canonical FV mesh
   Implement ``MeshAuditReport`` and ``MeshIndexAudit2D`` for owners,
   neighbours, patches, cell-face offsets, face IDs, and node IDs. Add valid and
   intentionally invalid manual mesh tests.

Block 3, Connectivity and geometry audit
   Add owner/neighbour reciprocity checks, face occurrence checks, duplicate
   cell-face detection, minimum face count, positive cell area, positive face
   length, and basic normal checks.

Block 4, Text audit writer
   Write stable, column-aligned 2D audit output from the canonical mesh and use
   manual meshes for regression tests.

Block 5, Face reconstruction from clipped polygonal cells
   Convert existing clipped Voronoi polygon cells into the canonical FV mesh:
   shared edge detection, global face creation, owner/neighbour assignment,
   boundary face detection, patch assignment, face centres, normals, lengths, and
   cell-face connectivity.

Block 6, Boundary patches for real domains with holes
   Extend patch assignment so outer boundaries, islands, walls, open boundaries,
   and no-flow regions are represented explicitly and deterministically.

Block 7, HDF5 writer 2D
   Add HDF5 as an I/O dependency, write the mesh package groups, and
   enforce audit-before-export. If reader integration is not available,
   add an internal schema validation test first.

Block 8, Solver-neutral stencil graph
   Build compact internal and boundary stencil arrays from the canonical face
   table. Keep backend solver adaptation outside VMM core.

Block 9, Projection and orthogonality anomaly audits
   Add boundary projection and internal face midpoint anomaly reporting. Report
   first; do not automatically alter operators or remesh.

Block 10, GIS vector input and CRS handling
   Introduce GDAL/OGR and PROJ behind ``vmm::gis`` and convert real GIS features
   to VMM-owned domain and patch types.

Block 11, Raster bathymetry/topography
   Add raster reading and interpolation after vector domains and metric CRS are
   stable.

Block 12, Remeshing by site editing
   Add site edit batches and remeshing plans that rebuild the canonical mesh and
   rerun mandatory audits.

Block 13, Extruded 3D finite-volume mesh
   Extrude stable 2D finite-volume meshes vertically and add 3D audits and text
   audit output.

Block 14, HDF5 writer 3D
   Extend the HDF5 package to extruded 3D once 3D tables and audits are stable.

Block 15, Optional advanced backends
   Evaluate JIGSAW, Voro++, regular triangulation, or other backends without
   leaking external types into public APIs.

Rationale for changes
---------------------

The original roadmap put face reconstruction before some audit work. In this
repository, manual canonical meshes should come first. They create a stable test
target and make every later importer, reconstructor, or exporter accountable to
the same invariants.

The original roadmap also grouped boundary patches early. The data table belongs
in Block 1, but full patch semantics for real holes, islands, and open
boundaries should wait until face reconstruction exists.

HDF5 export should remain a first milestone, but not before canonical tables,
audits, and text diagnostics are passing. Otherwise invalid mesh packages become
too easy to write and too hard to debug.
