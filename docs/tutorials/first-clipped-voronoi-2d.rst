First Clipped Voronoi Mesh in 2D
================================

The smallest complete workflow creates a rectangular domain, generates
hexagonal sites, computes clipped Voronoi cells, and exports them to VTK.

.. code-block:: cpp

   #include <vmm/domain/Rectangle2D.hpp>
   #include <vmm/export/VTKExporter.hpp>
   #include <vmm/site_generation/HexagonalSitePattern2D.hpp>
   #include <vmm/tessellation/Tessellator2D.hpp>

   int main()
   {
       const auto domain = vmm::domain::make_rectangle_2d(8.0, 4.0);
       const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.55);
       const auto result = vmm::tessellation::generate_clipped_voronoi_2d(sites, domain);

       vmm::io::write_legacy_vtk_polygonal_domain_2d(
           "rectangle_domain.vtk", "Rectangle domain", domain);

       vmm::io::write_legacy_vtk_voronoi_cells_2d(
           "rectangle_voronoi_cells_2d.vtk",
           "Rectangle clipped Voronoi cells",
           result.cells,
           sites);
   }

The repository example is located at
``examples/tessellation/voronoi_2d/ex_voronoi_2d.cpp``.
