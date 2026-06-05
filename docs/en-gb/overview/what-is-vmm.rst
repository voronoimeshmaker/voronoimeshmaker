What Is VoronoiMeshMaker?
=========================

VoronoiMeshMaker is a C++20 mesh-preparation library for Voronoi finite-volume
workflows. Its central product is not merely a geometric tessellation: it is an
audited finite-volume mesh with explicit nodes, cells, faces, owner/neighbour
relations, boundary patches and export-ready metadata.

The near-term implementation focuses on 2D canonical finite-volume tables and
audits. GIS input, raster fields, remeshing and 3D extrusion remain planned
blocks built around the same canonical mesh contract.
