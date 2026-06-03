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
// @file Triangle2D.hpp
// @brief Triangular 2D domain factory geometry.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>

#include <vector>

namespace vmm::domain {

class Triangle2D final {
public:
    Triangle2D(Point2D a, Point2D b, Point2D c)
        : a_(a), b_(b), c_(c)
    {
    }

    [[nodiscard]] Polygon2D to_polygon() const
    {
        return Polygon2D{std::vector<Point2D>{a_, b_, c_}};
    }

    [[nodiscard]] PolygonalDomain2D to_domain() const
    {
        return PolygonalDomain2D{to_polygon()};
    }

private:
    Point2D a_{};
    Point2D b_{};
    Point2D c_{};
};

[[nodiscard]] inline PolygonalDomain2D make_triangle_2d(Point2D a, Point2D b, Point2D c)
{
    return Triangle2D{a, b, c}.to_domain();
}

} // namespace vmm::domain
