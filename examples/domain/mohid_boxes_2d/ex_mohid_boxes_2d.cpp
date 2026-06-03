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
// @file ex_mohid_boxes_2d.cpp
// @brief Reads MOHID-style boxes through VMM domain APIs and exports VTK.
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
        const auto data_file = source_dir / "geometry_data" / "boxes_2d.dat";
        const auto output_file = source_dir / "output_data" / "boxes_2d.vtk";

        const auto boxes = vmm::domain::read_mohid_box_polygons_2d(data_file, "ex_mohid_boxes_2d");
        vmm::io::write_legacy_vtk_polygons_2d(output_file, "MOHID boxes 2D geometry", boxes);

        vmm::core::Real total_area{};
        for(const auto& box : boxes) {
            total_area += box.area();
        }

        std::cout << "MOHID boxes 2D example\n";
        std::cout << "data_file: " << data_file << '\n';
        std::cout << "boxes: " << boxes.size() << '\n';
        std::cout << "total_area: " << total_area << '\n';
        std::cout << "vtk_file: " << output_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_mohid_boxes_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
