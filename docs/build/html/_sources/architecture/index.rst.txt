Architecture
============

Core choices
------------

CGAL isolation
   Public headers under ``VMMLib/include/vmm`` must not include CGAL headers.
   CGAL belongs in private backend translation units.

Data-oriented topology
   Hot mesh data is stored in contiguous arrays. Neighbour relations use CSR
   offsets and connectivity arrays.

Open extension points
   Domain and site generation use concepts, traits, and registries. Closed
   enums are avoided for extension points.

Current 2D flow
---------------

.. code-block:: text

   PolygonalDomain2D
       -> site_generation::make_hexagonal_sites_2d
       -> tessellation::generate_clipped_voronoi_2d
       -> ClippedVoronoiMesh2D
       -> VTKExporter
