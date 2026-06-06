Block 14 Completion: HDF5 Export for Extruded 3D Meshes
================================================================

Goal
----

Block 14 makes the extruded 3D finite-volume mesh useful outside memory by
adding a mesh-oriented HDF5 package writer for audited 3D meshes.

Implemented scope
-----------------

* Added ``vmm::io::write_mohidng_hdf5_mesh_3d``.
* Added ``HDF5Writer3DOptions`` with coordinate reference system,
  vertical reference, note, and validation controls.
* Added schema marker ``mohidng.voronoi_mesh_package_3d/0.1``.
* Wrote root metadata with producer, dimension, CRS, vertical reference, and
  note.
* Wrote 3D node coordinates as ``/nodes/xyz``.
* Wrote 3D cell centres and volumes.
* Wrote 3D face owners, neighbours, centres, normals, areas, patch ids, and
  open face-role names.
* Wrote boundary patch ids, names, and semantic patch types.
* Wrote cell-face and face-node connectivity arrays so exported meshes can be
  checked without reconstructing topology from geometry.
* Kept HDF5 completely out of the public mesh-table API.

Validation
----------

The writer runs ``audit_extruded_mesh_3d`` by default before creating the HDF5
file. Invalid meshes can still be written only when callers explicitly disable
validation in the options.

Tests
-----

The block adds a dedicated HDF5 round-trip test that:

* exports a two-cell 2D mesh extruded through two vertical layers;
* checks root metadata and schema version;
* checks node, cell, face, patch, and connectivity dataset dimensions;
* checks expected cell volumes;
* checks that lateral, bottom, top, and internal-horizontal face roles are
  present;
* verifies invalid 3D meshes are rejected before export.

Design notes
------------

The 3D writer follows the 2D writer's isolation model: VMM owns the mesh tables
and exposes only VMM types in headers, while HDF5 remains an implementation
detail inside the writer source. Face roles are exported by their open trait
names rather than by a closed enum, matching the project policy for extensible
concepts.
