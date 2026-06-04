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
// @file TessellationResult.hpp
// @brief Result containers for generated tessellations.
// @ingroup vmm_tessellation
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/domain/Polygon2D.hpp>
#include <vmm/mesh/MeshTopology.hpp>

#include <vector>

namespace vmm::tessellation {

/**
 * @brief Complete result for a clipped 2D Voronoi generation pass.
 * @ingroup vmm_tessellation
 *
 * The `planar_cells` field is the richer geometry representation and should be
 * preferred by new code. The `cells` and `areas` fields are retained as compact
 * convenience views for the current examples and tests.
 */
struct ClippedVoronoiMesh2D final {
    vmm::core::MeshTopology<vmm::core::Dimension::D2> topology;
    std::vector<vmm::domain::PlanarCell2D> planar_cells;
    std::vector<vmm::domain::Polygon2D> cells;
    std::vector<vmm::core::Real> areas;
};

} // namespace vmm::tessellation
