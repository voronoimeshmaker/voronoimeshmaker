Block 13 Completion
===================

Scope
-----

Block 13 adds the first 3D finite-volume target: vertical extrusion of stable
2D finite-volume meshes. It does not attempt general 3D Voronoi/polyhedral
generation.

Implemented
-----------

* ``vmm::mesh::FiniteVolumeMesh3D`` stores extruded 3D nodes, cells, faces,
  boundary patches, cell-face connectivity, and face-node connectivity.
* ``vmm::mesh::extrude_finite_volume_mesh_3d`` repeats 2D nodes across vertical
  interfaces, creates lateral faces from 2D faces, creates bottom/surface and
  internal horizontal faces, and fills 3D cell volumes and face areas.
* ``vmm::mesh::FaceRole3D`` uses traits for lateral, bottom, top, and internal
  horizontal roles rather than a closed enum.
* ``vmm::audit::audit_extruded_mesh_3d`` validates basic 3D storage, positive
  volumes, positive face areas, owner/neighbour references, patch references,
  and connectivity spans.
* ``vmm::audit::write_mesh_text_audit_3d`` writes deterministic text diagnostics
  for extruded 3D meshes.
* Unit tests cover extrusion counts, volumes, areas, audit validity, invalid
  vertical interfaces, and text audit sections.

Deliberate limits
-----------------

* Only vertical layer extrusion is implemented in this block.
* The 3D mesh is built from an already valid 2D FV mesh.
* General 3D Voronoi generation remains later work.
