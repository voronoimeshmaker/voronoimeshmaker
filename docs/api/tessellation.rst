Tessellation API
================

Implemented functions
---------------------

``vmm::tessellation::generate_voronoi_2d(sites, domain, params)``
   Builds a 2D topology from generator sites and a polygonal domain. CGAL is
   used internally in the backend, but public headers remain CGAL-free.

``vmm::tessellation::generate_clipped_voronoi_2d(sites, domain, params)``
   Generates clipped Voronoi cells for the exterior domain boundary. The result
   includes topology, cell polygons, and cell areas.

Result types
------------

``vmm::tessellation::ClippedVoronoiMesh2D``
   Holds ``topology``, rich ``planar_cells``, compact exterior ``cells``, and ``areas``.

Current limitation
------------------

The clipped-cell result currently represents each cell as one polygonal ring.
Domains with holes need a richer cell type with exterior and interior rings.
