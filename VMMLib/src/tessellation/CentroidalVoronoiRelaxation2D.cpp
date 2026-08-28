//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 VoronoiMeshMaker
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/tessellation/CentroidalVoronoiRelaxation2D.hpp>

namespace vmm::tessellation {
namespace {

[[nodiscard]] vmm::core::Real distance(vmm::domain::Point2D lhs, vmm::domain::Point2D rhs) noexcept
{
    const auto dx = lhs.x - rhs.x;
    const auto dy = lhs.y - rhs.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

void validate_options(const LloydRelaxation2DOptions& options)
{
    if(options.convergence_tolerance < vmm::core::Real{}) {
        vmm::error::throw_invalid_argument("relax_sites_lloyd_2d",
                                           "Lloyd convergence tolerance cannot be negative.");
    }
}

[[nodiscard]] std::vector<vmm::domain::Point2D> centroids_from_cells(const ClippedVoronoiMesh2D& mesh)
{
    std::vector<vmm::domain::Point2D> centroids;
    centroids.reserve(mesh.planar_cells.size());
    for(const auto& cell : mesh.planar_cells) {
        centroids.push_back(cell.centroid());
    }
    return centroids;
}

[[nodiscard]] LloydIterationReport2D displacement_report(std::span<const vmm::domain::Point2D> old_sites,
                                                        std::span<const vmm::domain::Point2D> new_sites,
                                                        std::size_t iteration,
                                                        vmm::core::Real tolerance)
{
    if(old_sites.size() != new_sites.size()) {
        vmm::error::throw_invalid_argument("relax_sites_lloyd_2d",
                                           "Lloyd iteration changed the number of generator sites.");
    }

    vmm::core::Real displacement_sum{};
    vmm::core::Real max_displacement{};
    for(std::size_t site = 0U; site < old_sites.size(); ++site) {
        const auto site_displacement = distance(old_sites[site], new_sites[site]);
        displacement_sum += site_displacement;
        max_displacement = std::max(max_displacement, site_displacement);
    }

    const auto mean_displacement = old_sites.empty()
        ? vmm::core::Real{}
        : displacement_sum / static_cast<vmm::core::Real>(old_sites.size());
    return LloydIterationReport2D{iteration,
                                  mean_displacement,
                                  max_displacement,
                                  tolerance > vmm::core::Real{} && max_displacement <= tolerance};
}

} // namespace

LloydRelaxation2DResult relax_sites_lloyd_2d(std::span<const vmm::domain::Point2D> sites,
                                             const vmm::domain::PolygonalDomain2D& domain,
                                             const LloydRelaxation2DOptions& options)
{
    validate_options(options);

    std::vector<vmm::domain::Point2D> relaxed_sites{sites.begin(), sites.end()};
    std::vector<LloydIterationReport2D> iterations;
    iterations.reserve(options.iteration_count);
    bool converged{};

    for(std::size_t iteration = 1U; iteration <= options.iteration_count; ++iteration) {
        const auto current_voronoi = generate_clipped_voronoi_2d(relaxed_sites, domain, options.tessellation);
        auto next_sites = centroids_from_cells(current_voronoi);
        auto report = displacement_report(relaxed_sites, next_sites, iteration, options.convergence_tolerance);

        relaxed_sites = std::move(next_sites);
        converged = report.converged;
        iterations.push_back(report);

        if(report.converged) {
            break;
        }
    }

    auto clipped_voronoi = generate_clipped_voronoi_2d(relaxed_sites, domain, options.tessellation);
    return LloydRelaxation2DResult{std::move(relaxed_sites),
                                   std::move(clipped_voronoi),
                                   std::move(iterations),
                                   converged};
}

} // namespace vmm::tessellation
