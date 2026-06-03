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
// @file PolygonalDomain2D.hpp
// @brief Planar polygonal domain with an exterior ring and optional holes.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/Polygon2D.hpp>

#include <span>
#include <vector>

namespace vmm::domain {

class PolygonalDomain2D final {
public:
    PolygonalDomain2D() = default;
    explicit PolygonalDomain2D(Polygon2D exterior);
    PolygonalDomain2D(Polygon2D exterior, std::vector<Polygon2D> holes);

    [[nodiscard]] const Polygon2D& exterior() const noexcept;
    [[nodiscard]] std::span<const Polygon2D> holes() const noexcept;
    [[nodiscard]] vmm::core::Real area() const noexcept;
    [[nodiscard]] Bounds2D bounds() const;
    [[nodiscard]] PolygonalDomain2D translated(vmm::core::Real dx, vmm::core::Real dy) const;
    [[nodiscard]] PolygonalDomain2D rotated(vmm::core::Real angle_radians, Point2D centre = {}) const;

private:
    Polygon2D exterior_{};
    std::vector<Polygon2D> holes_{};
};

inline PolygonalDomain2D::PolygonalDomain2D(Polygon2D exterior)
    : exterior_(std::move(exterior))
{
}

inline PolygonalDomain2D::PolygonalDomain2D(Polygon2D exterior, std::vector<Polygon2D> holes)
    : exterior_(std::move(exterior)),
      holes_(std::move(holes))
{
}

inline const Polygon2D& PolygonalDomain2D::exterior() const noexcept
{
    return exterior_;
}

inline std::span<const Polygon2D> PolygonalDomain2D::holes() const noexcept
{
    return holes_;
}

inline vmm::core::Real PolygonalDomain2D::area() const noexcept
{
    auto result = exterior_.area();
    for(const auto& hole : holes_) {
        result -= hole.area();
    }
    return result;
}

inline Bounds2D PolygonalDomain2D::bounds() const
{
    return exterior_.bounds();
}

inline PolygonalDomain2D PolygonalDomain2D::translated(vmm::core::Real dx, vmm::core::Real dy) const
{
    std::vector<Polygon2D> moved_holes;
    moved_holes.reserve(holes_.size());
    for(const auto& hole : holes_) {
        moved_holes.push_back(hole.translated(dx, dy));
    }
    return PolygonalDomain2D{exterior_.translated(dx, dy), std::move(moved_holes)};
}

inline PolygonalDomain2D PolygonalDomain2D::rotated(vmm::core::Real angle_radians, Point2D centre) const
{
    std::vector<Polygon2D> moved_holes;
    moved_holes.reserve(holes_.size());
    for(const auto& hole : holes_) {
        moved_holes.push_back(hole.rotated(angle_radians, centre));
    }
    return PolygonalDomain2D{exterior_.rotated(angle_radians, centre), std::move(moved_holes)};
}

} // namespace vmm::domain
