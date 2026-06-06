Block 0 Completion Record
=========================

Status
------

Block 0 is complete.

Purpose
-------

Block 0 captured the Codex proposal packages, converted them into repository
planning records, reviewed the proposed block roadmap against the current
VoronoiMeshMaker repository, and defined the implementation scope for Block 1.

Completed artefacts
-------------------

The following planning artefacts are now part of the repository:

* Planning section index: docs/planning/index.rst
* Consolidated proposal record: docs/planning/upgrade-proposals.rst
* Roadmap review and adjusted block sequence: docs/planning/block-roadmap-review.rst
* Block 1 definition: docs/planning/block-1-definition.rst
* Original upgrade package archive contents: docs/planning/source-packages/upgrade
* Original visual identity package contents: docs/planning/source-packages/visual-identity

Accepted decisions
------------------

The project will keep the current repository layout rooted at VMMLib/include/vmm
for the near-term restructuring. A disruptive top-level include migration is not
part of the first implementation blocks.

The mesh-oriented mesh core will be finite-volume and face-based. The first
implementation target is not GIS, HDF5, remeshing, or 3D; it is the canonical 2D
finite-volume data model.

Strong IDs and invalid sentinel policy are cross-cutting foundation work and are
therefore included in Block 1.

Manual canonical meshes will be used before connecting the existing clipped
Voronoi pipeline. This gives audits and exporters a stable target before face
reconstruction from generated cells is introduced.

HDF5 export remains a first milestone, but it comes after canonical tables,
index/connectivity/geometry audits, and text audit diagnostics.

GIS input, raster fields, remeshing by site editing, and 3D extrusion remain
planned, but they are downstream of the explicit 2D finite-volume mesh.

Adjusted roadmap summary
------------------------

The adjusted implementation sequence is:

* Block 1: Strong IDs and finite-volume DOD 2D tables.
* Block 2: Index audit for the canonical FV mesh.
* Block 3: Connectivity and geometry audit.
* Block 4: Text audit writer.
* Block 5: Face reconstruction from clipped polygonal cells.
* Block 6: Boundary patches for real domains with holes.
* Block 7: HDF5 writer 2D.
* Block 8: Solver-neutral stencil graph.
* Block 9: Projection and orthogonality anomaly audits.
* Block 10: GIS vector input and CRS handling.
* Block 11: Raster bathymetry/topography.
* Block 12: Remeshing by site editing.
* Block 13: Extruded 3D finite-volume mesh.
* Block 14: HDF5 writer 3D.
* Block 15: Optional advanced backends.

Block 1 authorization
---------------------

Block 1 is authorized to begin after this record. Its scope is limited to strong
ID types, canonical 2D finite-volume tables, basic helpers, and tests for manual
in-memory meshes.

Block 1 must not implement CGAL integration, face reconstruction, GIS input,
HDF5 export, text audit writing, solver stencil construction, 3D extrusion, or
remeshing algorithms.

Validation
----------

The documentation build passed after adding the planning section. A fresh build
should be run after this completion record is added.
