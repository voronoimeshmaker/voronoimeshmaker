//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <string_view>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshProjectionAudit2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

using namespace vmm::audit;
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
    mesh.patches.name = {"wall", "open"};
    mesh.patches.type = {BoundaryPatchTypeTraits::Wall, BoundaryPatchTypeTraits::OpenBoundary};

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

bool has_code(const MeshAuditReport& report, std::string_view code)
{
    return std::any_of(report.issues.begin(), report.issues.end(), [code](const MeshAuditIssue& issue) {
        return issue.code == code;
    });
}

} // namespace

TEST(MeshProjectionAudit2DTest, CleanManualMeshHasNoProjectionWarnings)
{
    const auto mesh = make_two_cell_mesh();

    const auto report = audit_mesh_projection_2d(mesh);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(report.empty());
}

TEST(MeshProjectionAudit2DTest, ReportsInternalFaceMidpointOffset)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.centre_y[1] = 0.75;
    MeshProjectionAudit2DOptions options;
    options.internal_midpoint_tolerance = 1.0e-12;

    const auto report = audit_mesh_projection_2d(mesh, options);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-PRJ-001"));
}

TEST(MeshProjectionAudit2DTest, ReportsInternalNonOrthogonality)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.normal_x[1] = 0.1;
    mesh.faces.normal_y[1] = 1.0;
    MeshProjectionAudit2DOptions options;
    options.max_non_orthogonality_degrees = 30.0;

    const auto report = audit_mesh_projection_2d(mesh, options);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-ORT-001"));
}

TEST(MeshProjectionAudit2DTest, ReportsBoundaryProjectionOffset)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.centre_x[0] = 0.75;
    MeshProjectionAudit2DOptions options;
    options.boundary_projection_tolerance = 1.0e-12;

    const auto report = audit_mesh_projection_2d(mesh, options);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-PRJ-002"));
}

TEST(MeshProjectionAudit2DTest, StopsAfterFatalBaseAudit)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.owner[0] = CellId{99U};

    const auto report = audit_mesh_projection_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-006"));
    EXPECT_FALSE(has_code(report, "VMM-PRJ-002"));
}

TEST(MeshProjectionAudit2DTest, CanSkipBaseAuditsForDiagnosticUse)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.centre_x[0] = 0.75;
    MeshProjectionAudit2DOptions options;
    options.run_base_audits = false;
    options.boundary_projection_tolerance = 1.0e-12;

    const auto report = audit_mesh_projection_2d(mesh, options);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-PRJ-002"));
}

TEST(MeshProjectionAudit2DTest, RejectsInvalidOptions)
{
    const auto mesh = make_two_cell_mesh();

    MeshProjectionAudit2DOptions negative_midpoint;
    negative_midpoint.internal_midpoint_tolerance = -1.0;
    EXPECT_THROW((void)audit_mesh_projection_2d(mesh, negative_midpoint), vmm::error::MeshException);

    MeshProjectionAudit2DOptions negative_boundary;
    negative_boundary.boundary_projection_tolerance = -1.0;
    EXPECT_THROW((void)audit_mesh_projection_2d(mesh, negative_boundary), vmm::error::MeshException);

    MeshProjectionAudit2DOptions invalid_angle;
    invalid_angle.max_non_orthogonality_degrees = 91.0;
    EXPECT_THROW((void)audit_mesh_projection_2d(mesh, invalid_angle), vmm::error::MeshException);
}
