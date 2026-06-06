//==============================================================================
// File        : CartesianSitePattern2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Cartesian 2D point generator site pattern.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/site_generation/SitePatternTraits.hpp>

namespace vmm::site_generation {

class CartesianSitePattern2D final {
public:
    explicit CartesianSitePattern2D(vmm::core::Real spacing,
                                    vmm::domain::Point2D origin = {}) noexcept
        : spacing_(spacing),
          origin_(origin)
    {
    }

    [[nodiscard]] std::vector<vmm::domain::Point2D> generate(
        const vmm::domain::PolygonalDomain2D& domain) const
    {
        if(!(spacing_ > 0.0)) {
            vmm::error::throw_invalid_argument("CartesianSitePattern2D", "Site spacing must be positive.");
        }

        const auto bounds = domain.bounds();
        std::vector<vmm::domain::Point2D> sites;
        for(auto y = bounds.min.y + origin_.y; y <= bounds.max.y; y += spacing_) {
            for(auto x = bounds.min.x + origin_.x; x <= bounds.max.x; x += spacing_) {
                const vmm::domain::Point2D candidate{x, y};
                if(detail::point_in_domain_2d(domain, candidate)) {
                    sites.push_back(candidate);
                }
            }
        }
        return sites;
    }

private:
    vmm::core::Real spacing_{};
    vmm::domain::Point2D origin_{};
};

template<>
struct SitePatternTraits<CartesianSitePattern2D> {
    static constexpr std::string_view id = cartesian_site_pattern_2d_id;
};

[[nodiscard]] inline std::vector<vmm::domain::Point2D> make_cartesian_sites_2d(
    const vmm::domain::PolygonalDomain2D& domain,
    vmm::core::Real spacing)
{
    return CartesianSitePattern2D{spacing}.generate(domain);
}

} // namespace vmm::site_generation
