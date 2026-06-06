//==============================================================================
// File        : ex_complete_fv_voronoi_2d.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Generates a complete audited 2D finite-volume Voronoi mesh.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <iostream>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshTextAuditWriter2D.hpp>
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/export/MohidNGHDF5Writer2D.hpp>
#include <vmm/export/VTKExporter.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/workflow/FiniteVolumeVoronoiWorkflow2D.hpp>

#ifndef VMM_EXAMPLE_SOURCE_DIR
#define VMM_EXAMPLE_SOURCE_DIR "."
#endif

int main()
{
    try {
        const std::filesystem::path source_dir{VMM_EXAMPLE_SOURCE_DIR};
        const auto output_dir = source_dir / "output_data";
        const auto domain_file = output_dir / "complete_fv_voronoi_2d_domain.vtk";
        const auto cells_file = output_dir / "complete_fv_voronoi_2d_cells.vtk";
        const auto audit_file = output_dir / "complete_fv_voronoi_2d_audit.txt";
        const auto hdf5_file = output_dir / "complete_fv_voronoi_2d.h5";

        const auto domain = vmm::domain::make_rectangle_2d(4.0, 2.5);
        const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.45);
        const auto result = vmm::workflow::generate_complete_finite_volume_voronoi_mesh_2d(sites, domain);

        vmm::io::write_legacy_vtk_polygonal_domain_2d(domain_file, "Complete FV Voronoi 2D domain", domain);
        vmm::io::write_legacy_vtk_planar_cells_2d(cells_file,
                                                  "Complete FV Voronoi 2D cells",
                                                  result.clipped_voronoi.planar_cells);
        vmm::audit::write_mesh_text_audit_2d(audit_file,
                                             result.finite_volume_mesh,
                                             result.connectivity_geometry_report);
        vmm::io::write_mohidng_hdf5_mesh_2d(hdf5_file, result.finite_volume_mesh);

        std::cout << "Complete FV Voronoi 2D example\n";
        std::cout << "sites: " << sites.size() << '\n';
        std::cout << "cells: " << result.finite_volume_mesh.cell_count() << '\n';
        std::cout << "faces: " << result.finite_volume_mesh.face_count() << '\n';
        std::cout << "patches: " << result.finite_volume_mesh.patch_count() << '\n';
        std::cout << "valid: " << (result.valid() ? "true" : "false") << '\n';
        std::cout << "domain_vtk_file: " << domain_file << '\n';
        std::cout << "cells_vtk_file: " << cells_file << '\n';
        std::cout << "audit_file: " << audit_file << '\n';
        std::cout << "hdf5_file: " << hdf5_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_complete_fv_voronoi_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
