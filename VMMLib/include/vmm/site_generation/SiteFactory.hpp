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
// @file SiteFactory.hpp
// @brief Open factory for point generator site patterns.
// @ingroup vmm_site_generation
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace vmm::site_generation {

using SitePatternParams = std::unordered_map<std::string, vmm::core::Real>;
using SiteFactory2D = std::function<std::vector<vmm::domain::Point2D>(
    const vmm::domain::PolygonalDomain2D&, const SitePatternParams&)>;

class SiteFactoryRegistry2D final {
public:
    void register_factory(std::string id, SiteFactory2D factory)
    {
        factories_[std::move(id)] = std::move(factory);
    }

    [[nodiscard]] std::vector<vmm::domain::Point2D> make(
        std::string_view id,
        const vmm::domain::PolygonalDomain2D& domain,
        const SitePatternParams& params) const
    {
        const auto iterator = factories_.find(std::string{id});
        if(iterator == factories_.end()) {
            vmm::error::throw_invalid_argument(
                "SiteFactoryRegistry2D",
                "Unknown site pattern. / Padrao de pontos geradores desconhecido.");
        }
        return iterator->second(domain, params);
    }

private:
    std::unordered_map<std::string, SiteFactory2D> factories_;
};

inline vmm::core::Real site_param_or(const SitePatternParams& params,
                                     std::string_view key,
                                     vmm::core::Real fallback)
{
    const auto iterator = params.find(std::string{key});
    return iterator == params.end() ? fallback : iterator->second;
}

inline SiteFactoryRegistry2D default_site_factory_registry_2d()
{
    SiteFactoryRegistry2D registry;
    registry.register_factory(std::string{hexagonal_site_pattern_2d_id},
                              [](const vmm::domain::PolygonalDomain2D& domain,
                                 const SitePatternParams& params) {
                                  return make_hexagonal_sites_2d(
                                      domain,
                                      site_param_or(params, "spacing", static_cast<vmm::core::Real>(1)));
                              });
    return registry;
}

} // namespace vmm::site_generation
