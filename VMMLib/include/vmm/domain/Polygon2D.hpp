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
// @file Polygon2D.hpp
// @brief Polygonal 2D ring used to describe planar domains.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/error/MeshException.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <vector>

namespace vmm::domain {

using Point2D = vmm::core::Point<vmm::core::Dimension::D2>;


namespace detail {

inline bool same_point(Point2D lhs, Point2D rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline vmm::core::Real orientation(Point2D a, Point2D b, Point2D c) noexcept
{
    return ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
}

inline bool point_on_segment(Point2D a, Point2D b, Point2D p) noexcept
{
    return orientation(a, b, p) == vmm::core::Real{}
        && p.x >= std::min(a.x, b.x)
        && p.x <= std::max(a.x, b.x)
        && p.y >= std::min(a.y, b.y)
        && p.y <= std::max(a.y, b.y);
}

inline bool segments_intersect(Point2D a, Point2D b, Point2D c, Point2D d) noexcept
{
    const auto o1 = orientation(a, b, c);
    const auto o2 = orientation(a, b, d);
    const auto o3 = orientation(c, d, a);
    const auto o4 = orientation(c, d, b);

    if(((o1 > vmm::core::Real{} && o2 < vmm::core::Real{})
        || (o1 < vmm::core::Real{} && o2 > vmm::core::Real{}))
       && ((o3 > vmm::core::Real{} && o4 < vmm::core::Real{})
           || (o3 < vmm::core::Real{} && o4 > vmm::core::Real{}))) {
        return true;
    }

    return point_on_segment(a, b, c)
        || point_on_segment(a, b, d)
        || point_on_segment(c, d, a)
        || point_on_segment(c, d, b);
}

} // namespace detail

struct Bounds2D final {
    Point2D min{};
    Point2D max{};
};

class Polygon2D final {
public:
    Polygon2D() = default;
    explicit Polygon2D(std::vector<Point2D> vertices);
    explicit Polygon2D(std::span<const Point2D> vertices);

    [[nodiscard]] std::span<const Point2D> vertices() const noexcept;
    [[nodiscard]] std::size_t vertex_count() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool is_closed() const noexcept;
    [[nodiscard]] vmm::core::Real signed_area() const noexcept;
    [[nodiscard]] vmm::core::Real area() const noexcept;
    [[nodiscard]] Bounds2D bounds() const;
    [[nodiscard]] Polygon2D translated(vmm::core::Real dx, vmm::core::Real dy) const;
    [[nodiscard]] Polygon2D rotated(vmm::core::Real angle_radians, Point2D centre = {}) const;
    void validate_or_throw() const;

private:
    std::vector<Point2D> vertices_;
};

inline Polygon2D::Polygon2D(std::vector<Point2D> vertices)
    : vertices_(std::move(vertices))
{
    validate_or_throw();
}

inline Polygon2D::Polygon2D(std::span<const Point2D> vertices)
    : Polygon2D(std::vector<Point2D>{vertices.begin(), vertices.end()})
{
}

inline std::span<const Point2D> Polygon2D::vertices() const noexcept
{
    return vertices_;
}

inline std::size_t Polygon2D::vertex_count() const noexcept
{
    return vertices_.size();
}

inline bool Polygon2D::empty() const noexcept
{
    return vertices_.empty();
}

inline bool Polygon2D::is_closed() const noexcept
{
    return vertices_.size() > 1U
        && vertices_.front().x == vertices_.back().x
        && vertices_.front().y == vertices_.back().y;
}

inline vmm::core::Real Polygon2D::signed_area() const noexcept
{
    if(vertices_.size() < 3U) {
        return {};
    }

    const std::size_t count = is_closed() ? vertices_.size() - 1U : vertices_.size();
    vmm::core::Real result{};
    for(std::size_t i = 0; i < count; ++i) {
        const auto& a = vertices_[i];
        const auto& b = vertices_[(i + 1U) % count];
        result += (a.x * b.y) - (b.x * a.y);
    }
    return result / static_cast<vmm::core::Real>(2);
}

inline vmm::core::Real Polygon2D::area() const noexcept
{
    return std::abs(signed_area());
}

inline Bounds2D Polygon2D::bounds() const
{
    if(vertices_.empty()) {
        vmm::error::throw_invalid_argument("Polygon2D", "Cannot compute bounds of an empty polygon.");
    }

    Bounds2D result{vertices_.front(), vertices_.front()};
    for(const auto& vertex : vertices_) {
        result.min.x = std::min(result.min.x, vertex.x);
        result.min.y = std::min(result.min.y, vertex.y);
        result.max.x = std::max(result.max.x, vertex.x);
        result.max.y = std::max(result.max.y, vertex.y);
    }
    return result;
}


inline void Polygon2D::validate_or_throw() const
{
    if(vertices_.size() < 3U) {
        vmm::error::throw_error(vmm::error::builtin_error_id::invalid_polygon,
                                "Polygon2D",
                                "A polygon requires at least three vertices. / Um poligono requer pelo menos tres vertices.");
    }

    for(const auto& vertex : vertices_) {
        if(!std::isfinite(vertex.x) || !std::isfinite(vertex.y)) {
            vmm::error::throw_error(vmm::error::builtin_error_id::invalid_polygon,
                                    "Polygon2D",
                                    "Polygon coordinates must be finite. / As coordenadas do poligono devem ser finitas.");
        }
    }

    const std::size_t count = is_closed() ? vertices_.size() - 1U : vertices_.size();
    if(count < 3U) {
        vmm::error::throw_error(vmm::error::builtin_error_id::invalid_polygon,
                                "Polygon2D",
                                "A closed polygon requires at least three unique vertices. / Um poligono fechado requer pelo menos tres vertices unicos.");
    }

    for(std::size_t i = 0; i < count; ++i) {
        const auto& current = vertices_[i];
        const auto& next = vertices_[(i + 1U) % count];
        if(detail::same_point(current, next)) {
            vmm::error::throw_error(vmm::error::builtin_error_id::degenerate_polygon,
                                    "Polygon2D",
                                    "Polygon has repeated consecutive vertices. / O poligono tem vertices consecutivos repetidos.");
        }
    }

    if(signed_area() == vmm::core::Real{}) {
        vmm::error::throw_error(vmm::error::builtin_error_id::degenerate_polygon,
                                "Polygon2D",
                                "Polygon area must be non-zero. / A area do poligono deve ser diferente de zero.");
    }

    for(std::size_t i = 0; i < count; ++i) {
        const std::size_t i_next = (i + 1U) % count;
        for(std::size_t j = i + 1U; j < count; ++j) {
            const std::size_t j_next = (j + 1U) % count;
            const bool adjacent = i_next == j || j_next == i;
            if(adjacent) {
                continue;
            }

            if(detail::segments_intersect(vertices_[i], vertices_[i_next], vertices_[j], vertices_[j_next])) {
                vmm::error::throw_error(vmm::error::builtin_error_id::self_intersecting_polygon,
                                        "Polygon2D",
                                        "Polygon edges must not self-intersect. / As arestas do poligono nao podem ter auto-interseccao.");
            }
        }
    }
}

inline Polygon2D Polygon2D::translated(vmm::core::Real dx, vmm::core::Real dy) const
{
    std::vector<Point2D> moved = vertices_;
    for(auto& vertex : moved) {
        vertex.x += dx;
        vertex.y += dy;
    }
    return Polygon2D{std::move(moved)};
}

inline Polygon2D Polygon2D::rotated(vmm::core::Real angle_radians, Point2D centre) const
{
    const auto cos_angle = std::cos(angle_radians);
    const auto sin_angle = std::sin(angle_radians);
    std::vector<Point2D> moved = vertices_;

    for(auto& vertex : moved) {
        const auto local_x = vertex.x - centre.x;
        const auto local_y = vertex.y - centre.y;
        vertex.x = centre.x + (local_x * cos_angle) - (local_y * sin_angle);
        vertex.y = centre.y + (local_x * sin_angle) + (local_y * cos_angle);
    }
    return Polygon2D{std::move(moved)};
}

} // namespace vmm::domain
