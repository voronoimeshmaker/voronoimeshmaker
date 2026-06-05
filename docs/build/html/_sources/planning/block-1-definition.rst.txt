Block 1 Definition
==================

Name
----

Strong IDs and finite-volume DOD 2D tables.

Objective
---------

Introduce the canonical 2D finite-volume data model required by MohidNG-oriented
export, text audit, and later stencil construction. The block is deliberately
manual-data first: it must not depend on CGAL, GIS, HDF5, or the existing clipped
Voronoi pipeline.

Scope
-----

Block 1 should add:

* strong ID types for cells, faces, nodes, sites, and boundary patches;
* invalid sentinel values and ``is_valid`` helpers;
* no implicit conversion from IDs to integers;
* ``NodeTable2D`` with contiguous ``x`` and ``y`` arrays;
* ``CellTable2D`` with centre coordinates, area, site ID, face offset, and face count arrays;
* ``FaceTable2D`` with owner, neighbour, centre, unit normal, length, patch ID, and endpoint node arrays;
* ``BoundaryPatchTable`` with ID, name, and patch type arrays;
* ``CellFaceConnectivity`` with a flat face-ID array;
* ``FiniteVolumeMesh2D`` as the aggregate canonical mesh type;
* basic count helpers and ID range helpers;
* span/view helpers for cell faces;
* unit tests for IDs, table sizing, range checks, invalid sentinels, and a tiny manual mesh.

Initial file targets
--------------------

Use the repository's current layout:

* ``VMMLib/include/vmm/core/StrongIds.hpp``
* ``VMMLib/include/vmm/mesh/NodeTable2D.hpp``
* ``VMMLib/include/vmm/mesh/CellTable2D.hpp``
* ``VMMLib/include/vmm/mesh/FaceTable2D.hpp``
* ``VMMLib/include/vmm/mesh/BoundaryPatchTable.hpp``
* ``VMMLib/include/vmm/mesh/CellFaceConnectivity.hpp``
* ``VMMLib/include/vmm/mesh/FiniteVolumeMesh2D.hpp``
* ``tests/core/tst_strong_ids.cpp``
* ``tests/mesh/tst_finite_volume_mesh_2d.cpp``

Namespace policy
----------------

The existing project currently places ``MeshTopology`` in ``vmm::core`` even
though the header is under ``vmm/mesh``. For new finite-volume tables, use
``vmm::mesh`` unless implementation review finds a stronger local convention.
The strong IDs should live in ``vmm::core``.

Patch type policy
-----------------

Block 1 should define the patch type enum needed by later blocks:

* ``internal``;
* ``wall``;
* ``island``;
* ``inlet``;
* ``outlet``;
* ``open_boundary``;
* ``land_boundary``;
* ``no_flux``;
* ``bottom``;
* ``surface``.

Only basic storage is required in Block 1. Semantic validation belongs to later
audit blocks.

Explicit exclusions
-------------------

Block 1 must not implement:

* face reconstruction from clipped Voronoi cells;
* CGAL changes;
* GIS input;
* HDF5 export;
* text audit writer;
* solver-neutral stencil graph;
* 3D extrusion;
* remeshing algorithms.

Exit criteria
-------------

Block 1 is complete when:

* the project configures and builds;
* all existing tests still pass;
* new ID and table tests pass;
* a small manual two-cell mesh can be represented in memory using the canonical
  tables;
* public headers added in the block include no CGAL/GDAL/PROJ/HDF5 headers;
* no per-cell object graph or nested per-cell vectors are introduced.

Implementation notes
--------------------

Use simple, explicit C++20 structures first. Avoid abstractions that only become
useful in later blocks. Keep the memory layout obvious and auditable.

This block creates the foundation that later audits will harden. It is acceptable
for invalid meshes to be representable in Block 1, because Block 2 and Block 3
will define the mandatory rejection rules.
