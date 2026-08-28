//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 VoronoiMeshMaker
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
// @file CentroidalVoronoiRelaxation2D.hpp
// @brief Lloyd relaxation for centroidal Voronoi tessellations.
// @ingroup vmm_tessellation
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <span>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/tessellation/TessellationResult.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>

namespace vmm::tessellation {

struct LloydIterationReport2D final {
    std::size_t iteration{};
    vmm::core::Real mean_generator_displacement{};
    vmm::core::Real max_generator_displacement{};
    bool converged{};
};

struct LloydRelaxation2DOptions final {
    std::size_t iteration_count{};
    vmm::core::Real convergence_tolerance{};
    GenerationParams2D tessellation;
};

struct LloydRelaxation2DResult final {
    std::vector<vmm::domain::Point2D> sites;
    ClippedVoronoiMesh2D clipped_voronoi;
    std::vector<LloydIterationReport2D> iterations;
    bool converged{};

    [[nodiscard]] std::size_t completed_iteration_count() const noexcept { return iterations.size(); }
};

[[nodiscard]] LloydRelaxation2DResult relax_sites_lloyd_2d(
    std::span<const vmm::domain::Point2D> sites,
    const vmm::domain::PolygonalDomain2D& domain,
    const LloydRelaxation2DOptions& options = {});

} // namespace vmm::tessellation
