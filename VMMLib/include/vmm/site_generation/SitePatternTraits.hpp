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
// @file SitePatternTraits.hpp
// @brief Open identifiers for built-in point generator site patterns.
// @ingroup vmm_site_generation
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::site_generation {

template<typename Pattern>
struct SitePatternTraits;

inline constexpr std::string_view hexagonal_site_pattern_2d_id = "hexagonal_2d";

} // namespace vmm::site_generation
