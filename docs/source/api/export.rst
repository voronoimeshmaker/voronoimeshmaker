Export API
==========

Implemented functions
---------------------

``vmm::io::write_legacy_vtk_polygonal_domain_2d``
   Writes domain boundaries as legacy ASCII VTK ``POLYDATA``.

``vmm::io::write_legacy_vtk_voronoi_cells_2d``
   Writes clipped Voronoi cells as legacy ASCII VTK ``POLYDATA`` polygons.

``vmm::io::write_legacy_vtk_planar_cells_2d``
   Writes rich planar cells and exports generator id, area, boundary type, and centroid fields.

``vmm::io::write_vtu_cell_centres``
   Writes mesh cell centres as VTU vertex cells.

Exporter direction
------------------

The long-term mesh export model should be based on points, faces, owner,
neighbour, and boundary patches. This will align VMM with unstructured mesh
formats such as OpenFOAM ``polyMesh`` while preserving compact CSR-style
connectivity.
