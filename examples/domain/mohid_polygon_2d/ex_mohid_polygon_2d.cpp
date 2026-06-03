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
// @file ex_mohid_polygon_2d.cpp
// @brief Reads a MOHID-style polygon through VMM domain APIs and exports VTK.
// @ingroup vmm_examples
//==============================================================================

#include <vmm/domain/MohidGeometryReader.hpp>
#include <vmm/export/VTKExporter.hpp>

#include <filesystem>
#include <iostream>

#ifndef VMM_EXAMPLE_SOURCE_DIR
#define VMM_EXAMPLE_SOURCE_DIR "."
#endif

int main()
{
    try {
        const std::filesystem::path source_dir{VMM_EXAMPLE_SOURCE_DIR};
        const auto data_file = source_dir / "geometry_data" / "coast_like_polygon.xy";
        const auto output_file = source_dir / "output_data" / "coast_like_polygon.vtk";

        const auto polygon = vmm::domain::read_mohid_polygon_2d(data_file, "ex_mohid_polygon_2d");
        const vmm::domain::PolygonalDomain2D domain{polygon};
        const auto bounds = domain.bounds();

        vmm::io::write_legacy_vtk_polygonal_domain_2d(output_file, "MOHID polygon 2D geometry", domain);

        std::cout << "MOHID polygon 2D example\n";
        std::cout << "data_file: " << data_file << '\n';
        std::cout << "vertices: " << polygon.vertex_count() << '\n';
        std::cout << "area: " << domain.area() << '\n';
        std::cout << "bounds_min: " << bounds.min.x << ' ' << bounds.min.y << '\n';
        std::cout << "bounds_max: " << bounds.max.x << ' ' << bounds.max.y << '\n';
        std::cout << "vtk_file: " << output_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_mohid_polygon_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
