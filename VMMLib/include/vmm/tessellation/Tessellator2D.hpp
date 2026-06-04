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
// @file Tessellator2D.hpp
// @brief Public API for 2D Voronoi mesh generation.
// @ingroup vmm_tessellation
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/mesh/MeshTopology.hpp>
#include <vmm/tessellation/TessellationResult.hpp>

#include <span>
#include <vector>

namespace vmm::tessellation {

struct GenerationParams2D final {
    vmm::core::Real min_edge_length = static_cast<vmm::core::Real>(0);
    vmm::core::Real max_edge_length = static_cast<vmm::core::Real>(1.0e30);
    bool enforce_conformity = true;
    bool clip_to_domain = true;
    bool verbose = false;
};

[[nodiscard]] vmm::core::MeshTopology<vmm::core::Dimension::D2>
generate_voronoi_2d(std::span<const vmm::domain::Point2D> sites,
                    const vmm::domain::PolygonalDomain2D& domain,
                    const GenerationParams2D& params = {});

[[nodiscard]] ClippedVoronoiMesh2D
generate_clipped_voronoi_2d(std::span<const vmm::domain::Point2D> sites,
                            const vmm::domain::PolygonalDomain2D& domain,
                            const GenerationParams2D& params = {});

[[nodiscard]] vmm::core::MeshTopology<vmm::core::Dimension::D2>
generate_voronoi_2d(
    const std::vector<vmm::core::Point<vmm::core::Dimension::D2>>& sites,
    const std::vector<vmm::core::Point<vmm::core::Dimension::D2>>& boundary,
    const std::vector<std::vector<vmm::core::Point<vmm::core::Dimension::D2>>>& holes,
    const GenerationParams2D& params = {});

} // namespace vmm::tessellation
