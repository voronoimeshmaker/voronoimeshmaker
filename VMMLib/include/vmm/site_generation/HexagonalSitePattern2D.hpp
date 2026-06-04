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
// @file HexagonalSitePattern2D.hpp
// @brief Hexagonal point generator site pattern for polygonal 2D domains.
// @ingroup vmm_site_generation
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/SitePatternTraits.hpp>

#include <cmath>
#include <cstddef>
#include <vector>

namespace vmm::site_generation {

namespace detail {

inline bool point_on_segment_2d(vmm::domain::Point2D a,
                                vmm::domain::Point2D b,
                                vmm::domain::Point2D point) noexcept
{
    const auto cross = ((b.x - a.x) * (point.y - a.y)) - ((b.y - a.y) * (point.x - a.x));
    if(std::abs(cross) > static_cast<vmm::core::Real>(1.0e-12)) {
        return false;
    }

    return point.x >= std::min(a.x, b.x)
        && point.x <= std::max(a.x, b.x)
        && point.y >= std::min(a.y, b.y)
        && point.y <= std::max(a.y, b.y);
}

inline bool point_in_ring_2d(const vmm::domain::Polygon2D& polygon,
                             vmm::domain::Point2D point) noexcept
{
    const auto vertices = polygon.vertices();
    const std::size_t count = polygon.is_closed() ? vertices.size() - 1U : vertices.size();
    bool inside = false;

    for(std::size_t i = 0U, j = count - 1U; i < count; j = i++) {
        const auto a = vertices[i];
        const auto b = vertices[j];
        if(point_on_segment_2d(a, b, point)) {
            return true;
        }

        const bool crosses = ((a.y > point.y) != (b.y > point.y))
            && (point.x < (((b.x - a.x) * (point.y - a.y)) / (b.y - a.y)) + a.x);
        if(crosses) {
            inside = !inside;
        }
    }

    return inside;
}

inline bool point_in_domain_2d(const vmm::domain::PolygonalDomain2D& domain,
                               vmm::domain::Point2D point) noexcept
{
    if(!point_in_ring_2d(domain.exterior(), point)) {
        return false;
    }
    for(const auto& hole : domain.holes()) {
        if(point_in_ring_2d(hole, point)) {
            return false;
        }
    }
    return true;
}

} // namespace detail

class HexagonalSitePattern2D final {
public:
    explicit HexagonalSitePattern2D(vmm::core::Real spacing,
                                    vmm::domain::Point2D origin = {}) noexcept
        : spacing_(spacing)
        , origin_(origin)
    {}

    [[nodiscard]] vmm::core::Real spacing() const noexcept
    {
        return spacing_;
    }

    [[nodiscard]] vmm::domain::Point2D origin() const noexcept
    {
        return origin_;
    }

    [[nodiscard]] std::vector<vmm::domain::Point2D> generate(
        const vmm::domain::PolygonalDomain2D& domain) const
    {
        if(!(spacing_ > vmm::core::Real{})) {
            vmm::error::throw_invalid_argument(
                "HexagonalSitePattern2D",
                "Site spacing must be positive. / O espacamento dos pontos geradores deve ser positivo.");
        }

        const auto bounds = domain.bounds();
        const auto row_step = spacing_ * std::sqrt(static_cast<vmm::core::Real>(3)) / static_cast<vmm::core::Real>(2);
        std::vector<vmm::domain::Point2D> sites;

        std::size_t row = 0U;
        for(auto y = bounds.min.y; y <= bounds.max.y; y += row_step) {
            const auto offset = (row % 2U == 0U) ? vmm::core::Real{} : spacing_ / static_cast<vmm::core::Real>(2);
            for(auto x = bounds.min.x; x <= bounds.max.x; x += spacing_) {
                const vmm::domain::Point2D candidate{x + offset + origin_.x, y + origin_.y};
                if(detail::point_in_domain_2d(domain, candidate)) {
                    sites.push_back(candidate);
                }
            }
            ++row;
        }

        return sites;
    }

private:
    vmm::core::Real spacing_{};
    vmm::domain::Point2D origin_{};
};

template<>
struct SitePatternTraits<HexagonalSitePattern2D> {
    static constexpr std::string_view id = hexagonal_site_pattern_2d_id;
};

inline std::vector<vmm::domain::Point2D> make_hexagonal_sites_2d(
    const vmm::domain::PolygonalDomain2D& domain,
    vmm::core::Real spacing)
{
    return HexagonalSitePattern2D{spacing}.generate(domain);
}

} // namespace vmm::site_generation
