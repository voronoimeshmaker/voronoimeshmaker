//==============================================================================
// c++ includes
//==============================================================================
#include <string>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::mesh;

namespace {

PlanarCell2D make_cell(std::vector<Point2D> vertices, Point2D site, GeneratorID generator_id)
{
    return PlanarCell2D{Polygon2D{std::move(vertices)}, site, generator_id};
}

FiniteVolumeMesh2D make_valid_mesh()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "wall";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    return build_finite_volume_mesh_2d(cells, options);
}

bool has_issue_code(const vmm::audit::MeshAuditReport& report, const std::string& code)
{
    for(const auto& issue : report.all_issues()) {
        if(issue.code == code) {
            return true;
        }
    }
    return false;
}

FiniteVolumeMesh2D make_mesh_with_two_patches()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "wall";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    options.boundary_patches = {
        BoundaryPatchRing2D{Polygon2D{{{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}},
                            "outer-land",
                            BoundaryPatchTypeTraits::LandBoundary}};
    return build_finite_volume_mesh_2d(cells, options);
}

} // namespace

TEST(MeshPatchAudit2DTest, ValidBuilderMeshPasses)
{
    const auto mesh = make_valid_mesh();

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(report.empty());
}

TEST(MeshPatchAudit2DTest, RejectsBoundaryFaceWithInternalPatchType)
{
    auto mesh = make_valid_mesh();
    mesh.patches.type[0] = BoundaryPatchTypeTraits::Internal;

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_issue_code(report, "VMM-PAT-004"));
}

TEST(MeshPatchAudit2DTest, RejectsEmptyPatchName)
{
    auto mesh = make_valid_mesh();
    mesh.patches.name[0].clear();

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_issue_code(report, "VMM-PAT-002"));
}

TEST(MeshPatchAudit2DTest, RejectsEmptyPatchTypeName)
{
    auto mesh = make_valid_mesh();
    mesh.patches.type[0] = BoundaryPatchType{"", 99U};

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_issue_code(report, "VMM-PAT-003"));
}

TEST(MeshPatchAudit2DTest, RejectsPatchIdThatDoesNotMatchRow)
{
    auto mesh = make_valid_mesh();
    mesh.patches.id[0] = BoundaryPatchId{99U};

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_issue_code(report, "VMM-IDX-005"));
}

TEST(MeshPatchAudit2DTest, RejectsPatchIdThatIsValidButInTheWrongRow)
{
    auto mesh = make_mesh_with_two_patches();
    ASSERT_EQ(mesh.patch_count(), 2U);
    mesh.patches.id[0] = BoundaryPatchId{1U};
    mesh.patches.id[1] = BoundaryPatchId{0U};

    const auto report = vmm::audit::audit_mesh_patches_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_issue_code(report, "VMM-PAT-001"));
}
