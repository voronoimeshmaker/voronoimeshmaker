External Mesh Generator Survey
==============================

Purpose
-------

This page records the external mesh-generator survey requested during Block 2.
It is intentionally architectural: the survey does not add new VMM features.

Relevant patterns
-----------------

* Gmsh, JIGSAW, DistMesh and OceanMesh2D reinforce the value of a size-field
  abstraction for spatially variable resolution.
* CGAL and Triangle reinforce the need for robust geometric predicates and
  backend isolation.
* MMG reinforces remeshing as a controlled plan of local operations or site
  changes rather than direct mutation of final FV tables.
* snappyHexMesh reinforces staged diagnostics and explicit quality gates.
* MOAB, CGNS and meshio reinforce tags, sets, families and format adapters over
  closed enums.

VMM decisions
-------------

* Keep the canonical FV mesh owned by VMM.
* Keep external backends private and optional.
* Prefer traits, tags, IDs and registries for extensible metadata.
* Add size-field design before raster/GIS/remeshing-heavy work.
* Continue the roadmap through index, topology, geometry and text audits before
  export packages.
