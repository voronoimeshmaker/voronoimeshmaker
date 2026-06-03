//==============================================================================
// File        : Tessellator2D.hpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Public API for 2D Voronoi mesh generation. Provides a clean,
//               CGAL-free interface for constrained Delaunay triangulation,
//               hole handling, and dual Voronoi construction.
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.
//
// @file Tessellator2D.hpp
// @brief Public API for 2D Voronoi mesh generation with hole support.
// @ingroup vmm_tessellation
//==============================================================================
#pragma once

#include <vector>
#include <memory>
#include "vmm/core/Types.hpp"
#include "vmm/mesh/MeshTopology.hpp"

namespace vmm::tessellation {

/**
 * @brief Configuration parameters for 2D Voronoi generation.
 * @ingroup vmm_tessellation
 */
struct GenerationParams2D {
    core::Real min_edge_length      = static_cast<core::Real>(0.0);  ///< Minimum allowed edge length
    core::Real max_edge_length      = static_cast<core::Real>(1e30); ///< Maximum allowed edge length
    bool       enforce_conformity   = true;  ///< Enforce boundary conformity
    bool       clip_to_domain       = true;  ///< Clip Voronoi cells to domain boundary
    bool       verbose              = false; ///< Enable diagnostic output
};

/**
 * @brief Generate a 2D Voronoi mesh from seeds and domain definition.
 * @param seeds Generator points (centroids of future Voronoi cells).
 * @param boundary External polygon boundary (counter-clockwise).
 * @param holes Optional internal holes (clockwise polygons).
 * @param params Generation parameters.
 * @returns Fully initialised MeshTopology<Dimension::D2>.
 * @ingroup vmm_tessellation
 *
 * @note The implementation uses CGAL internally, but the public API exposes
 *       only POD types and standard containers. No CGAL headers are included
 *       in this translation unit.
 */
core::MeshTopology<core::Dimension::D2>
generate_voronoi_2d(
    const std::vector<core::Point<core::Dimension::D2>>& seeds,
    const std::vector<core::Point<core::Dimension::D2>>& boundary,
    const std::vector<std::vector<core::Point<core::Dimension::D2>>>& holes,
    const GenerationParams2D& params = {});

} // namespace vmm::tessellation