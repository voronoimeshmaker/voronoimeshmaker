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
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <CGAL/Hilbert_sort_2.h>
#include <CGAL/Hilbert_sort_median_2.h>
#include <CGAL/tags.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/error/ErrorId.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/CellRenumbering2D.hpp>

namespace vmm::mesh {
namespace {

using vmm::core::CellId;
using vmm::core::Real;
using vmm::core::is_valid;

struct HilbertCellPoint2D final {
    Real x{};
    Real y{};
    std::size_t cell_index{};
};

struct HilbertCellSortTraits2D final {
    using Point_2 = HilbertCellPoint2D;

    struct LessX2 final {
        [[nodiscard]] bool operator()(const Point_2& lhs, const Point_2& rhs) const noexcept
        {
            if(lhs.x == rhs.x) {
                if(lhs.y == rhs.y) {
                    return lhs.cell_index < rhs.cell_index;
                }
                return lhs.y < rhs.y;
            }
            return lhs.x < rhs.x;
        }
    };

    struct LessY2 final {
        [[nodiscard]] bool operator()(const Point_2& lhs, const Point_2& rhs) const noexcept
        {
            if(lhs.y == rhs.y) {
                if(lhs.x == rhs.x) {
                    return lhs.cell_index < rhs.cell_index;
                }
                return lhs.x < rhs.x;
            }
            return lhs.y < rhs.y;
        }
    };

    [[nodiscard]] LessX2 less_x_2_object() const noexcept { return {}; }
    [[nodiscard]] LessY2 less_y_2_object() const noexcept { return {}; }
};

[[nodiscard]] CellPermutation2D make_permutation_from_new_to_old(std::vector<std::size_t> new_to_old_indices)
{
    CellPermutation2D permutation;
    permutation.old_to_new.resize(new_to_old_indices.size());
    permutation.new_to_old.reserve(new_to_old_indices.size());

    for(std::size_t new_index = 0U; new_index < new_to_old_indices.size(); ++new_index) {
        const auto old_index = new_to_old_indices[new_index];
        permutation.new_to_old.push_back(CellId{old_index});
        permutation.old_to_new[old_index] = CellId{new_index};
    }

    return permutation;
}

[[nodiscard]] std::vector<std::size_t> identity_indices(std::size_t count)
{
    std::vector<std::size_t> indices(count);
    std::iota(indices.begin(), indices.end(), std::size_t{0U});
    return indices;
}

void validate_mesh_for_renumbering(const FiniteVolumeMesh2D& mesh, std::string_view context)
{
    const auto geometry_report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    if(!geometry_report.valid()) {
        const auto code = geometry_report.empty() ? std::string{"unknown"} : geometry_report.issues.front().code;
        vmm::error::throw_invalid_argument(context,
                                           "Cannot renumber invalid mesh. First geometry issue: " + code);
    }

    const auto patch_report = vmm::audit::audit_mesh_patches_2d(mesh);
    if(!patch_report.valid()) {
        const auto code = patch_report.empty() ? std::string{"unknown"} : patch_report.issues.front().code;
        vmm::error::throw_invalid_argument(context,
                                           "Cannot renumber invalid mesh. First patch issue: " + code);
    }
}

void validate_options(const CellRenumbering2DOptions& options, std::string_view context)
{
    if(options.space_filling_curve_bits == 0U || options.space_filling_curve_bits > 31U) {
        vmm::error::throw_invalid_argument(context, "Space-filling curve resolution must be between 1 and 31 bits.");
    }
}

[[nodiscard]] std::vector<std::size_t> reverse_cuthill_mckee_indices(const FiniteVolumeMesh2D& mesh)
{
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

    Graph graph(mesh.cell_count());
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto neighbour = mesh.faces.neighbour[face];
        if(!is_valid(neighbour)) {
            continue;
        }

        boost::add_edge(static_cast<std::size_t>(mesh.faces.owner[face].value),
                        static_cast<std::size_t>(neighbour.value),
                        graph);
    }

    std::vector<std::size_t> new_to_old(mesh.cell_count());
    boost::cuthill_mckee_ordering(graph, new_to_old.rbegin());
    return new_to_old;
}

[[nodiscard]] std::uint64_t expand_bits_2d(std::uint32_t value) noexcept
{
    std::uint64_t expanded{};
    for(std::uint32_t bit = 0U; bit < 32U; ++bit) {
        expanded |= (static_cast<std::uint64_t>((value >> bit) & 1U) << (2U * bit));
    }
    return expanded;
}

[[nodiscard]] std::uint64_t morton_key(std::uint32_t x, std::uint32_t y) noexcept
{
    return expand_bits_2d(x) | (expand_bits_2d(y) << 1U);
}

[[nodiscard]] std::uint32_t quantize(Real value, Real minimum, Real maximum, std::uint8_t bits) noexcept
{
    if(maximum <= minimum) {
        return 0U;
    }

    const auto largest_value = (std::uint64_t{1U} << bits) - 1U;
    const auto normalized = std::clamp((value - minimum) / (maximum - minimum), Real{0.0}, Real{1.0});
    return static_cast<std::uint32_t>(std::llround(normalized * static_cast<Real>(largest_value)));
}

struct MortonCellKey2D final {
    std::uint64_t key{};
    std::size_t cell_index{};
};

[[nodiscard]] std::vector<MortonCellKey2D> morton_keys(const FiniteVolumeMesh2D& mesh, std::uint8_t bits)
{
    auto min_x = std::numeric_limits<Real>::max();
    auto min_y = std::numeric_limits<Real>::max();
    auto max_x = std::numeric_limits<Real>::lowest();
    auto max_y = std::numeric_limits<Real>::lowest();

    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        min_x = std::min(min_x, mesh.cells.centre_x[cell]);
        min_y = std::min(min_y, mesh.cells.centre_y[cell]);
        max_x = std::max(max_x, mesh.cells.centre_x[cell]);
        max_y = std::max(max_y, mesh.cells.centre_y[cell]);
    }

    std::vector<MortonCellKey2D> keys;
    keys.reserve(mesh.cell_count());
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto x = quantize(mesh.cells.centre_x[cell], min_x, max_x, bits);
        const auto y = quantize(mesh.cells.centre_y[cell], min_y, max_y, bits);
        keys.push_back(MortonCellKey2D{morton_key(x, y), cell});
    }
    return keys;
}

[[nodiscard]] std::vector<std::size_t> sorted_morton_indices(const FiniteVolumeMesh2D& mesh, std::uint8_t bits)
{
    auto keys = morton_keys(mesh, bits);
    std::sort(keys.begin(), keys.end(), [](const MortonCellKey2D& lhs, const MortonCellKey2D& rhs) {
        if(lhs.key == rhs.key) {
            return lhs.cell_index < rhs.cell_index;
        }
        return lhs.key < rhs.key;
    });

    std::vector<std::size_t> indices;
    indices.reserve(keys.size());
    for(const auto& key : keys) {
        indices.push_back(key.cell_index);
    }
    return indices;
}

[[nodiscard]] std::vector<std::size_t> sorted_hilbert_indices(const FiniteVolumeMesh2D& mesh)
{
    std::vector<HilbertCellPoint2D> points;
    points.reserve(mesh.cell_count());
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        points.push_back(HilbertCellPoint2D{mesh.cells.centre_x[cell], mesh.cells.centre_y[cell], cell});
    }

    CGAL::Hilbert_sort_2<HilbertCellSortTraits2D, CGAL::Hilbert_sort_median_policy, CGAL::Sequential_tag> sorter{
        HilbertCellSortTraits2D{}};
    sorter(points.begin(), points.end());

    std::vector<std::size_t> indices;
    indices.reserve(points.size());
    for(const auto& point : points) {
        indices.push_back(point.cell_index);
    }
    return indices;
}

void maybe_validate_mesh(const FiniteVolumeMesh2D& mesh,
                         const CellRenumbering2DOptions& options,
                         std::string_view context)
{
    if(options.require_valid_mesh) {
        validate_mesh_for_renumbering(mesh, context);
    }
}

[[nodiscard]] CellPermutation2D compute_native(const FiniteVolumeMesh2D& mesh, const CellRenumbering2DOptions&)
{
    return cell_renumbering_native_2d(mesh);
}

} // namespace

bool CellPermutation2D::has_consistent_storage() const noexcept
{
    if(old_to_new.size() != new_to_old.size()) {
        return false;
    }

    std::vector<bool> seen_old(old_to_new.size(), false);
    std::vector<bool> seen_new(old_to_new.size(), false);

    for(std::size_t old_index = 0U; old_index < old_to_new.size(); ++old_index) {
        const auto new_id = old_to_new[old_index];
        if(!is_valid(new_id) || new_id.value >= old_to_new.size()) {
            return false;
        }
        seen_new[static_cast<std::size_t>(new_id.value)] = true;
    }

    for(std::size_t new_index = 0U; new_index < new_to_old.size(); ++new_index) {
        const auto old_id = new_to_old[new_index];
        if(!is_valid(old_id) || old_id.value >= new_to_old.size()) {
            return false;
        }
        seen_old[static_cast<std::size_t>(old_id.value)] = true;
        if(old_to_new[static_cast<std::size_t>(old_id.value)] != CellId{new_index}) {
            return false;
        }
    }

    return std::all_of(seen_old.begin(), seen_old.end(), [](bool value) { return value; }) &&
           std::all_of(seen_new.begin(), seen_new.end(), [](bool value) { return value; });
}

CellId CellPermutation2D::new_id_for(CellId old_id) const
{
    if(!is_valid(old_id) || old_id.value >= old_to_new.size()) {
        vmm::error::throw_invalid_argument("CellPermutation2D::new_id_for", "Cell id exceeds permutation bounds.");
    }
    return old_to_new[static_cast<std::size_t>(old_id.value)];
}

CellId CellPermutation2D::old_id_for(CellId new_id) const
{
    if(!is_valid(new_id) || new_id.value >= new_to_old.size()) {
        vmm::error::throw_invalid_argument("CellPermutation2D::old_id_for", "Cell id exceeds permutation bounds.");
    }
    return new_to_old[static_cast<std::size_t>(new_id.value)];
}

CellPermutation2D cell_renumbering_native_2d(const FiniteVolumeMesh2D& mesh)
{
    return make_permutation_from_new_to_old(identity_indices(mesh.cell_count()));
}

CellPermutation2D cell_renumbering_random_2d(const FiniteVolumeMesh2D& mesh,
                                             const CellRenumbering2DOptions& options)
{
    maybe_validate_mesh(mesh, options, "cell_renumbering_random_2d");

    auto indices = identity_indices(mesh.cell_count());
    std::mt19937 generator{options.random_seed};
    std::shuffle(indices.begin(), indices.end(), generator);
    return make_permutation_from_new_to_old(std::move(indices));
}

CellPermutation2D cell_renumbering_reverse_cuthill_mckee_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options)
{
    maybe_validate_mesh(mesh, options, "cell_renumbering_reverse_cuthill_mckee_2d");
    return make_permutation_from_new_to_old(reverse_cuthill_mckee_indices(mesh));
}

CellPermutation2D cell_renumbering_hilbert_2d(const FiniteVolumeMesh2D& mesh,
                                              const CellRenumbering2DOptions& options)
{
    maybe_validate_mesh(mesh, options, "cell_renumbering_hilbert_2d");
    return make_permutation_from_new_to_old(sorted_hilbert_indices(mesh));
}

CellPermutation2D cell_renumbering_morton_2d(const FiniteVolumeMesh2D& mesh,
                                             const CellRenumbering2DOptions& options)
{
    validate_options(options, "cell_renumbering_morton_2d");
    maybe_validate_mesh(mesh, options, "cell_renumbering_morton_2d");
    return make_permutation_from_new_to_old(sorted_morton_indices(mesh, options.space_filling_curve_bits));
}

CellPermutation2D cell_renumbering_metis_nested_dissection_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options)
{
    maybe_validate_mesh(mesh, options, "cell_renumbering_metis_nested_dissection_2d");
    vmm::error::throw_error(vmm::error::builtin_error_id::unsupported_operation,
                            "cell_renumbering_metis_nested_dissection_2d",
                            "METIS-ND renumbering requires an optional METIS backend that is not enabled.");
}

CellPermutation2D compute_cell_renumbering_2d(const FiniteVolumeMesh2D& mesh,
                                              CellRenumberingMethod2D method,
                                              const CellRenumbering2DOptions& options)
{
    return compute_cell_renumbering_2d(mesh, method, default_cell_renumbering_algorithms_2d(), options);
}

std::span<const CellRenumberingAlgorithm2D> default_cell_renumbering_algorithms_2d() noexcept
{
    static constexpr std::array algorithms{
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::Native, compute_native},
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::Random, cell_renumbering_random_2d},
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::ReverseCuthillMckee,
                                   cell_renumbering_reverse_cuthill_mckee_2d},
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::Hilbert, cell_renumbering_hilbert_2d},
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::Morton, cell_renumbering_morton_2d},
        CellRenumberingAlgorithm2D{CellRenumberingMethod2DTraits::MetisNestedDissection,
                                   cell_renumbering_metis_nested_dissection_2d},
    };

    return algorithms;
}

CellPermutation2D compute_cell_renumbering_2d(const FiniteVolumeMesh2D& mesh,
                                              CellRenumberingMethod2D method,
                                              std::span<const CellRenumberingAlgorithm2D> algorithms,
                                              const CellRenumbering2DOptions& options)
{
    const auto entry = std::find_if(algorithms.begin(), algorithms.end(), [method](const auto& algorithm) {
        return algorithm.method == method;
    });
    if(entry != algorithms.end() && entry->compute != nullptr) {
        return entry->compute(mesh, options);
    }

    vmm::error::throw_invalid_argument("compute_cell_renumbering_2d",
                                       "Unknown cell renumbering method: " + std::string{method.name});
}

std::uint64_t finite_volume_matrix_half_bandwidth_2d(const FiniteVolumeStencilGraph2D& graph,
                                                     const CellPermutation2D& permutation)
{
    if(!permutation.has_consistent_storage()) {
        vmm::error::throw_invalid_argument("finite_volume_matrix_half_bandwidth_2d",
                                           "Cell permutation storage is inconsistent.");
    }

    std::uint64_t half_bandwidth{};
    for(std::size_t entry = 0U; entry < graph.internal.size(); ++entry) {
        const auto owner = permutation.new_id_for(graph.internal.owner[entry]).value;
        const auto neighbour = permutation.new_id_for(graph.internal.neighbour[entry]).value;
        const auto distance = owner > neighbour ? owner - neighbour : neighbour - owner;
        half_bandwidth = std::max(half_bandwidth, distance);
    }
    return half_bandwidth;
}

std::uint64_t finite_volume_matrix_bandwidth_2d(const FiniteVolumeStencilGraph2D& graph,
                                                const CellPermutation2D& permutation)
{
    return (2U * finite_volume_matrix_half_bandwidth_2d(graph, permutation)) + 1U;
}

} // namespace vmm::mesh
