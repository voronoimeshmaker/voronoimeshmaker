#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace vmm::core;
using namespace vmm::mesh;

namespace {

FiniteVolumeMesh2D make_two_cell_mesh()
{
    FiniteVolumeMesh2D mesh;

    mesh.nodes.x = {0.0, 1.0, 2.0, 0.0, 1.0, 2.0};
    mesh.nodes.y = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};

    mesh.cells.centre_x = {0.5, 1.5};
    mesh.cells.centre_y = {0.5, 0.5};
    mesh.cells.area = {1.0, 1.0};
    mesh.cells.site_id = {SiteId{0U}, SiteId{1U}};
    mesh.cells.face_offset = {0U, 4U};
    mesh.cells.face_count = {4U, 4U};

    mesh.patches.id = {BoundaryPatchId{0U}, BoundaryPatchId{1U}};
    mesh.patches.name = {"Wall", "open"};
    mesh.patches.type = {BoundaryPatchType::Wall, BoundaryPatchType::OpenBoundary};

    mesh.faces.owner = {
        CellId{0U}, CellId{0U}, CellId{0U}, CellId{0U}, CellId{1U}, CellId{1U}, CellId{1U}};
    mesh.faces.neighbour = {
        CellId::invalid(), CellId{1U}, CellId::invalid(), CellId::invalid(), CellId::invalid(), CellId::invalid(),
        CellId::invalid()};
    mesh.faces.centre_x = {0.5, 1.0, 0.5, 0.0, 1.5, 2.0, 1.5};
    mesh.faces.centre_y = {0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0};
    mesh.faces.normal_x = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, 0.0};
    mesh.faces.normal_y = {-1.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0};
    mesh.faces.length = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    mesh.faces.patch_id = {
        BoundaryPatchId{0U}, BoundaryPatchId::invalid(), BoundaryPatchId{0U}, BoundaryPatchId{0U},
        BoundaryPatchId{0U}, BoundaryPatchId{1U}, BoundaryPatchId{0U}};
    mesh.faces.node0 = {NodeId{0U}, NodeId{1U}, NodeId{4U}, NodeId{3U}, NodeId{1U}, NodeId{2U}, NodeId{5U}};
    mesh.faces.node1 = {NodeId{1U}, NodeId{4U}, NodeId{3U}, NodeId{0U}, NodeId{2U}, NodeId{5U}, NodeId{4U}};

    mesh.cell_faces.face_ids = {
        FaceId{0U}, FaceId{1U}, FaceId{2U}, FaceId{3U}, FaceId{1U}, FaceId{4U}, FaceId{5U}, FaceId{6U}};

    return mesh;
}

} // namespace

TEST(FiniteVolumeMesh2DTest, EmptyMeshHasZeroCounts)
{
    const FiniteVolumeMesh2D mesh;

    EXPECT_EQ(mesh.node_count(), 0U);
    EXPECT_EQ(mesh.cell_count(), 0U);
    EXPECT_EQ(mesh.face_count(), 0U);
    EXPECT_EQ(mesh.patch_count(), 0U);
    EXPECT_TRUE(mesh.nodes.empty());
    EXPECT_TRUE(mesh.cells.empty());
    EXPECT_TRUE(mesh.faces.empty());
    EXPECT_TRUE(mesh.patches.empty());
    EXPECT_TRUE(mesh.cell_faces.empty());
}

TEST(FiniteVolumeMesh2DTest, ManualTwoCellMeshStoresCanonicalTables)
{
    const auto mesh = make_two_cell_mesh();

    EXPECT_TRUE(mesh.has_consistent_storage());
    EXPECT_EQ(mesh.node_count(), 6U);
    EXPECT_EQ(mesh.cell_count(), 2U);
    EXPECT_EQ(mesh.face_count(), 7U);
    EXPECT_EQ(mesh.patch_count(), 2U);

    EXPECT_TRUE(mesh.contains(NodeId{5U}));
    EXPECT_TRUE(mesh.contains(CellId{1U}));
    EXPECT_TRUE(mesh.contains(FaceId{6U}));
    EXPECT_TRUE(mesh.contains(BoundaryPatchId{1U}));

    EXPECT_FALSE(mesh.contains(NodeId{6U}));
    EXPECT_FALSE(mesh.contains(CellId::invalid()));
    EXPECT_FALSE(mesh.contains(FaceId{7U}));
    EXPECT_FALSE(mesh.contains(BoundaryPatchId::invalid()));
}

TEST(FiniteVolumeMesh2DTest, CellFaceConnectivityReturnsStableSpans)
{
    const auto mesh = make_two_cell_mesh();

    const auto left_faces = mesh.faces_of(CellId{0U});
    const auto right_faces = mesh.faces_of(CellId{1U});

    ASSERT_EQ(left_faces.size(), 4U);
    ASSERT_EQ(right_faces.size(), 4U);

    EXPECT_EQ(left_faces[0], FaceId{0U});
    EXPECT_EQ(left_faces[1], FaceId{1U});
    EXPECT_EQ(left_faces[2], FaceId{2U});
    EXPECT_EQ(left_faces[3], FaceId{3U});

    EXPECT_EQ(right_faces[0], FaceId{1U});
    EXPECT_EQ(right_faces[1], FaceId{4U});
    EXPECT_EQ(right_faces[2], FaceId{5U});
    EXPECT_EQ(right_faces[3], FaceId{6U});
}

TEST(FiniteVolumeMesh2DTest, InvalidCellFaceRequestsThrow)
{
    const auto mesh = make_two_cell_mesh();

    EXPECT_THROW(static_cast<void>(mesh.faces_of(CellId{2U})), std::out_of_range);
    EXPECT_THROW(static_cast<void>(mesh.faces_of(CellId::invalid())), std::out_of_range);
    EXPECT_THROW(static_cast<void>(mesh.cell_faces.cell_faces(20U, 1U)), std::out_of_range);
}

TEST(FiniteVolumeMesh2DTest, InternalAndBoundarySentinelsCanBeRepresented)
{
    const auto mesh = make_two_cell_mesh();

    EXPECT_TRUE(is_valid(mesh.faces.neighbour[1]));
    EXPECT_FALSE(is_valid(mesh.faces.patch_id[1]));

    EXPECT_FALSE(is_valid(mesh.faces.neighbour[0]));
    EXPECT_TRUE(is_valid(mesh.faces.patch_id[0]));
    EXPECT_EQ(mesh.patches.type[mesh.faces.patch_id[0].value], BoundaryPatchType::Wall);
}
