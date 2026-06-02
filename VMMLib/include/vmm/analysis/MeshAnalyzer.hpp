//==============================================================================
// File        : MeshAnalyzer.hpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Quality analysis module for Voronoi meshes. Computes geometric
//               and topological metrics, and emits structured warnings when
//               configurable thresholds are violated.
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
// @file MeshAnalyzer.hpp
// @brief Quality analysis and configurable warning generation for meshes.
// @ingroup vmm_analysis
//==============================================================================
#pragma once


//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <string>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>
#include <vmm/core/MeshTopology.hpp>

namespace vmm::analysis {

/**
 * @brief Configurable thresholds for mesh quality validation.
 * @ingroup vmm_analysis
 */
struct QualityCriteria {
    core::Real min_orthogonality       = static_cast<core::Real>(0.95); ///< Minimum face orthogonality [0,1]
    core::Real max_skewness            = static_cast<core::Real>(0.8);  ///< Maximum cell skewness [0,1]
    core::Real min_internal_angle_2d   = static_cast<core::Real>(10.0); ///< Minimum internal angle in degrees
    core::Real max_internal_angle_2d   = static_cast<core::Real>(150.0);///< Maximum internal angle in degrees
    core::Real min_internal_angle_3d   = static_cast<core::Real>(5.0);  ///< Minimum dihedral angle in degrees
    core::Real max_internal_angle_3d   = static_cast<core::Real>(170.0);///< Maximum dihedral angle in degrees
    core::Real max_aspect_ratio        = static_cast<core::Real>(10.0); ///< Maximum cell aspect ratio
    core::Real min_volume_ratio        = static_cast<core::Real>(0.1);  ///< Minimum cell volume / mean volume
    core::Real max_volume_ratio        = static_cast<core::Real>(10.0); ///< Maximum cell volume / mean volume
    bool       fail_on_violation       = false;                          ///< Treat warnings as fatal errors
};

/**
 * @brief Structured record of a single quality violation.
 * @ingroup vmm_analysis
 */
struct Violation {
    uint64_t    entity_id;   ///< GeneratorID or CellIndex
    std::string metric_name; ///< e.g., "neighbour_degree_low"
    core::Real  expected;    ///< Threshold value
    core::Real  actual;      ///< Measured value
    std::string message;     ///< Human-readable explanation (en_GB)
};

/**
 * @brief Aggregated validation report returned by the analyser.
 * @ingroup vmm_analysis
 */
struct ValidationReport {
    bool                  passed                  = true;
    std::vector<Violation> violations;
    core::Real            min_orthogonality       = static_cast<core::Real>(1.0);
    core::Real            max_skewness            = static_cast<core::Real>(0.0);
    core::Real            min_angle               = static_cast<core::Real>(180.0);
    core::Real            max_angle               = static_cast<core::Real>(0.0);
    core::Real            mean_neighbour_degree   = static_cast<core::Real>(0.0);
    core::Real            centroid_spacing_var    = static_cast<core::Real>(0.0);
};

/**
 * @brief Stateless quality analyser for dimension-agnostic meshes.
 * @ingroup vmm_analysis
 *
 * Operates on read-only `MeshTopology` spans. No dynamic allocation occurs
 * within hot loops. Full angle/skewness computation requires face/vertex data
 * (available post-dualisation); current metrics use topological and centroid
 * proxies to validate early-stage generation.
 */
class MeshAnalyzer {
public:
    /**
     * @brief Analyses mesh quality against configurable criteria.
     * @tparam Dim Dimensional configuration.
     * @param mesh Read-only topology reference.
     * @param criteria Quality thresholds.
     * @returns Structured validation report.
     */
    template<core::Dimension Dim>
    static ValidationReport analyse(const core::MeshTopology<Dim>& mesh,
                                    const QualityCriteria& criteria = {});
};

} // namespace vmm::analysis