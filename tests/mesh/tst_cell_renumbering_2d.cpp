//==============================================================================
// c++ includes
//==============================================================================
#include <array>
#include <cstddef>
#include <utility>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/ErrorId.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/CellRenumbering2D.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>
#include <vmm/mesh/FiniteVolumeStencilGraph2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::mesh;

namespace {

PlanarCell2D make_cell(std::vector<Point2D> vertices, Point2D site, GeneratorID generator_id)
{
    return PlanarCell2D{Polygon2D{std::move(vertices)}, site, generator_id};
}

FiniteVolumeMesh2D make_scrambled_four_square_mesh()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{2.0, 0.0}, {3.0, 0.0}, {3.0, 1.0}, {2.0, 1.0}}, {2.5, 0.5}, 1U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 2U),
        make_cell({{3.0, 0.0}, {4.0, 0.0}, {4.0, 1.0}, {3.0, 1.0}}, {3.5, 0.5}, 3U),
    };
    return build_finite_volume_mesh_2d(cells);
}

void expect_valid_permutation(const CellPermutation2D& permutation, std::size_t expected_size)
{
    ASSERT_EQ(permutation.size(), expected_size);
    ASSERT_TRUE(permutation.has_consistent_storage());

    for(std::size_t old_index = 0U; old_index < expected_size; ++old_index) {
        const auto old_id = CellId{old_index};
        const auto new_id = permutation.new_id_for(old_id);
        EXPECT_EQ(permutation.old_id_for(new_id), old_id);
    }
}

CellPermutation2D custom_reverse_ordering(const FiniteVolumeMesh2D& mesh, const CellRenumbering2DOptions&)
{
    std::vector<std::size_t> new_to_old;
    new_to_old.reserve(mesh.cell_count());
    for(std::size_t cell = mesh.cell_count(); cell > 0U; --cell) {
        new_to_old.push_back(cell - 1U);
    }

    CellPermutation2D permutation;
    permutation.old_to_new.resize(new_to_old.size());
    permutation.new_to_old.reserve(new_to_old.size());
    for(std::size_t new_index = 0U; new_index < new_to_old.size(); ++new_index) {
        const auto old_index = new_to_old[new_index];
        permutation.old_to_new[old_index] = CellId{new_index};
        permutation.new_to_old.push_back(CellId{old_index});
    }
    return permutation;
}

} // namespace

TEST(CellRenumbering2DTest, NativePermutationPreservesOriginalNumbering)
{
    const auto mesh = make_scrambled_four_square_mesh();

    const auto permutation = compute_cell_renumbering_2d(mesh, CellRenumberingMethod2DTraits::Native);

    expect_valid_permutation(permutation, mesh.cell_count());
    EXPECT_EQ(permutation.new_id_for(CellId{0U}), CellId{0U});
    EXPECT_EQ(permutation.new_id_for(CellId{1U}), CellId{1U});
    EXPECT_EQ(permutation.new_id_for(CellId{2U}), CellId{2U});
    EXPECT_EQ(permutation.new_id_for(CellId{3U}), CellId{3U});
}

TEST(CellRenumbering2DTest, RandomPermutationIsDeterministicForFixedSeed)
{
    const auto mesh = make_scrambled_four_square_mesh();
    CellRenumbering2DOptions options;
    options.random_seed = 17U;

    const auto first = cell_renumbering_random_2d(mesh, options);
    const auto second = cell_renumbering_random_2d(mesh, options);

    expect_valid_permutation(first, mesh.cell_count());
    EXPECT_EQ(first.old_to_new, second.old_to_new);
    EXPECT_EQ(first.new_to_old, second.new_to_old);
}

TEST(CellRenumbering2DTest, ReverseCuthillMckeeReducesBandwidthForScrambledLine)
{
    const auto mesh = make_scrambled_four_square_mesh();
    const auto graph = build_finite_volume_stencil_graph_2d(mesh);
    const auto permutation = cell_renumbering_reverse_cuthill_mckee_2d(mesh);

    expect_valid_permutation(permutation, mesh.cell_count());
    EXPECT_EQ(finite_volume_matrix_half_bandwidth_2d(graph), 2U);
    EXPECT_EQ(finite_volume_matrix_half_bandwidth_2d(graph, permutation), 1U);
}

TEST(CellRenumbering2DTest, HilbertOrderingUsesCGALSpatialSort)
{
    const auto mesh = make_scrambled_four_square_mesh();
    const auto graph = build_finite_volume_stencil_graph_2d(mesh);
    const auto permutation = cell_renumbering_hilbert_2d(mesh);

    expect_valid_permutation(permutation, mesh.cell_count());
    EXPECT_LE(finite_volume_matrix_half_bandwidth_2d(graph, permutation),
              finite_volume_matrix_half_bandwidth_2d(graph));
}

TEST(CellRenumbering2DTest, MortonOrderingSortsCellsByZOrderKey)
{
    const auto mesh = make_scrambled_four_square_mesh();
    CellRenumbering2DOptions options;
    options.space_filling_curve_bits = 8U;

    const auto permutation = cell_renumbering_morton_2d(mesh, options);

    expect_valid_permutation(permutation, mesh.cell_count());
    EXPECT_EQ(permutation.old_id_for(CellId{0U}), CellId{0U});
    EXPECT_EQ(permutation.old_id_for(CellId{1U}), CellId{2U});
    EXPECT_EQ(permutation.old_id_for(CellId{2U}), CellId{1U});
    EXPECT_EQ(permutation.old_id_for(CellId{3U}), CellId{3U});
}

TEST(CellRenumbering2DTest, PermutedBandwidthReportsFullMatrixBandwidth)
{
    const auto mesh = make_scrambled_four_square_mesh();
    const auto graph = build_finite_volume_stencil_graph_2d(mesh);
    const auto permutation = cell_renumbering_morton_2d(mesh);

    EXPECT_EQ(finite_volume_matrix_half_bandwidth_2d(graph, permutation), 1U);
    EXPECT_EQ(finite_volume_matrix_bandwidth_2d(graph, permutation), 3U);
}

TEST(CellRenumbering2DTest, MetisNestedDissectionReportsUnsupportedBackend)
{
    const auto mesh = make_scrambled_four_square_mesh();

    try {
        (void)cell_renumbering_metis_nested_dissection_2d(mesh);
        FAIL() << "Expected METIS-ND to report an unavailable optional backend.";
    } catch(const vmm::error::MeshException& exception) {
        EXPECT_EQ(exception.id(), vmm::error::builtin_error_id::unsupported_operation);
    }
}

TEST(CellRenumbering2DTest, CustomRegistryCanAddMethodsWithoutChangingTraits)
{
    const auto mesh = make_scrambled_four_square_mesh();
    constexpr CellRenumberingMethod2D custom_method{"custom-reverse"};
    const std::array algorithms{CellRenumberingAlgorithm2D{custom_method, custom_reverse_ordering}};

    const auto permutation = compute_cell_renumbering_2d(mesh, custom_method, algorithms);

    expect_valid_permutation(permutation, mesh.cell_count());
    EXPECT_EQ(permutation.old_id_for(CellId{0U}), CellId{3U});
    EXPECT_EQ(permutation.old_id_for(CellId{3U}), CellId{0U});
}

TEST(CellRenumbering2DTest, RejectsInvalidMortonResolution)
{
    const auto mesh = make_scrambled_four_square_mesh();
    CellRenumbering2DOptions options;
    options.space_filling_curve_bits = 0U;

    EXPECT_THROW((void)cell_renumbering_morton_2d(mesh, options), vmm::error::MeshException);
}
