//==============================================================================
// File        : MeshAnalyzer.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the MeshAnalyzer quality validation module.
//               Computes topological regularity and centroid spacing variance
//               as early-stage quality proxies before full geometric dualisation.
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
// @file MeshAnalyzer.cpp
// @brief Implementation of the mesh quality analyser.
// @ingroup vmm_analysis
//==============================================================================


//==============================================================================
// c++ includes
//==============================================================================
#include <numeric>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/analysis/MeshAnalyzer.hpp>

namespace vmm::analysis {

template<core::Dimension Dim>
ValidationReport MeshAnalyzer::analyse(const core::MeshTopology<Dim>& mesh,
                                       const QualityCriteria& criteria) {
    ValidationReport report;
    const auto n_cells = mesh.cell_count();
    if (n_cells == 0) {
        report.passed = false;
        report.violations.push_back({0, "empty_mesh", static_cast<core::Real>(1.0), static_cast<core::Real>(0.0), "Mesh contains zero cells."});
        return report;
    }

    const auto centroids = mesh.centroids();
    std::vector<core::Real> neighbour_counts(n_cells);
    std::vector<core::Real> mean_neighbour_dist(n_cells);

    // Hot loop: topological & spacing metrics (zero allocation)
    for (core::CellIndex i = 0; i < n_cells; ++i) {
        const auto neighbours = mesh.neighbours(i);
        neighbour_counts[i] = static_cast<core::Real>(neighbours.size());

        core::Real dist_sum = static_cast<core::Real>(0.0);
        for (core::CellIndex n_id : neighbours) {
            if constexpr (Dim == core::Dimension::D2) {
                dist_sum += std::hypot(centroids[i].x - centroids[n_id].x,
                                       centroids[i].y - centroids[n_id].y);
            } else {
                dist_sum += std::hypot(centroids[i].x - centroids[n_id].x,
                                       centroids[i].y - centroids[n_id].y,
                                       centroids[i].z - centroids[n_id].z);
            }
        }
        mean_neighbour_dist[i] = (neighbours.empty()) ? static_cast<core::Real>(0.0) : dist_sum / static_cast<core::Real>(neighbours.size());
    }

    // Aggregate metrics
    const core::Real mean_degree = std::accumulate(neighbour_counts.begin(), neighbour_counts.end(), static_cast<core::Real>(0.0)) / static_cast<core::Real>(n_cells);
    report.mean_neighbour_degree = mean_degree;

    // Compute centroid spacing variance (proxy for cell size uniformity)
    const core::Real mean_dist = std::accumulate(mean_neighbour_dist.begin(), mean_neighbour_dist.end(), static_cast<core::Real>(0.0)) / static_cast<core::Real>(n_cells);
    core::Real var_sum = static_cast<core::Real>(0.0);
    for (core::Real d : mean_neighbour_dist) {
        const core::Real diff = d - mean_dist;
        var_sum += diff * diff;
    }
    report.centroid_spacing_var = (mean_dist > static_cast<core::Real>(0.0)) ? var_sum / static_cast<core::Real>(n_cells) : static_cast<core::Real>(0.0);

    // Dimension-specific threshold checks
    const core::Real min_degree = (Dim == core::Dimension::D2) ? static_cast<core::Real>(3.0) : static_cast<core::Real>(4.0);
    const core::Real max_degree = (Dim == core::Dimension::D2) ? static_cast<core::Real>(10.0) : static_cast<core::Real>(20.0);

    for (core::CellIndex i = 0; i < n_cells; ++i) {
        const core::Real deg = neighbour_counts[i];
        const auto gid = mesh.generator_ids()[i];

        if (deg < min_degree) {
            report.violations.push_back({gid, "neighbour_degree_low", min_degree, deg,
                                         "Cell has fewer neighbours than topological minimum."});
            report.passed = false;
        } else if (deg > max_degree) {
            report.violations.push_back({gid, "neighbour_degree_high", max_degree, deg,
                                         "Cell has excessive neighbours; may indicate clustering or numerical artefact."});
            report.passed = false;
        }

        // Size uniformity check (simple proxy)
        if (mean_neighbour_dist[i] < mean_dist * criteria.min_volume_ratio) {
            report.violations.push_back({gid, "cell_too_small", mean_dist * criteria.min_volume_ratio, mean_neighbour_dist[i],
                                         "Cell spacing significantly below mean; may cause solver instability."});
            if (criteria.fail_on_violation) report.passed = false;
        } else if (mean_neighbour_dist[i] > mean_dist * criteria.max_volume_ratio) {
            report.violations.push_back({gid, "cell_too_large", mean_dist * criteria.max_volume_ratio, mean_neighbour_dist[i],
                                         "Cell spacing significantly above mean; may under-resolve physics."});
            if (criteria.fail_on_violation) report.passed = false;
        }
    }

    // NOTE: Full internal angles, orthogonality, and skewness require face/vertex
    // coordinates. These will be computed post-CGAL dualisation when MeshGeometry
    // is available. The current pipeline validates topological soundness early.

    return report;
}

// Explicit instantiations
template ValidationReport MeshAnalyzer::analyse(const core::MeshTopology<core::Dimension::D2>&, const QualityCriteria&);
template ValidationReport MeshAnalyzer::analyse(const core::MeshTopology<core::Dimension::D3>&, const QualityCriteria&);

} // namespace vmm::analysis