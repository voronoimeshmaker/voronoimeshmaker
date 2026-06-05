Block 5 Completion
==================

Purpose
-------

Block 5 connects the existing planar-cell geometry layer to the canonical
finite-volume mesh model. It reconstructs nodes, faces, owners, neighbours,
boundary patches and cell-face connectivity from clipped polygonal cells.

Implemented artefacts
---------------------

Public builder
   ``vmm::mesh::FiniteVolumeMeshBuilder2D``.

Convenience API
   ``vmm::mesh::build_finite_volume_mesh_2d``.

Tests
   ``tests/mesh/tst_finite_volume_mesh_builder_2d.cpp``.

Current scope
-------------

The builder consumes exterior rings from ``PlanarCell2D`` and emits a canonical
``FiniteVolumeMesh2D``. It deduplicates vertices with a configurable tolerance,
uses shared undirected edges to create one internal face, stores owner/neighbour
relations, and assigns a default boundary patch to boundary faces.

The current block intentionally does not assign rich GIS boundary semantics.
That remains Block 6.

Exit criteria
-------------

Block 5 is complete when:

* a single square cell produces four boundary faces;
* adjacent cells share one internal face;
* shared vertices are deduplicated;
* the reconstructed mesh passes the index/connectivity/geometry audits;
* the text audit writer can serialise the reconstructed mesh.
