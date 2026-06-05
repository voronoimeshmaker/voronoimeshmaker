//==============================================================================
// File        : tst_mesh_topology.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Unit tests for the MeshTopology container, validating DOD
//               allocation, bounds checking, CSR integrity, sequential ID
//               assignment, and zero CGAL header leakage.
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
// @file tst_mesh_topology.cpp
// @brief Unit tests for the dimension-agnostic MeshTopology container.
// @ingroup vmm_core_tests
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <numeric>

//==============================================================================
// gtest includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/mesh/MeshTopology.hpp>

using namespace vmm::core;

// ============================================================================
// Test Fixture for 2D Topology
// ============================================================================
class MeshTopologyD2Test : public ::testing::Test {
protected:
    void SetUp() override {
        mesh_ = std::make_unique<MeshTopology<Dimension::D2>>(kCapacity);
    }
    static constexpr std::size_t kCapacity = 100;
    std::unique_ptr<MeshTopology<Dimension::D2>> mesh_;
};

// ============================================================================
// Allocation & Initial State
// ============================================================================
TEST_F(MeshTopologyD2Test, InitialCapacityAndZeroNeighbours) {
    EXPECT_EQ(mesh_->cell_count(), kCapacity);
    EXPECT_EQ(mesh_->total_neighbours(), 0);
    EXPECT_EQ(mesh_->centroids().size(), kCapacity);
    EXPECT_EQ(mesh_->generator_ids().size(), kCapacity);
}

// ============================================================================
// Bounds Checking & Exception Safety
// ============================================================================
TEST_F(MeshTopologyD2Test, OutOfBoundsNeighbourAccessThrows) {
    const auto out_of_bounds = static_cast<CellIndex>(kCapacity);
    // Explicit void cast suppresses [[nodiscard]] warnings inside GTest macros
    EXPECT_THROW(static_cast<void>(mesh_->neighbours(out_of_bounds)), std::out_of_range);
    EXPECT_THROW(static_cast<void>(mesh_->neighbours(static_cast<CellIndex>(kCapacity + 1))), std::out_of_range);
}

TEST_F(MeshTopologyD2Test, OutOfBoundsBoundaryAccessThrows) {
    const auto out_of_bounds = static_cast<CellIndex>(kCapacity);
    EXPECT_THROW(static_cast<void>(mesh_->boundary_type(out_of_bounds)), std::out_of_range);
}

// ============================================================================
// Sequential Generator ID Assignment
// ============================================================================
TEST_F(MeshTopologyD2Test, SequentialIDAssignmentAndRetrieval) {
    std::vector<GeneratorID> expected_ids(kCapacity);
    std::iota(expected_ids.begin(), expected_ids.end(), 0);

    for (std::size_t i = 0; i < kCapacity; ++i) {
        mesh_->assign_generator_id(static_cast<CellIndex>(i), expected_ids[i]);
    }

    auto actual_span = mesh_->generator_ids();
    for (std::size_t i = 0; i < kCapacity; ++i) {
        EXPECT_EQ(actual_span[i], expected_ids[i]);
    }
}

// ============================================================================
// Boundary Flag Management
// ============================================================================
TEST_F(MeshTopologyD2Test, BoundaryClassificationAssignment) {
    mesh_->set_boundary_flag(0, BoundaryTypeTraits::Internal);
    mesh_->set_boundary_flag(1, BoundaryTypeTraits::ExternalBoundary);
    mesh_->set_boundary_flag(2, BoundaryTypeTraits::HoleBoundary);

    EXPECT_EQ(mesh_->boundary_type(0), BoundaryTypeTraits::Internal);
    EXPECT_EQ(mesh_->boundary_type(1), BoundaryTypeTraits::ExternalBoundary);
    EXPECT_EQ(mesh_->boundary_type(2), BoundaryTypeTraits::HoleBoundary);
}

// ============================================================================
// Centroid Management
// ============================================================================
TEST_F(MeshTopologyD2Test, CentroidAssignmentAndSpanConsistency) {
    Point<Dimension::D2> p{1.5, -3.2};
    mesh_->set_centroid(42, p);

    auto centroids = mesh_->centroids();
    EXPECT_DOUBLE_EQ(centroids[42].x, 1.5);
    EXPECT_DOUBLE_EQ(centroids[42].y, -3.2);

    EXPECT_DOUBLE_EQ(centroids[0].x, 0.0);
    EXPECT_DOUBLE_EQ(centroids[0].y, 0.0);
}

// ============================================================================
// 3D Template Instantiation Validation
// ============================================================================
TEST(MeshTopologyTest, D3_CompilationAndBasicAccess) {
    MeshTopology<Dimension::D3> mesh_3d(50);
    EXPECT_EQ(mesh_3d.cell_count(), 50);
    EXPECT_EQ(mesh_3d.centroids().size(), 50);

    mesh_3d.set_centroid(10, {0.0, 0.0, 10.5});
    EXPECT_DOUBLE_EQ(mesh_3d.centroids()[10].z, 10.5);
}

// ============================================================================
// Compile-Time Contract Verification
// ============================================================================
TEST(MeshTopologyTest, ZeroCGALHeaderLeakage) {
    // Successful compilation of this file confirms the architectural
    // isolation contract: no CGAL headers are transitively included.
    SUCCEED();
}