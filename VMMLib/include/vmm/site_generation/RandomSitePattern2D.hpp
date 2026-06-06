//==============================================================================
// File        : RandomSitePattern2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Deterministic-seed random 2D point generator site pattern.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstdint>
#include <random>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/site_generation/SitePatternTraits.hpp>

namespace vmm::site_generation {

class RandomSitePattern2D final {
public:
    RandomSitePattern2D(std::size_t target_count, std::uint32_t seed = 5489U) noexcept
        : target_count_(target_count),
          seed_(seed)
    {
    }

    [[nodiscard]] std::vector<vmm::domain::Point2D> generate(
        const vmm::domain::PolygonalDomain2D& domain) const
    {
        if(target_count_ == 0U) {
            vmm::error::throw_invalid_argument("RandomSitePattern2D", "Random site target count must be positive.");
        }

        const auto bounds = domain.bounds();
        std::mt19937 generator{seed_};
        std::uniform_real_distribution<vmm::core::Real> x_distribution{bounds.min.x, bounds.max.x};
        std::uniform_real_distribution<vmm::core::Real> y_distribution{bounds.min.y, bounds.max.y};
        std::vector<vmm::domain::Point2D> sites;
        sites.reserve(target_count_);

        const auto max_attempts = target_count_ * 100U;
        for(std::size_t attempt = 0U; attempt < max_attempts && sites.size() < target_count_; ++attempt) {
            const vmm::domain::Point2D candidate{x_distribution(generator), y_distribution(generator)};
            if(detail::point_in_domain_2d(domain, candidate)) {
                sites.push_back(candidate);
            }
        }
        if(sites.size() != target_count_) {
            vmm::error::throw_invalid_argument("RandomSitePattern2D",
                                               "Unable to place the requested number of random sites inside the domain.");
        }
        return sites;
    }

private:
    std::size_t target_count_{};
    std::uint32_t seed_{};
};

template<>
struct SitePatternTraits<RandomSitePattern2D> {
    static constexpr std::string_view id = random_site_pattern_2d_id;
};

} // namespace vmm::site_generation
