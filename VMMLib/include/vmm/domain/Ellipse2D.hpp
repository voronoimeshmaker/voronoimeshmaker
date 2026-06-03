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
// @file Ellipse2D.hpp
// @brief Elliptic 2D domain approximated by a polygonal ring.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>

#include <cmath>
#include <numbers>
#include <vector>

namespace vmm::domain {

class Ellipse2D final {
public:
    Ellipse2D(vmm::core::Real radius_x,
              vmm::core::Real radius_y,
              Point2D centre = {},
              std::size_t segments = 96U);

    [[nodiscard]] Polygon2D to_polygon() const;
    [[nodiscard]] PolygonalDomain2D to_domain() const;

private:
    vmm::core::Real radius_x_{};
    vmm::core::Real radius_y_{};
    Point2D centre_{};
    std::size_t segments_{};
};

inline Ellipse2D::Ellipse2D(vmm::core::Real radius_x,
                            vmm::core::Real radius_y,
                            Point2D centre,
                            std::size_t segments)
    : radius_x_(radius_x),
      radius_y_(radius_y),
      centre_(centre),
      segments_(segments)
{
    if(radius_x_ <= vmm::core::Real{} || radius_y_ <= vmm::core::Real{}) {
        vmm::error::throw_invalid_argument("Ellipse2D",
                                           "Ellipse radii must be positive. / Os raios da elipse devem ser positivos.");
    }
    if(segments_ < 8U) {
        vmm::error::throw_invalid_argument("Ellipse2D",
                                           "Ellipse polygonal approximation requires at least eight segments. / A aproximacao poligonal da elipse requer pelo menos oito segmentos.");
    }
}

inline Polygon2D Ellipse2D::to_polygon() const
{
    std::vector<Point2D> vertices;
    vertices.reserve(segments_);
    constexpr auto two = static_cast<vmm::core::Real>(2);
    const auto pi = static_cast<vmm::core::Real>(std::numbers::pi_v<long double>);

    for(std::size_t i = 0; i < segments_; ++i) {
        const auto angle = two * pi * static_cast<vmm::core::Real>(i) / static_cast<vmm::core::Real>(segments_);
        vertices.push_back(Point2D{
            centre_.x + radius_x_ * std::cos(angle),
            centre_.y + radius_y_ * std::sin(angle)
        });
    }
    return Polygon2D{std::move(vertices)};
}

inline PolygonalDomain2D Ellipse2D::to_domain() const
{
    return PolygonalDomain2D{to_polygon()};
}

[[nodiscard]] inline PolygonalDomain2D make_ellipse_2d(vmm::core::Real radius_x,
                                                       vmm::core::Real radius_y,
                                                       Point2D centre = {},
                                                       std::size_t segments = 96U)
{
    return Ellipse2D{radius_x, radius_y, centre, segments}.to_domain();
}

} // namespace vmm::domain
