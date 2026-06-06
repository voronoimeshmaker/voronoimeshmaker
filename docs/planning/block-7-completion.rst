Block 7 Completion: MohidNG HDF5 Writer 2D
==========================================

Purpose
-------

Block 7 turns the canonical 2D finite-volume mesh into the physical package
consumed by MohidNG. The writer follows the current MohidNG reader contract
rather than inventing a parallel format inside VMM.

Implemented Artifacts
---------------------

* ``vmm::io::write_mohidng_hdf5_mesh_2d`` writes HDF5 packages with the schema
  identifier ``mohidng.voronoi_mesh_package/0.1``.
* Root attributes are written for schema version, producer, dimension,
  coordinate reference system, and note.
* The package contains ``/nodes``, ``/cells``, ``/faces``, and
  ``/boundary_patches`` groups with the dataset names, shapes, and primitive
  types expected by the MohidNG reader.
* Export is audit-gated by default. The writer refuses invalid canonical FV
  meshes before creating the package.
* HDF5 is linked as an implementation dependency. The public VMM writer header
  does not expose HDF5 handles or C headers.

Schema Summary
--------------

The current 2D package contains:

* ``/nodes/id`` and ``/nodes/xy``;
* ``/cells/id``, ``/cells/centre``, and ``/cells/area``;
* ``/faces/id``, ``/faces/owner``, ``/faces/neighbour``, ``/faces/centre``,
  ``/faces/unit_normal``, ``/faces/length``, and ``/faces/patch_id``;
* ``/boundary_patches/id``, ``/boundary_patches/name``, and
  ``/boundary_patches/type``.

Design Decisions
----------------

The writer consumes ``FiniteVolumeMesh2D`` directly. It does not reconstruct
connectivity, infer patches, or modify geometry. That keeps VMM's export layer a
deterministic serialization step after the mesh has already passed the canonical
audits.

The HDF5 schema uses row indices as contiguous ids for nodes, cells, and faces,
because the canonical DOD tables store those entities by row. Internal faces use
``-1`` for neighbour-invalid and patch-invalid sentinels, matching the MohidNG
reader.

Exit Criteria
-------------

Block 7 is complete when:

* CMake locates and links HDF5;
* a valid canonical 2D FV mesh writes a MohidNG-compatible HDF5 package;
* a schema-focused test verifies root attributes, dataset shapes, and key
  values;
* invalid meshes are rejected before export;
* API/status/planning documentation records the HDF5 writer layer;
* build, tests, Sphinx, and whitespace checks pass.
