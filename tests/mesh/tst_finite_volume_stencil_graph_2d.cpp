//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/MeshException.hpp>
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

FiniteVolumeMesh2D make_two_square_mesh()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 1U),
    };

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "fallback-wall";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    options.boundary_patches = {
        BoundaryPatchRing2D{Polygon2D{{{0.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {0.0, 1.0}}},
                            "outer-land",
                            BoundaryPatchTypeTraits::LandBoundary}};
    return build_finite_volume_mesh_2d(cells, options);
}

std::size_t boundary_entry_for_face(const FiniteVolumeStencilGraph2D& graph, FaceId face_id)
{
    for(std::size_t entry = 0U; entry < graph.boundary.size(); ++entry) {
        if(graph.boundary.face_id[entry] == face_id) {
            return entry;
        }
    }
    return graph.boundary.size();
}

} // namespace

TEST(FiniteVolumeStencilGraph2DTest, BuildsInternalAndBoundaryStencilTables)
{
    const auto mesh = make_two_square_mesh();
    const auto graph = build_finite_volume_stencil_graph_2d(mesh);

    EXPECT_TRUE(graph.has_consistent_storage());
    EXPECT_EQ(graph.internal_count(), 1U);
    EXPECT_EQ(graph.boundary_count(), 6U);

    ASSERT_EQ(graph.internal.size(), 1U);
    EXPECT_EQ(graph.internal.owner[0], CellId{0U});
    EXPECT_EQ(graph.internal.neighbour[0], CellId{1U});
    EXPECT_DOUBLE_EQ(graph.internal.owner_to_neighbour_x[0], 1.0);
    EXPECT_DOUBLE_EQ(graph.internal.owner_to_neighbour_y[0], 0.0);
    EXPECT_DOUBLE_EQ(graph.internal.centre_distance[0], 1.0);
    EXPECT_DOUBLE_EQ(graph.internal.face_length[0], 1.0);
}

TEST(FiniteVolumeStencilGraph2DTest, BoundaryEntriesPreservePatchAndNormalData)
{
    const auto mesh = make_two_square_mesh();
    const auto graph = build_finite_volume_stencil_graph_2d(mesh);

    ASSERT_EQ(graph.boundary_count(), 6U);

    std::size_t checked_entries{};
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        if(is_valid(mesh.faces.neighbour[face])) {
            continue;
        }

        const auto entry = boundary_entry_for_face(graph, FaceId{face});
        ASSERT_LT(entry, graph.boundary.size());
        EXPECT_EQ(graph.boundary.cell[entry], mesh.faces.owner[face]);
        EXPECT_EQ(graph.boundary.patch_id[entry], mesh.faces.patch_id[face]);
        EXPECT_DOUBLE_EQ(graph.boundary.normal_x[entry], mesh.faces.normal_x[face]);
        EXPECT_DOUBLE_EQ(graph.boundary.normal_y[entry], mesh.faces.normal_y[face]);
        EXPECT_DOUBLE_EQ(graph.boundary.face_length[entry], mesh.faces.length[face]);
        EXPECT_GT(graph.boundary.centre_to_face_distance[entry], 0.0);
        ++checked_entries;
    }
    EXPECT_EQ(checked_entries, graph.boundary_count());
}

TEST(FiniteVolumeStencilGraph2DTest, SingleCellMeshHasOnlyBoundaryStencils)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};
    const auto mesh = build_finite_volume_mesh_2d(cells);

    const auto graph = build_finite_volume_stencil_graph_2d(mesh);

    EXPECT_TRUE(graph.internal.empty());
    EXPECT_EQ(graph.boundary_count(), 4U);
}

TEST(FiniteVolumeStencilGraph2DTest, RejectsInvalidMeshBeforeBuildingGraph)
{
    auto mesh = make_two_square_mesh();
    mesh.faces.length[0] = 0.0;

    EXPECT_THROW((void)build_finite_volume_stencil_graph_2d(mesh), vmm::error::MeshException);
}

TEST(FiniteVolumeStencilGraph2DTest, CanBypassAuditGateForDiagnosticUse)
{
    auto mesh = make_two_square_mesh();
    mesh.faces.length[0] = 0.0;
    FiniteVolumeStencilGraph2DOptions options;
    options.require_valid_mesh = false;

    const auto graph = build_finite_volume_stencil_graph_2d(mesh, options);

    EXPECT_EQ(graph.internal_count(), 1U);
    EXPECT_EQ(graph.boundary_count(), 6U);
}
