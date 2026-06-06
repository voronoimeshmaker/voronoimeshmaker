//==============================================================================
// File        : RadialSitePattern2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Radial 2D point generator site pattern.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <numbers>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/site_generation/SitePatternTraits.hpp>

namespace vmm::site_generation {

class RadialSitePattern2D final {
public:
    RadialSitePattern2D(vmm::core::Real radial_spacing,
                        std::size_t angular_count,
                        vmm::domain::Point2D centre = {}) noexcept
        : radial_spacing_(radial_spacing),
          angular_count_(angular_count),
          centre_(centre)
    {
    }

    [[nodiscard]] std::vector<vmm::domain::Point2D> generate(
        const vmm::domain::PolygonalDomain2D& domain) const
    {
        if(!(radial_spacing_ > 0.0) || angular_count_ < 3U) {
            vmm::error::throw_invalid_argument("RadialSitePattern2D",
                                               "Radial spacing must be positive and angular count must be at least three.");
        }

        const auto bounds = domain.bounds();
        const auto max_radius = std::hypot(std::max(std::abs(bounds.min.x - centre_.x), std::abs(bounds.max.x - centre_.x)),
                                           std::max(std::abs(bounds.min.y - centre_.y), std::abs(bounds.max.y - centre_.y)));
        const auto pi = static_cast<vmm::core::Real>(std::numbers::pi_v<long double>);
        std::vector<vmm::domain::Point2D> sites;
        if(detail::point_in_domain_2d(domain, centre_)) {
            sites.push_back(centre_);
        }
        for(auto radius = radial_spacing_; radius <= max_radius; radius += radial_spacing_) {
            for(std::size_t i = 0U; i < angular_count_; ++i) {
                const auto angle = 2.0 * pi * static_cast<vmm::core::Real>(i) /
                                   static_cast<vmm::core::Real>(angular_count_);
                const vmm::domain::Point2D candidate{
                    centre_.x + (radius * std::cos(angle)),
                    centre_.y + (radius * std::sin(angle))};
                if(detail::point_in_domain_2d(domain, candidate)) {
                    sites.push_back(candidate);
                }
            }
        }
        return sites;
    }

private:
    vmm::core::Real radial_spacing_{};
    std::size_t angular_count_{};
    vmm::domain::Point2D centre_{};
};

template<>
struct SitePatternTraits<RadialSitePattern2D> {
    static constexpr std::string_view id = radial_site_pattern_2d_id;
};

} // namespace vmm::site_generation
