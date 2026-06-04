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
// @file SitePatternConcept.hpp
// @brief Concepts for extensible point generator site patterns.
// @ingroup vmm_site_generation
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>

#include <concepts>
#include <vector>

namespace vmm::site_generation {

template<typename Pattern>
concept SitePattern2D = requires(const Pattern& pattern, const vmm::domain::PolygonalDomain2D& domain) {
    { pattern.generate(domain) } -> std::same_as<std::vector<vmm::domain::Point2D>>;
};

} // namespace vmm::site_generation
