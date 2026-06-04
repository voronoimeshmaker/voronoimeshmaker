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
// @file DelaunayGenerator.cpp
// @brief Private CGAL-backed Delaunay and clipped Voronoi extraction for 2D generation.
// @ingroup vmm_backend_cgal_2d
//==============================================================================

#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <set>
#include <span>
#include <vector>

namespace vmm::tessellation {
namespace {

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using VertexBase = CGAL::Triangulation_vertex_base_with_info_2<std::size_t, Kernel>;
using FaceBase = CGAL::Triangulation_face_base_2<Kernel>;
using TriangulationDataStructure = CGAL::Triangulation_data_structure_2<VertexBase, FaceBase>;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel, TriangulationDataStructure>;
using CgalPoint = Kernel::Point_2;

[[nodiscard]] vmm::core::Real squared_norm(vmm::domain::Point2D point) noexcept
{
    return (point.x * point.x) + (point.y * point.y);
}

[[nodiscard]] bool is_inside_bisector_half_plane(vmm::domain::Point2D point,
                                                 vmm::domain::Point2D site,
                                                 vmm::domain::Point2D other) noexcept
{
    const auto a = other.x - site.x;
    const auto b = other.y - site.y;
    const auto c = (squared_norm(other) - squared_norm(site)) / static_cast<vmm::core::Real>(2);
    return ((a * point.x) + (b * point.y)) <= c + static_cast<vmm::core::Real>(1.0e-12);
}

[[nodiscard]] vmm::domain::Point2D bisector_intersection(vmm::domain::Point2D first,
                                                         vmm::domain::Point2D second,
                                                         vmm::domain::Point2D site,
                                                         vmm::domain::Point2D other) noexcept
{
    const auto a = other.x - site.x;
    const auto b = other.y - site.y;
    const auto c = (squared_norm(other) - squared_norm(site)) / static_cast<vmm::core::Real>(2);
    const auto first_value = (a * first.x) + (b * first.y) - c;
    const auto second_value = (a * second.x) + (b * second.y) - c;
    const auto denominator = first_value - second_value;

    if(std::abs(denominator) <= static_cast<vmm::core::Real>(1.0e-15)) {
        return first;
    }

    const auto t = first_value / denominator;
    return {
        first.x + (t * (second.x - first.x)),
        first.y + (t * (second.y - first.y))
    };
}

[[nodiscard]] std::vector<vmm::domain::Point2D> clip_by_bisector(
    std::span<const vmm::domain::Point2D> polygon,
    vmm::domain::Point2D site,
    vmm::domain::Point2D other)
{
    std::vector<vmm::domain::Point2D> clipped;
    if(polygon.empty()) {
        return clipped;
    }

    auto previous = polygon.back();
    auto previous_inside = is_inside_bisector_half_plane(previous, site, other);
    for(const auto current : polygon) {
        const auto current_inside = is_inside_bisector_half_plane(current, site, other);

        if(current_inside) {
            if(!previous_inside) {
                clipped.push_back(bisector_intersection(previous, current, site, other));
            }
            clipped.push_back(current);
        } else if(previous_inside) {
            clipped.push_back(bisector_intersection(previous, current, site, other));
        }

        previous = current;
        previous_inside = current_inside;
    }

    return clipped;
}

[[nodiscard]] std::vector<vmm::domain::Point2D> open_vertices(const vmm::domain::Polygon2D& polygon)
{
    const auto vertices = polygon.vertices();
    const auto count = polygon.is_closed() ? vertices.size() - 1U : vertices.size();
    return {vertices.begin(), vertices.begin() + static_cast<std::ptrdiff_t>(count)};
}

[[nodiscard]] vmm::domain::Polygon2D clipped_cell_polygon(std::span<const vmm::domain::Point2D> sites,
                                                          std::size_t site_index,
                                                          const vmm::domain::PolygonalDomain2D& domain)
{
    auto cell = open_vertices(domain.exterior());
    const auto site = sites[site_index];

    for(std::size_t other_index = 0U; other_index < sites.size(); ++other_index) {
        if(other_index == site_index) {
            continue;
        }
        cell = clip_by_bisector(std::span<const vmm::domain::Point2D>{cell.data(), cell.size()}, site, sites[other_index]);
        if(cell.size() < 3U) {
            break;
        }
    }

    if(cell.size() < 3U) {
        vmm::error::throw_error(vmm::error::builtin_error_id::degenerate_polygon,
                                "generate_clipped_voronoi_2d",
                                "A clipped Voronoi cell became degenerate. / Uma celula de Voronoi recortada ficou degenerada.");
    }

    return vmm::domain::Polygon2D{std::move(cell)};
}

[[nodiscard]] bool is_boundary_site(const vmm::domain::PolygonalDomain2D& domain,
                                    vmm::domain::Point2D point,
                                    vmm::core::Real spacing_hint)
{
    const auto bounds = domain.bounds();
    const auto tolerance = std::max(spacing_hint, static_cast<vmm::core::Real>(1.0e-9));
    return point.x <= bounds.min.x + tolerance
        || point.x >= bounds.max.x - tolerance
        || point.y <= bounds.min.y + tolerance
        || point.y >= bounds.max.y - tolerance;
}

[[nodiscard]] vmm::core::Real spacing_hint(std::span<const vmm::domain::Point2D> sites)
{
    if(sites.size() < 2U) {
        return static_cast<vmm::core::Real>(1);
    }

    auto best = std::numeric_limits<vmm::core::Real>::max();
    for(std::size_t i = 1U; i < sites.size(); ++i) {
        const auto dx = sites[i].x - sites[i - 1U].x;
        const auto dy = sites[i].y - sites[i - 1U].y;
        const auto distance = std::sqrt((dx * dx) + (dy * dy));
        if(distance > vmm::core::Real{}) {
            best = std::min(best, distance);
        }
    }
    return best == std::numeric_limits<vmm::core::Real>::max()
        ? static_cast<vmm::core::Real>(1)
        : best;
}

[[nodiscard]] std::vector<std::vector<vmm::core::CellIndex>> delaunay_adjacency(
    std::span<const vmm::domain::Point2D> sites)
{
    std::vector<std::pair<CgalPoint, std::size_t>> cgal_sites;
    cgal_sites.reserve(sites.size());
    for(std::size_t i = 0U; i < sites.size(); ++i) {
        cgal_sites.emplace_back(CgalPoint{sites[i].x, sites[i].y}, i);
    }

    Delaunay triangulation;
    triangulation.insert(cgal_sites.begin(), cgal_sites.end());

    std::vector<std::set<vmm::core::CellIndex>> adjacency_sets(sites.size());
    for(auto edge = triangulation.finite_edges_begin(); edge != triangulation.finite_edges_end(); ++edge) {
        const auto face = edge->first;
        const int opposite = edge->second;
        const auto first = face->vertex((opposite + 1) % 3)->info();
        const auto second = face->vertex((opposite + 2) % 3)->info();
        if(first != second) {
            adjacency_sets[first].insert(static_cast<vmm::core::CellIndex>(second));
            adjacency_sets[second].insert(static_cast<vmm::core::CellIndex>(first));
        }
    }

    std::vector<std::vector<vmm::core::CellIndex>> neighbours;
    neighbours.reserve(adjacency_sets.size());
    for(const auto& set : adjacency_sets) {
        neighbours.emplace_back(set.begin(), set.end());
    }
    return neighbours;
}

void validate_sites(std::span<const vmm::domain::Point2D> sites,
                    const vmm::domain::PolygonalDomain2D& domain,
                    std::string_view context)
{
    if(sites.empty()) {
        vmm::error::throw_invalid_argument(
            context,
            "At least one generator site is required. / E necessario ao menos um ponto gerador.");
    }

    for(const auto site : sites) {
        if(!vmm::site_generation::detail::point_in_domain_2d(domain, site)) {
            vmm::error::throw_invalid_argument(
                context,
                "Generator sites must lie inside the domain. / Os pontos geradores devem estar dentro do dominio.");
        }
    }
}

} // namespace

vmm::core::MeshTopology<vmm::core::Dimension::D2>
generate_voronoi_2d(std::span<const vmm::domain::Point2D> sites,
                    const vmm::domain::PolygonalDomain2D& domain,
                    const GenerationParams2D&)
{
    validate_sites(sites, domain, "generate_voronoi_2d");
    const auto neighbours = delaunay_adjacency(sites);

    vmm::core::MeshTopology<vmm::core::Dimension::D2> topology{sites.size()};
    const auto hint = spacing_hint(sites) * static_cast<vmm::core::Real>(0.75);
    for(std::size_t i = 0U; i < sites.size(); ++i) {
        const auto cell = static_cast<vmm::core::CellIndex>(i);
        topology.set_centroid(cell, sites[i]);
        topology.assign_generator_id(cell, static_cast<vmm::core::GeneratorID>(i));
        topology.set_boundary_flag(cell,
                                   is_boundary_site(domain, sites[i], hint)
                                       ? vmm::core::BoundaryType::ExternalBoundary
                                       : vmm::core::BoundaryType::Internal);
    }
    topology.set_neighbours(neighbours);
    return topology;
}

ClippedVoronoiMesh2D
generate_clipped_voronoi_2d(std::span<const vmm::domain::Point2D> sites,
                            const vmm::domain::PolygonalDomain2D& domain,
                            const GenerationParams2D& params)
{
    validate_sites(sites, domain, "generate_clipped_voronoi_2d");

    std::vector<vmm::domain::PlanarCell2D> planar_cells;
    std::vector<vmm::domain::Polygon2D> cells;
    std::vector<vmm::core::Real> areas;
    planar_cells.reserve(sites.size());
    cells.reserve(sites.size());
    areas.reserve(sites.size());

    auto topology = generate_voronoi_2d(sites, domain, params);
    for(std::size_t i = 0U; i < sites.size(); ++i) {
        auto polygon = clipped_cell_polygon(sites, i, domain);
        const auto cell_index = static_cast<vmm::core::CellIndex>(i);
        vmm::domain::PlanarCell2D planar_cell{
            polygon,
            sites[i],
            static_cast<vmm::core::GeneratorID>(i),
            topology.boundary_type(cell_index)
        };

        topology.set_centroid(cell_index, planar_cell.centroid());
        areas.push_back(planar_cell.area());
        cells.push_back(std::move(polygon));
        planar_cells.push_back(std::move(planar_cell));
    }

    return {std::move(topology), std::move(planar_cells), std::move(cells), std::move(areas)};
}

vmm::core::MeshTopology<vmm::core::Dimension::D2>
generate_voronoi_2d(
    const std::vector<vmm::core::Point<vmm::core::Dimension::D2>>& sites,
    const std::vector<vmm::core::Point<vmm::core::Dimension::D2>>& boundary,
    const std::vector<std::vector<vmm::core::Point<vmm::core::Dimension::D2>>>& holes,
    const GenerationParams2D& params)
{
    std::vector<vmm::domain::Polygon2D> hole_polygons;
    hole_polygons.reserve(holes.size());
    for(const auto& hole : holes) {
        hole_polygons.emplace_back(hole);
    }
    const vmm::domain::PolygonalDomain2D domain{vmm::domain::Polygon2D{boundary}, std::move(hole_polygons)};
    return generate_voronoi_2d(std::span<const vmm::domain::Point2D>{sites.data(), sites.size()}, domain, params);
}

} // namespace vmm::tessellation
