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
// @file DomainConcept.hpp
// @brief Concepts for domain-like 2D objects.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/domain/Polyhedron3D.hpp>

#include <concepts>

namespace vmm::domain {

template<class T>
concept PolygonalDomain = requires(const T& domain) {
    { domain.exterior() } -> std::same_as<const Polygon2D&>;
    { domain.area() } -> std::convertible_to<vmm::core::Real>;
};

template<class T>
concept PolyhedralDomain = requires(const T& domain) {
    { domain.surface() } -> std::same_as<const SurfaceMesh3D&>;
    { domain.volume() } -> std::convertible_to<vmm::core::Real>;
};

} // namespace vmm::domain
