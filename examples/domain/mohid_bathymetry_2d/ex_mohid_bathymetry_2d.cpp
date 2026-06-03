//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file ex_mohid_bathymetry_2d.cpp
// @brief Reads MOHID-inspired bathymetry geometry through VMM APIs and exports VTK.
// @ingroup vmm_examples
//==============================================================================

#include <vmm/domain/MohidGeometryReader.hpp>
#include <vmm/export/VTKExporter.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <limits>
#include <vector>

#ifndef VMM_EXAMPLE_SOURCE_DIR
#define VMM_EXAMPLE_SOURCE_DIR "."
#endif

int main()
{
    try {
        const std::filesystem::path source_dir{VMM_EXAMPLE_SOURCE_DIR};
        const auto data_dir = source_dir / "geometry_data";
        const auto boundary_file = data_dir / "bathymetry_boundary.xy";
        const auto island_file = data_dir / "island.xy";
        const auto bathymetry_file = data_dir / "bathymetry_points.xyz";
        const auto output_file = source_dir / "output_data" / "bathymetry_2d.vtk";

        const auto exterior = vmm::domain::read_mohid_polygon_2d(boundary_file, "ex_mohid_bathymetry_2d");
        const auto island = vmm::domain::read_mohid_polygon_2d(island_file, "ex_mohid_bathymetry_2d");
        const auto bathymetry = vmm::domain::read_mohid_bathymetry_points_2d(
            bathymetry_file,
            "ex_mohid_bathymetry_2d");

        const vmm::domain::PolygonalDomain2D domain{exterior, {island}};
        std::vector<vmm::io::ScalarPoint2D> depth_points;
        depth_points.reserve(bathymetry.size());

        vmm::core::Real min_depth = std::numeric_limits<vmm::core::Real>::max();
        vmm::core::Real max_depth = std::numeric_limits<vmm::core::Real>::lowest();
        for(const auto& sample : bathymetry) {
            depth_points.push_back(vmm::io::ScalarPoint2D{sample.point, sample.depth});
            min_depth = std::min(min_depth, sample.depth);
            max_depth = std::max(max_depth, sample.depth);
        }

        vmm::io::write_legacy_vtk_polygonal_domain_2d(output_file,
                                                      "MOHID bathymetry 2D geometry",
                                                      domain,
                                                      depth_points,
                                                      "depth");

        std::cout << "MOHID bathymetry 2D example\n";
        std::cout << "boundary_file: " << boundary_file << '\n';
        std::cout << "island_file: " << island_file << '\n';
        std::cout << "bathymetry_file: " << bathymetry_file << '\n';
        std::cout << "boundary_vertices: " << exterior.vertex_count() << '\n';
        std::cout << "island_vertices: " << island.vertex_count() << '\n';
        std::cout << "approximate_water_area: " << domain.area() << '\n';
        std::cout << "bathymetry_points: " << bathymetry.size() << '\n';
        std::cout << "depth_range: " << min_depth << ' ' << max_depth << '\n';
        std::cout << "vtk_file: " << output_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_mohid_bathymetry_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
