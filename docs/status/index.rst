Implementation Status
=====================

Implemented now
---------------

* 2D rectangle, triangle, ellipse, polygon, and polygonal domain.
* 2D translation and rotation.
* Open domain factory.
* Hexagonal 2D site pattern.
* Open 2D site factory.
* 2D clipped Voronoi cells for exterior polygonal domains.
* Rich 2D planar cell representation with optional holes and generator metadata.
* VTK export for domains, Voronoi cell polygons, and cell centres.
* Canonical 2D finite-volume DOD tables for nodes, cells, faces, patches, and cell-face connectivity.
* Strong IDs and invalid sentinels for canonical FV mesh entities.
* Index audit for canonical 2D FV meshes.
* Connectivity and basic geometry audit for canonical 2D FV meshes.
* Text audit writer for canonical 2D FV mesh diagnostics.
* Face reconstruction from planar 2D cells into canonical FV tables.
* Structured error layer with bilingual message direction.

Planned next
------------

* Voronoi cells with holes.
* 3D domain geometry implementation.
* More site patterns.
* Automatic API extraction with Doxygen/Breathe once the public headers settle.
