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
// @file PlanarCell2D.hpp
// @brief Rich planar cell representation with exterior ring, holes, and generator metadata.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/domain/Polygon2D.hpp>

#include <span>
#include <utility>
#include <vector>

namespace vmm::domain {

/**
 * @brief Planar polygonal cell with optional interior rings.
 * @ingroup vmm_domain
 *
 * `PlanarCell2D` is the geometry-level representation used after clipped
 * Voronoi construction. Unlike `Polygon2D`, it can carry the metadata needed
 * for mesh extraction: the generator site, generator id, centroid, area, and
 * boundary classification. The current implementation stores holes explicitly,
 * preparing the library for domains with islands or excluded regions.
 */
class PlanarCell2D final {
public:
    PlanarCell2D() = default;

    /**
     * @brief Creates a cell from an exterior ring and generator metadata.
     * @param exterior Exterior polygonal ring.
     * @param generator_site Voronoi generator site associated with the cell.
     * @param generator_id Stable generator identifier.
     * @param boundary_type Boundary classification for the cell.
     */
    PlanarCell2D(Polygon2D exterior,
                 Point2D generator_site,
                 vmm::core::GeneratorID generator_id,
                 vmm::core::BoundaryType boundary_type = vmm::core::BoundaryType::Internal);

    /**
     * @brief Creates a cell from exterior and interior rings.
     */
    PlanarCell2D(Polygon2D exterior,
                 std::vector<Polygon2D> holes,
                 Point2D generator_site,
                 vmm::core::GeneratorID generator_id,
                 vmm::core::BoundaryType boundary_type = vmm::core::BoundaryType::Internal);

    [[nodiscard]] const Polygon2D& exterior() const noexcept;
    [[nodiscard]] std::span<const Polygon2D> holes() const noexcept;
    [[nodiscard]] Point2D generator_site() const noexcept;
    [[nodiscard]] vmm::core::GeneratorID generator_id() const noexcept;
    [[nodiscard]] Point2D centroid() const noexcept;
    [[nodiscard]] vmm::core::Real area() const noexcept;
    [[nodiscard]] vmm::core::BoundaryType boundary_type() const noexcept;
    [[nodiscard]] bool has_holes() const noexcept;

private:
    static Point2D compute_polygon_centroid(const Polygon2D& polygon) noexcept;
    static Point2D compute_centroid(const Polygon2D& exterior, std::span<const Polygon2D> holes) noexcept;
    static vmm::core::Real compute_area(const Polygon2D& exterior, std::span<const Polygon2D> holes) noexcept;

    Polygon2D exterior_{};
    std::vector<Polygon2D> holes_{};
    Point2D generator_site_{};
    vmm::core::GeneratorID generator_id_{};
    Point2D centroid_{};
    vmm::core::Real area_{};
    vmm::core::BoundaryType boundary_type_{vmm::core::BoundaryType::Internal};
};

using VoronoiCell2D = PlanarCell2D;

inline PlanarCell2D::PlanarCell2D(Polygon2D exterior,
                                  Point2D generator_site,
                                  vmm::core::GeneratorID generator_id,
                                  vmm::core::BoundaryType boundary_type)
    : PlanarCell2D(std::move(exterior), {}, generator_site, generator_id, boundary_type)
{
}

inline PlanarCell2D::PlanarCell2D(Polygon2D exterior,
                                  std::vector<Polygon2D> holes,
                                  Point2D generator_site,
                                  vmm::core::GeneratorID generator_id,
                                  vmm::core::BoundaryType boundary_type)
    : exterior_(std::move(exterior))
    , holes_(std::move(holes))
    , generator_site_(generator_site)
    , generator_id_(generator_id)
    , centroid_(compute_centroid(exterior_, holes_))
    , area_(compute_area(exterior_, holes_))
    , boundary_type_(boundary_type)
{
}

inline const Polygon2D& PlanarCell2D::exterior() const noexcept
{
    return exterior_;
}

inline std::span<const Polygon2D> PlanarCell2D::holes() const noexcept
{
    return holes_;
}

inline Point2D PlanarCell2D::generator_site() const noexcept
{
    return generator_site_;
}

inline vmm::core::GeneratorID PlanarCell2D::generator_id() const noexcept
{
    return generator_id_;
}

inline Point2D PlanarCell2D::centroid() const noexcept
{
    return centroid_;
}

inline vmm::core::Real PlanarCell2D::area() const noexcept
{
    return area_;
}

inline vmm::core::BoundaryType PlanarCell2D::boundary_type() const noexcept
{
    return boundary_type_;
}

inline bool PlanarCell2D::has_holes() const noexcept
{
    return !holes_.empty();
}

inline Point2D PlanarCell2D::compute_polygon_centroid(const Polygon2D& polygon) noexcept
{
    const auto vertices = polygon.vertices();
    const std::size_t count = polygon.is_closed() ? vertices.size() - 1U : vertices.size();
    vmm::core::Real signed_area_twice{};
    vmm::core::Real centroid_x{};
    vmm::core::Real centroid_y{};

    for(std::size_t i = 0U; i < count; ++i) {
        const auto a = vertices[i];
        const auto b = vertices[(i + 1U) % count];
        const auto cross = (a.x * b.y) - (b.x * a.y);
        signed_area_twice += cross;
        centroid_x += (a.x + b.x) * cross;
        centroid_y += (a.y + b.y) * cross;
    }

    if(signed_area_twice == vmm::core::Real{}) {
        return vertices.empty() ? Point2D{} : vertices.front();
    }

    const auto scale = static_cast<vmm::core::Real>(1)
        / (static_cast<vmm::core::Real>(3) * signed_area_twice);
    return {centroid_x * scale, centroid_y * scale};
}

inline vmm::core::Real PlanarCell2D::compute_area(
    const Polygon2D& exterior,
    std::span<const Polygon2D> holes) noexcept
{
    auto result = exterior.area();
    for(const auto& hole : holes) {
        result -= hole.area();
    }
    return result;
}

inline Point2D PlanarCell2D::compute_centroid(
    const Polygon2D& exterior,
    std::span<const Polygon2D> holes) noexcept
{
    const auto exterior_area = exterior.area();
    auto weighted_x = compute_polygon_centroid(exterior).x * exterior_area;
    auto weighted_y = compute_polygon_centroid(exterior).y * exterior_area;
    auto total_area = exterior_area;

    for(const auto& hole : holes) {
        const auto hole_area = hole.area();
        const auto hole_centroid = compute_polygon_centroid(hole);
        weighted_x -= hole_centroid.x * hole_area;
        weighted_y -= hole_centroid.y * hole_area;
        total_area -= hole_area;
    }

    if(total_area == vmm::core::Real{}) {
        return compute_polygon_centroid(exterior);
    }

    return {weighted_x / total_area, weighted_y / total_area};
}

} // namespace vmm::domain
