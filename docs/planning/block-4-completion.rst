Block 4 Completion
==================

Purpose
-------

Block 4 adds deterministic text diagnostics for canonical 2D finite-volume
meshes. It builds on Blocks 2 and 3: the writer serialises a mesh and an audit
report, and convenience overloads can run the combined connectivity/geometry
audit before writing.

Implemented artefacts
---------------------

Public writer
   ``vmm::audit::MeshTextAuditWriter2D``.

Convenience API
   ``vmm::audit::write_mesh_text_audit_2d`` overloads for ``std::ostream`` and
   filesystem paths, with or without a precomputed ``MeshAuditReport``.

Tests
   ``tests/audit/tst_mesh_text_audit_writer_2d.cpp``.

Format contract
---------------

The text output is intentionally simple and stable. It writes:

* a header with format version and validity;
* mesh counts;
* audit issues;
* node table;
* cell table;
* face table;
* boundary patch table;
* cell-face connectivity table.

Exit criteria
-------------

Block 4 is complete when:

* valid meshes write all sections in deterministic order;
* invalid meshes include the expected audit issues;
* invalid sentinels are readable in the output;
* path-based and stream-based writers are covered by tests;
* the documentation build succeeds.

Deferred work
-------------

Binary HDF5 export remains a later block. The text writer is a
diagnostic gate, not the production package format.
