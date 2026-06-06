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
* Boundary patch ring assignment for exterior, island, open, inlet, outlet, and no-flux style edges.
* Patch semantic audit for canonical 2D FV meshes.
* MohidNG HDF5 writer for audited canonical 2D FV mesh packages.
* Solver-neutral internal and boundary stencil graph for canonical 2D FV meshes.
* Projection and orthogonality anomaly audit for canonical 2D FV meshes.
* Optional GIS vector-domain API with isolated GDAL/OGR and PROJ backend discovery.
* Raster bathymetry/topography field storage, interpolation, and cell-centre sampling.
* Integrated 2D finite-volume Voronoi workflow for complete audited mesh generation.
* Structured error layer with bilingual message direction.

Planned next
------------

* Automated clipping/import workflows for Voronoi cells with holes.
* Raster-to-export workflows and raster/vector CRS import validation.
* Site edit batches that rerun the integrated 2D workflow after remeshing changes.
* 3D domain geometry implementation.
* More site patterns.
* Automatic API extraction with Doxygen/Breathe once the public headers settle.
