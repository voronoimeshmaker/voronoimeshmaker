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

``vmm::tessellation::relax_sites_lloyd_2d(sites, domain, options)``
   Runs a controlled Lloyd relaxation sequence for centroidal Voronoi
   tessellations. ``iteration_count = 0`` preserves the original ``L0`` sites.
   Positive iteration counts repeatedly generate clipped Voronoi cells and move
   each generator to its cell centroid. The result stores the relaxed sites, the
   final clipped Voronoi mesh, and per-iteration mean and maximum generator
   displacement.

Result types
------------

``vmm::tessellation::ClippedVoronoiMesh2D``
   Holds ``topology``, rich ``planar_cells``, compact exterior ``cells``, and ``areas``.

``vmm::tessellation::LloydRelaxation2DResult``
   Holds the relaxed generator sites, final clipped Voronoi mesh, convergence
   flag, and one displacement report per Lloyd iteration.

Current limitation
------------------

The clipped-cell result currently represents each cell as one polygonal ring.
Domains with holes need a richer cell type with exterior and interior rings.
