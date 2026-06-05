Text Audit Format
=================

Purpose
-------

The 2D text audit is a deterministic diagnostic format for canonical
``FiniteVolumeMesh2D`` meshes. It is intended for human inspection, regression
tests, and debugging invalid mesh packages before binary export.

The writer does not replace the in-memory audit reports. It serialises a mesh
and a ``MeshAuditReport`` in a stable order.

Writers
-------

``vmm::audit::MeshTextAuditWriter2D``
   Writes a mesh and a supplied report to ``std::ostream`` or to a filesystem
   path.

``vmm::audit::write_mesh_text_audit_2d``
   Convenience overloads. Overloads without an explicit report run
   ``audit_mesh_connectivity_geometry_2d`` before writing.

Section order
-------------

The format always writes sections in this order:

* header;
* ``[counts]``;
* ``[issues]``;
* ``[nodes]``;
* ``[cells]``;
* ``[faces]``;
* ``[patches]``;
* ``[cell_faces]``.

Sentinels
---------

Invalid entity identifiers are written as ``invalid`` in mesh tables and as
``-`` in issue context columns.

Example
-------

.. code-block:: text

   VMM Mesh Audit 2D
   version: 1
   valid: false

   [counts]
   nodes: 6
   cells: 2
   faces: 7
   patches: 2
   cell_face_entries: 8

   [issues]
   severity code cell face node site patch message
   fatal VMM-GEO-007 0 0 - - - Face normal does not point outward from owner
