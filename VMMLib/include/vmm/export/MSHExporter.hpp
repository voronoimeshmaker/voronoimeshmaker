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
// @file MSHExporter.hpp
// @brief ASCII Gmsh MSH exporter for currently available mesh topology data.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/MeshTopology.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>

namespace vmm::io {

/**
 * @brief Writes cell centres as Gmsh point elements in MSH 2.2 ASCII format.
 * @ingroup vmm_export
 */
template<vmm::core::Dimension Dim>
void write_msh_cell_centres(const std::filesystem::path& file_path,
                            const vmm::core::MeshTopology<Dim>& topology)
{
    std::ofstream output{file_path};
    if(!output) {
        vmm::error::throw_file_not_found("write_msh_cell_centres", file_path.string());
    }

    const auto centroids = topology.centroids();
    output << std::setprecision(17);
    output << "$MeshFormat\n2.2 0 8\n$EndMeshFormat\n";
    output << "$Nodes\n" << centroids.size() << "\n";

    for(std::size_t i = 0; i < centroids.size(); ++i) {
        const auto& point = centroids[i];
        output << (i + 1U) << ' ';
        if constexpr(Dim == vmm::core::Dimension::D2) {
            output << point.x << ' ' << point.y << " 0\n";
        } else {
            output << point.x << ' ' << point.y << ' ' << point.z << "\n";
        }
    }

    output << "$EndNodes\n";
    output << "$Elements\n" << centroids.size() << "\n";
    for(std::size_t i = 0; i < centroids.size(); ++i) {
        output << (i + 1U) << " 15 2 0 0 " << (i + 1U) << "\n";
    }
    output << "$EndElements\n";
}

} // namespace vmm::io
