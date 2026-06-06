Block 6 Completion: Boundary Patches for Domains With Holes
===========================================================

Purpose
-------

Block 6 makes the canonical finite-volume mesh capable of carrying explicit
boundary patch semantics for real 2D domains. The implementation focuses on the
mesh contract and validation layer: exterior land boundaries, islands, no-flux
edges, open boundaries, inlets, outlets, and future GIS-derived boundaries can
now be represented without introducing closed enums.

Implemented Artifacts
---------------------

* ``vmm::mesh::BoundaryPatchRing2D`` describes a geometric boundary ring, a
  patch name, and a flexible ``BoundaryPatchType`` trait.
* ``vmm::mesh::FiniteVolumeMeshBuilder2DOptions`` now accepts boundary patch
  rings and a separate boundary matching tolerance.
* ``vmm::mesh::FiniteVolumeMeshBuilder2D`` classifies boundary faces against
  the configured rings while preserving the default patch as a fallback for
  unclassified boundary faces.
* ``vmm::audit::MeshPatchAudit2D`` validates patch identity, patch metadata,
  and prevents boundary faces from using the structural ``internal`` patch type.
* Tests cover boundary classification for an exterior ring plus island ring,
  fallback behaviour, tolerance validation, and patch-audit failures.

Design Decisions
----------------

Boundary patch categories remain open. The project policy is to avoid enums for
extension points that would force edits across multiple files whenever a new
boundary family is introduced. Patch semantics therefore use small trait values
and ordinary data:

* ``BoundaryPatchTypeTraits::LandBoundary`` for exterior land boundaries;
* ``BoundaryPatchTypeTraits::Island`` for hole/island boundaries;
* ``BoundaryPatchTypeTraits::OpenBoundary``, ``Inlet``, ``Outlet``, and
  ``NoFlux`` for common hydrodynamic cases;
* custom ``BoundaryPatchType`` values for application-specific extensions.

The builder does not attempt GIS clipping in this block. It assigns patch ids to
boundary faces that already exist in the supplied planar cells. This keeps Block
6 aligned with the current mesh-construction layer and leaves shapefile/GDAL
integration as a separate importer concern.

Exit Criteria
-------------

Block 6 is complete when:

* boundary patch rings can be passed to the canonical FV mesh builder;
* boundary faces on exterior and island rings receive distinct patch ids;
* boundary faces that do not match a ring keep the default fallback patch;
* patch semantics are covered by an audit API;
* the mesh API and implementation-status documentation describe the new layer;
* focused C++ tests and Sphinx documentation build successfully.
