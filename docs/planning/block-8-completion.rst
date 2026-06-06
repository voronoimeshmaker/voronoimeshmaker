Block 8 Completion: Solver-Neutral Stencil Graph
================================================

Purpose
-------

Block 8 derives compact finite-volume stencil arrays from the canonical 2D face
table. The graph is intentionally solver-neutral: it records topology and basic
geometry needed by downstream discretisation layers, but it does not choose
operator coefficients, boundary-condition formulas, time integration, or solver
backends.

Implemented Artifacts
---------------------

* ``vmm::mesh::FiniteVolumeStencilGraph2D`` stores separate internal and
  boundary stencil tables.
* ``InternalStencilTable2D`` stores face id, owner cell, neighbour cell,
  owner-to-neighbour vector, centre distance, and face length.
* ``BoundaryStencilTable2D`` stores face id, owner cell, boundary patch id,
  centre-to-face normal distance, outward normal, and face length.
* ``vmm::mesh::FiniteVolumeStencilGraphBuilder2D`` builds the graph from
  ``FiniteVolumeMesh2D``.
* Graph construction is audit-gated by default using the existing connectivity,
  geometry, and patch audits.
* Tests cover internal and boundary stencil extraction, patch preservation,
  graph storage consistency, and invalid-mesh rejection.

Design Decisions
----------------

The graph is derived only from canonical FV faces. It does not inspect CGAL
types, polygon clipping details, HDF5 packages, or internals. That keeps
the VMM core as the owner of mesh topology while allowing or other
consumers to build their own numerical operators from the same neutral arrays.

Boundary entries retain patch ids rather than closed boundary enums. New
boundary families can therefore be represented by patch metadata without
changing the stencil graph type.

The boundary distance stored in this block is the absolute projection from cell
centre to face centre along the face normal. More advanced projection and
orthogonality anomaly detection is reserved for Block 9.

Exit Criteria
-------------

Block 8 is complete when:

* internal and boundary stencil arrays can be built from a valid canonical mesh;
* internal faces preserve owner/neighbour adjacency and centre distances;
* boundary faces preserve owner cell, patch id, normal, and face length;
* invalid canonical meshes are rejected before graph construction;
* API/status/planning documentation records the new graph layer;
* build, tests, Sphinx, and whitespace checks pass.
