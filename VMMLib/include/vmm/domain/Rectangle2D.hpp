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
// @file Rectangle2D.hpp
// @brief Rectangular 2D domain factory geometry.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>

#include <vector>

namespace vmm::domain {

class Rectangle2D final {
public:
    Rectangle2D(vmm::core::Real width, vmm::core::Real height, Point2D centre = {});

    [[nodiscard]] vmm::core::Real width() const noexcept { return width_; }
    [[nodiscard]] vmm::core::Real height() const noexcept { return height_; }
    [[nodiscard]] Point2D centre() const noexcept { return centre_; }
    [[nodiscard]] Polygon2D to_polygon() const;
    [[nodiscard]] PolygonalDomain2D to_domain() const;

private:
    vmm::core::Real width_{};
    vmm::core::Real height_{};
    Point2D centre_{};
};

inline Rectangle2D::Rectangle2D(vmm::core::Real width, vmm::core::Real height, Point2D centre)
    : width_(width),
      height_(height),
      centre_(centre)
{
    if(width_ <= vmm::core::Real{} || height_ <= vmm::core::Real{}) {
        vmm::error::throw_invalid_argument("Rectangle2D",
                                           "Rectangle width and height must be positive. / A largura e a altura do retangulo devem ser positivas.");
    }
}

inline Polygon2D Rectangle2D::to_polygon() const
{
    const auto half_width = width_ / static_cast<vmm::core::Real>(2);
    const auto half_height = height_ / static_cast<vmm::core::Real>(2);
    return Polygon2D{std::vector<Point2D>{
        {centre_.x - half_width, centre_.y - half_height},
        {centre_.x + half_width, centre_.y - half_height},
        {centre_.x + half_width, centre_.y + half_height},
        {centre_.x - half_width, centre_.y + half_height}
    }};
}

inline PolygonalDomain2D Rectangle2D::to_domain() const
{
    return PolygonalDomain2D{to_polygon()};
}

[[nodiscard]] inline PolygonalDomain2D make_rectangle_2d(vmm::core::Real width,
                                                         vmm::core::Real height,
                                                         Point2D centre = {})
{
    return Rectangle2D{width, height, centre}.to_domain();
}

} // namespace vmm::domain
