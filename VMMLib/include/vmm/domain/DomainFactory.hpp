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
// @file DomainFactory.hpp
// @brief Open factory for 2D polygonal domains.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/Ellipse2D.hpp>
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/domain/Triangle2D.hpp>

#include <functional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

namespace vmm::domain {

using DomainParameterMap = std::unordered_map<std::string, vmm::core::Real>;
using DomainFactoryFunction = std::function<PolygonalDomain2D(const DomainParameterMap&)>;

class DomainFactoryRegistry final {
public:
    void register_factory(std::string id, DomainFactoryFunction factory)
    {
        factories_[std::move(id)] = std::move(factory);
    }

    [[nodiscard]] bool contains(std::string_view id) const
    {
        return factories_.find(std::string{id}) != factories_.end();
    }

    [[nodiscard]] PolygonalDomain2D make(std::string_view id, const DomainParameterMap& parameters) const
    {
        const auto iterator = factories_.find(std::string{id});
        if(iterator == factories_.end()) {
            vmm::error::throw_error(vmm::error::builtin_error_id::invalid_configuration,
                                    "DomainFactoryRegistry",
                                    "Unknown domain factory identifier. / Identificador de fabrica de dominio desconhecido.");
        }
        return iterator->second(parameters);
    }

private:
    std::unordered_map<std::string, DomainFactoryFunction> factories_;
};

inline vmm::core::Real parameter_or(const DomainParameterMap& parameters,
                                    std::string_view name,
                                    vmm::core::Real fallback)
{
    const auto iterator = parameters.find(std::string{name});
    if(iterator == parameters.end()) {
        return fallback;
    }
    return iterator->second;
}

inline DomainFactoryRegistry default_domain_factory_registry()
{
    DomainFactoryRegistry registry;
    registry.register_factory("rectangle", [](const DomainParameterMap& parameters) {
        return make_rectangle_2d(parameter_or(parameters, "width", {}),
                                 parameter_or(parameters, "height", {}),
                                 Point2D{parameter_or(parameters, "centre_x", {}),
                                         parameter_or(parameters, "centre_y", {})});
    });
    registry.register_factory("triangle", [](const DomainParameterMap& parameters) {
        return make_triangle_2d(Point2D{parameter_or(parameters, "x1", {}), parameter_or(parameters, "y1", {})},
                                Point2D{parameter_or(parameters, "x2", {}), parameter_or(parameters, "y2", {})},
                                Point2D{parameter_or(parameters, "x3", {}), parameter_or(parameters, "y3", {})});
    });
    registry.register_factory("ellipse", [](const DomainParameterMap& parameters) {
        const auto segments = static_cast<std::size_t>(parameter_or(parameters, "segments", 96.0));
        return make_ellipse_2d(parameter_or(parameters, "radius_x", {}),
                               parameter_or(parameters, "radius_y", {}),
                               Point2D{parameter_or(parameters, "centre_x", {}),
                                       parameter_or(parameters, "centre_y", {})},
                               segments);
    });
    return registry;
}

} // namespace vmm::domain
