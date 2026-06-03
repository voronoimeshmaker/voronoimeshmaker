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
// @file DomainTraits.hpp
// @brief Traits for domain geometry types.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/Ellipse2D.hpp>
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/domain/Triangle2D.hpp>

namespace vmm::domain {

template<class T>
struct DomainTraits;

template<>
struct DomainTraits<Rectangle2D> final {
    static constexpr std::string_view id{"rectangle"};
};

template<>
struct DomainTraits<Triangle2D> final {
    static constexpr std::string_view id{"triangle"};
};

template<>
struct DomainTraits<Ellipse2D> final {
    static constexpr std::string_view id{"ellipse"};
};

} // namespace vmm::domain
