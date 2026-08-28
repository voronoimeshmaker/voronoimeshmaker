Relation To Mohid NG
====================

VoronoiMeshMaker is designed to prepare and check Voronoi finite-volume mesh
packages that can be consumed by downstream modelling tools such as Mohid NG.
The library owns the geometric preparation steps: domain description, site
generation, Voronoi tessellation, finite-volume connectivity, boundary patch
metadata, auditing, and package export.

The relationship is intentionally loose. VMM does not embed Mohid NG solver
logic, equation policies, or runtime configuration. Instead, it produces a
validated mesh representation with explicit nodes, cells, faces, patches,
connectivity, projection diagnostics, optional raster cell fields, and HDF5
package metadata.

This separation keeps VMM useful as an independent mesh generator and diagnostic
library while still supporting Mohid NG-oriented exchange files.
