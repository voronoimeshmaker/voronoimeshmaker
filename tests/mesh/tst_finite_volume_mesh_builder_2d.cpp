//==============================================================================
// c++ includes
//==============================================================================
#include <sstream>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/audit/MeshTextAuditWriter2D.hpp>
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::mesh;

namespace {

PlanarCell2D make_cell(std::vector<Point2D> vertices, Point2D site, GeneratorID generator_id)
{
    return PlanarCell2D{Polygon2D{std::move(vertices)}, site, generator_id};
}

std::vector<PlanarCell2D> make_two_square_cells()
{
    return {
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 1U),
    };
}

std::vector<PlanarCell2D> make_square_domain_with_square_island_cells()
{
    return {
        make_cell({{0.0, 0.0}, {3.0, 0.0}, {3.0, 1.0}, {2.0, 1.0}, {1.0, 1.0}, {0.0, 1.0}}, {1.5, 0.5}, 0U),
        make_cell({{2.0, 1.0}, {3.0, 1.0}, {3.0, 2.0}, {2.0, 2.0}}, {2.5, 1.5}, 1U),
        make_cell({{0.0, 2.0}, {1.0, 2.0}, {2.0, 2.0}, {3.0, 2.0}, {3.0, 3.0}, {0.0, 3.0}}, {1.5, 2.5}, 2U),
        make_cell({{0.0, 1.0}, {1.0, 1.0}, {1.0, 2.0}, {0.0, 2.0}}, {0.5, 1.5}, 3U),
    };
}

std::size_t count_faces_with_patch_type(const FiniteVolumeMesh2D& mesh, BoundaryPatchType patch_type)
{
    std::size_t count{};
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto patch_id = mesh.faces.patch_id[face];
        if(!is_valid(patch_id)) {
            continue;
        }
        const auto patch_index = static_cast<std::size_t>(patch_id.value);
        if(mesh.patches.type[patch_index] == patch_type) {
            ++count;
        }
    }
    return count;
}

std::size_t count_faces_with_patch_id(const FiniteVolumeMesh2D& mesh, BoundaryPatchId patch_id)
{
    std::size_t count{};
    for(const auto face_patch_id : mesh.faces.patch_id) {
        count += face_patch_id == patch_id ? 1U : 0U;
    }
    return count;
}

} // namespace

TEST(FiniteVolumeMeshBuilder2DTest, SingleSquareBuildsBoundaryFaces)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};

    const auto mesh = build_finite_volume_mesh_2d(cells);

    EXPECT_EQ(mesh.node_count(), 4U);
    EXPECT_EQ(mesh.cell_count(), 1U);
    EXPECT_EQ(mesh.face_count(), 4U);
    EXPECT_EQ(mesh.patch_count(), 1U);
    EXPECT_EQ(mesh.cell_faces.size(), 4U);

    const auto report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    EXPECT_TRUE(report.valid());
}

TEST(FiniteVolumeMeshBuilder2DTest, AdjacentSquaresShareOneInternalFace)
{
    const auto cells = make_two_square_cells();

    const auto mesh = build_finite_volume_mesh_2d(cells);

    EXPECT_EQ(mesh.node_count(), 6U);
    EXPECT_EQ(mesh.cell_count(), 2U);
    EXPECT_EQ(mesh.face_count(), 7U);
    EXPECT_EQ(mesh.patch_count(), 1U);
    EXPECT_EQ(mesh.cell_faces.size(), 8U);

    std::size_t internal_faces{};
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        if(is_valid(mesh.faces.neighbour[face])) {
            ++internal_faces;
            EXPECT_FALSE(is_valid(mesh.faces.patch_id[face]));
            EXPECT_EQ(mesh.faces.owner[face], CellId{0U});
            EXPECT_EQ(mesh.faces.neighbour[face], CellId{1U});
        }
    }
    EXPECT_EQ(internal_faces, 1U);

    const auto report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    EXPECT_TRUE(report.valid());
}

TEST(FiniteVolumeMeshBuilder2DTest, ClockwiseCellsStillProduceOutwardBoundaryNormals)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}}, {0.5, 0.5}, 0U)};

    const auto mesh = build_finite_volume_mesh_2d(cells);

    EXPECT_EQ(mesh.node_count(), 4U);
    EXPECT_EQ(mesh.face_count(), 4U);

    const auto report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    EXPECT_TRUE(report.valid());
}

TEST(FiniteVolumeMeshBuilder2DTest, RejectsEdgesSharedByMoreThanTwoCells)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 1U),
        make_cell({{1.0, 0.0}, {1.0, 1.0}, {0.5, 0.5}}, {0.8, 0.5}, 2U),
    };

    EXPECT_THROW((void)build_finite_volume_mesh_2d(cells), vmm::error::MeshException);
}

TEST(FiniteVolumeMeshBuilder2DTest, SharedNodesAreDeduplicatedWithTolerance)
{
    std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0 + 1.0e-10, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0 + 1.0e-10, 1.0}},
                  {1.5, 0.5},
                  1U),
    };
    FiniteVolumeMeshBuilder2DOptions options;
    options.vertex_tolerance = 1.0e-9;

    const auto mesh = build_finite_volume_mesh_2d(cells, options);

    EXPECT_EQ(mesh.node_count(), 6U);
    EXPECT_EQ(mesh.face_count(), 7U);
}

TEST(FiniteVolumeMeshBuilder2DTest, BoundaryRingsClassifyOuterBoundaryAndIsland)
{
    const auto cells = make_square_domain_with_square_island_cells();

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "unclassified-boundary";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    options.boundary_patches = {
        BoundaryPatchRing2D{Polygon2D{{{0.0, 0.0}, {3.0, 0.0}, {3.0, 3.0}, {0.0, 3.0}}},
                            "outer-land",
                            BoundaryPatchTypeTraits::LandBoundary},
        BoundaryPatchRing2D{Polygon2D{{{1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}, {1.0, 2.0}}},
                            "island",
                            BoundaryPatchTypeTraits::Island},
    };

    const auto mesh = build_finite_volume_mesh_2d(cells, options);

    ASSERT_EQ(mesh.patch_count(), 3U);
    EXPECT_EQ(mesh.patches.name[0], "unclassified-boundary");
    EXPECT_EQ(mesh.patches.name[1], "outer-land");
    EXPECT_EQ(mesh.patches.name[2], "island");
    EXPECT_EQ(mesh.patches.type[1], BoundaryPatchTypeTraits::LandBoundary);
    EXPECT_EQ(mesh.patches.type[2], BoundaryPatchTypeTraits::Island);
    EXPECT_EQ(count_faces_with_patch_id(mesh, BoundaryPatchId{0U}), 0U);
    EXPECT_GT(count_faces_with_patch_type(mesh, BoundaryPatchTypeTraits::LandBoundary), 0U);
    EXPECT_GT(count_faces_with_patch_type(mesh, BoundaryPatchTypeTraits::Island), 0U);

    const auto patch_report = vmm::audit::audit_mesh_patches_2d(mesh);
    EXPECT_TRUE(patch_report.valid());
}

TEST(FiniteVolumeMeshBuilder2DTest, UnmatchedBoundaryFacesKeepDefaultPatch)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "fallback-wall";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    options.boundary_patches = {
        BoundaryPatchRing2D{Polygon2D{{{2.0, 2.0}, {3.0, 2.0}, {3.0, 3.0}, {2.0, 3.0}}},
                            "far-open",
                            BoundaryPatchTypeTraits::OpenBoundary},
    };

    const auto mesh = build_finite_volume_mesh_2d(cells, options);

    EXPECT_EQ(mesh.patch_count(), 2U);
    EXPECT_EQ(count_faces_with_patch_id(mesh, BoundaryPatchId{0U}), 4U);
    EXPECT_EQ(count_faces_with_patch_id(mesh, BoundaryPatchId{1U}), 0U);
}

TEST(FiniteVolumeMeshBuilder2DTest, RejectsNegativeBoundaryTolerance)
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U)};
    FiniteVolumeMeshBuilder2DOptions options;
    options.boundary_tolerance = -1.0;

    EXPECT_THROW((void)build_finite_volume_mesh_2d(cells, options), vmm::error::MeshException);
}

TEST(FiniteVolumeMeshBuilder2DTest, TextAuditCanWriteReconstructedMesh)
{
    const auto cells = make_two_square_cells();
    const auto mesh = build_finite_volume_mesh_2d(cells);
    std::ostringstream output;

    vmm::audit::write_mesh_text_audit_2d(output, mesh);

    EXPECT_NE(output.str().find("valid: true\n"), std::string::npos);
    EXPECT_NE(output.str().find("faces: 7\n"), std::string::npos);
    EXPECT_NE(output.str().find("[cell_faces]\n"), std::string::npos);
}
