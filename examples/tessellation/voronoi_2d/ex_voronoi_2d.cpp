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
// @file ex_voronoi_2d.cpp
// @brief Builds a rectangle domain, generates hexagonal sites, tessellates, and exports VTK.
// @ingroup vmm_examples
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/export/VTKExporter.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

#ifndef VMM_EXAMPLE_SOURCE_DIR
#define VMM_EXAMPLE_SOURCE_DIR "."
#endif

namespace {

[[nodiscard]] std::string trim(std::string_view text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if(first == std::string_view::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return std::string{text.substr(first, last - first + 1U)};
}

[[nodiscard]] vmm::core::Real parse_real(std::string_view text, std::string_view context)
{
    char* end = nullptr;
    const std::string value{text};
    const auto parsed = std::strtod(value.c_str(), &end);
    if(end == value.c_str() || (end != nullptr && *end != '\0')) {
        vmm::error::throw_parse_error(context, "Invalid real value. / Valor real invalido.");
    }
    return parsed;
}

[[nodiscard]] std::unordered_map<std::string, vmm::core::Real> read_scalar_yaml(
    const std::filesystem::path& file_path,
    std::string_view context)
{
    std::ifstream input{file_path};
    if(!input) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }

    std::unordered_map<std::string, vmm::core::Real> values;
    std::string section;
    std::string line;
    while(std::getline(input, line)) {
        const auto comment = line.find('#');
        if(comment != std::string::npos) {
            line.erase(comment);
        }
        const auto clean = trim(line);
        if(clean.empty()) {
            continue;
        }
        if(clean.back() == ':') {
            section = trim(std::string_view{clean}.substr(0U, clean.size() - 1U));
            continue;
        }
        const auto separator = clean.find(':');
        if(separator == std::string::npos) {
            continue;
        }
        const auto key = trim(std::string_view{clean}.substr(0U, separator));
        const auto value = trim(std::string_view{clean}.substr(separator + 1U));
        values[section + "." + key] = parse_real(value, context);
    }
    return values;
}

[[nodiscard]] vmm::core::Real value_or(const std::unordered_map<std::string, vmm::core::Real>& values,
                                       std::string_view key,
                                       vmm::core::Real fallback)
{
    const auto iterator = values.find(std::string{key});
    return iterator == values.end() ? fallback : iterator->second;
}

} // namespace

int main()
{
    try {
        const std::filesystem::path source_dir{VMM_EXAMPLE_SOURCE_DIR};
        const auto data_file = source_dir / "geometry_data" / "rectangle_voronoi_2d.yaml";
        const auto boundary_file = source_dir / "output_data" / "rectangle_domain.vtk";
        const auto mesh_file = source_dir / "output_data" / "rectangle_voronoi_2d.vtu";
        const auto cells_file = source_dir / "output_data" / "rectangle_voronoi_cells_2d.vtk";

        const auto values = read_scalar_yaml(data_file, "ex_voronoi_2d");
        const vmm::domain::Point2D centre{
            value_or(values, "domain.centre_x", 0.0),
            value_or(values, "domain.centre_y", 0.0)
        };
        const auto domain = vmm::domain::make_rectangle_2d(value_or(values, "domain.width", 8.0),
                                                           value_or(values, "domain.height", 4.0),
                                                           centre);

        const auto sites = vmm::site_generation::make_hexagonal_sites_2d(
            domain,
            value_or(values, "sites.spacing", 0.55));

        const auto result = vmm::tessellation::generate_clipped_voronoi_2d(sites, domain);
        const auto& mesh = result.topology;

        vmm::io::write_legacy_vtk_polygonal_domain_2d(boundary_file, "Rectangle domain", domain);
        vmm::io::write_vtu_cell_centres(mesh_file, mesh);
        vmm::io::write_legacy_vtk_planar_cells_2d(cells_file, "Rectangle clipped Voronoi cells", result.planar_cells);

        std::cout << "Voronoi 2D rectangle example\n";
        std::cout << "data_file: " << data_file << '\n';
        std::cout << "sites: " << sites.size() << '\n';
        std::cout << "cells: " << mesh.cell_count() << '\n';
        std::cout << "neighbour_entries: " << mesh.total_neighbours() << '\n';
        std::cout << "domain_vtk_file: " << boundary_file << '\n';
        std::cout << "mesh_vtu_file: " << mesh_file << '\n';
        std::cout << "cells_vtk_file: " << cells_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_voronoi_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
