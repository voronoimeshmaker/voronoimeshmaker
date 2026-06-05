Block 3 Completion
==================

Purpose
-------

Block 3 adds the first connectivity and geometry audit above the canonical
finite-volume index audit. It is still based on manual canonical 2D meshes; it
does not reconstruct faces from clipped Voronoi cells yet.

Implemented artefacts
---------------------

Public audit interface
   ``vmm::audit::MeshConnectivityGeometryAudit2D`` and the convenience function
   ``vmm::audit::audit_mesh_connectivity_geometry_2d``.

Implementation
   ``VMMLib/src/audit/MeshConnectivityGeometryAudit2D.cpp``.

Tests
   ``tests/audit/tst_mesh_connectivity_geometry_audit_2d.cpp``.

Audit sequence
--------------

The Block 3 audit first runs ``MeshIndexAudit2D``. If the index audit produces
fatal errors, the connectivity/geometry pass stops immediately. This keeps the
second pass from indexing invalid table entries.

Connectivity checks
-------------------

The audit reports:

* ``VMM-CON-001`` when an owner cell does not reference one of its owned faces;
* ``VMM-CON-002`` when a neighbour cell does not reference an internal face;
* ``VMM-CON-003`` when a cell face list contains a duplicate face;
* ``VMM-CON-004`` when a 2D cell has fewer than three faces;
* ``VMM-CON-005`` when a face occurrence count does not match its state:
  boundary faces should occur once, internal faces should occur twice.

Geometry checks
---------------

The audit reports:

* ``VMM-GEO-001`` for non-finite cell centres;
* ``VMM-GEO-002`` for non-positive or non-finite cell area;
* ``VMM-GEO-003`` for non-finite face centres;
* ``VMM-GEO-004`` for non-positive or non-finite face length;
* ``VMM-GEO-005`` for non-finite face normals;
* ``VMM-GEO-006`` for zero face normals;
* ``VMM-GEO-007`` when the face normal does not point outward from the owner.

Exit criteria
-------------

Block 3 is complete when:

* the new audit API is available from public headers;
* valid manual FV meshes pass the combined index/connectivity/geometry audit;
* intentionally invalid manual meshes cover each ``VMM-CON-*`` and
  ``VMM-GEO-*`` code;
* the focused regression suite passes;
* the documentation build succeeds.

Deferred work
-------------

Face reconstruction from clipped polygonal cells remains Block 5. Stable text
audit output remains Block 4. More advanced geometric quality metrics, such as
orthogonality anomaly reporting, remain Block 9.
