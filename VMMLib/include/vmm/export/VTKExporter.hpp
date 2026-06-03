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
// @file VTKExporter.hpp
// @brief ASCII VTK exporters for mesh topology and planar geometry.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/MeshTopology.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <span>
#include <string_view>
#include <vector>

namespace vmm::io {

struct ScalarPoint2D final {
    vmm::domain::Point2D point{};
    vmm::core::Real value{};
};

namespace detail {

template<vmm::core::Dimension Dim>
void write_vtu_point_coordinates(std::ostream& output, std::span<const vmm::core::Point<Dim>> points)
{
    output << std::setprecision(17);
    for(const auto& point : points) {
        if constexpr(Dim == vmm::core::Dimension::D2) {
            output << "          " << point.x << ' ' << point.y << " 0\n";
        } else {
            output << "          " << point.x << ' ' << point.y << ' ' << point.z << "\n";
        }
    }
}

inline std::size_t visible_vertex_count(const vmm::domain::Polygon2D& polygon)
{
    return polygon.is_closed() ? polygon.vertex_count() - 1U : polygon.vertex_count();
}

} // namespace detail

inline void write_legacy_vtk_polygons_2d(const std::filesystem::path& file_path,
                                         std::string_view title,
                                         std::span<const vmm::domain::Polygon2D> polygons,
                                         std::span<const ScalarPoint2D> scalar_points = {},
                                         std::string_view scalar_name = "value")
{
    std::filesystem::create_directories(file_path.parent_path());
    std::ofstream output{file_path};
    if(!output) {
        vmm::error::throw_file_not_found("write_legacy_vtk_polygons_2d", file_path.string());
    }

    std::size_t point_count{};
    std::size_t line_storage_count{};
    for(const auto& polygon : polygons) {
        const auto count = detail::visible_vertex_count(polygon);
        point_count += count;
        line_storage_count += count + 2U;
    }
    point_count += scalar_points.size();

    output << "# vtk DataFile Version 3.0\n";
    output << title << "\n";
    output << "ASCII\n";
    output << "DATASET POLYDATA\n";
    output << "POINTS " << point_count << " double\n";
    output << std::setprecision(17);

    for(const auto& polygon : polygons) {
        const auto count = detail::visible_vertex_count(polygon);
        const auto vertices = polygon.vertices();
        for(std::size_t i = 0; i < count; ++i) {
            output << vertices[i].x << ' ' << vertices[i].y << " 0\n";
        }
    }
    for(const auto& scalar_point : scalar_points) {
        output << scalar_point.point.x << ' ' << scalar_point.point.y << ' ' << scalar_point.value << "\n";
    }

    if(!scalar_points.empty()) {
        output << "VERTICES " << scalar_points.size() << ' ' << (scalar_points.size() * 2U) << "\n";
        const auto scalar_offset = point_count - scalar_points.size();
        for(std::size_t i = 0; i < scalar_points.size(); ++i) {
            output << "1 " << (scalar_offset + i) << "\n";
        }
    }

    output << "LINES " << polygons.size() << ' ' << line_storage_count << "\n";
    std::size_t offset{};
    for(const auto& polygon : polygons) {
        const auto count = detail::visible_vertex_count(polygon);
        output << (count + 1U);
        for(std::size_t i = 0; i < count; ++i) {
            output << ' ' << (offset + i);
        }
        output << ' ' << offset << "\n";
        offset += count;
    }

    if(!scalar_points.empty()) {
        output << "POINT_DATA " << point_count << "\n";
        output << "SCALARS " << scalar_name << " double 1\n";
        output << "LOOKUP_TABLE default\n";
        for(std::size_t i = 0; i < point_count - scalar_points.size(); ++i) {
            output << "-999\n";
        }
        for(const auto& scalar_point : scalar_points) {
            output << scalar_point.value << "\n";
        }
    }
}

inline void write_legacy_vtk_polygonal_domain_2d(
    const std::filesystem::path& file_path,
    std::string_view title,
    const vmm::domain::PolygonalDomain2D& domain,
    std::span<const ScalarPoint2D> scalar_points = {},
    std::string_view scalar_name = "value")
{
    std::vector<vmm::domain::Polygon2D> polygons;
    polygons.reserve(domain.holes().size() + 1U);
    polygons.push_back(domain.exterior());
    for(const auto& hole : domain.holes()) {
        polygons.push_back(hole);
    }
    write_legacy_vtk_polygons_2d(file_path, title, polygons, scalar_points, scalar_name);
}

template<vmm::core::Dimension Dim>
void write_vtu_cell_centres(const std::filesystem::path& file_path,
                            const vmm::core::MeshTopology<Dim>& topology)
{
    std::ofstream output{file_path};
    if(!output) {
        vmm::error::throw_file_not_found("write_vtu_cell_centres", file_path.string());
    }

    const auto centroids = topology.centroids();
    const auto generator_ids = topology.generator_ids();
    const auto cell_count = centroids.size();

    output << "<?xml version=\"1.0\"?>\n";
    output << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    output << "  <UnstructuredGrid>\n";
    output << "    <Piece NumberOfPoints=\"" << cell_count << "\" NumberOfCells=\"" << cell_count << "\">\n";
    output << "      <Points>\n";
    output << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    detail::write_vtu_point_coordinates(output, centroids);
    output << "        </DataArray>\n";
    output << "      </Points>\n";
    output << "      <Cells>\n";
    output << "        <DataArray type=\"UInt32\" Name=\"connectivity\" format=\"ascii\">\n";
    output << "         ";
    for(std::size_t i = 0; i < cell_count; ++i) {
        output << ' ' << i;
    }
    output << "\n        </DataArray>\n";
    output << "        <DataArray type=\"UInt32\" Name=\"offsets\" format=\"ascii\">\n";
    output << "         ";
    for(std::size_t i = 0; i < cell_count; ++i) {
        output << ' ' << (i + 1U);
    }
    output << "\n        </DataArray>\n";
    output << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
    output << "         ";
    for(std::size_t i = 0; i < cell_count; ++i) {
        output << " 1";
    }
    output << "\n        </DataArray>\n";
    output << "      </Cells>\n";
    output << "      <CellData Scalars=\"generator_id\">\n";
    output << "        <DataArray type=\"UInt64\" Name=\"generator_id\" format=\"ascii\">\n";
    output << "         ";
    for(const auto id : generator_ids) {
        output << ' ' << id;
    }
    output << "\n        </DataArray>\n";
    output << "        <DataArray type=\"UInt8\" Name=\"boundary_type\" format=\"ascii\">\n";
    output << "         ";
    for(std::size_t i = 0; i < cell_count; ++i) {
        const auto cell = static_cast<vmm::core::CellIndex>(i);
        output << ' ' << static_cast<unsigned>(topology.boundary_type(cell));
    }
    output << "\n        </DataArray>\n";
    output << "      </CellData>\n";
    output << "    </Piece>\n";
    output << "  </UnstructuredGrid>\n";
    output << "</VTKFile>\n";
}

} // namespace vmm::io
