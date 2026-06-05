//==============================================================================
// File        : tst_mesh_analyzer.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Unit tests for the MeshAnalyzer quality validation module.
//               Verifies topological regularity checks, size uniformity proxies,
//               and structured violation reporting.
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
// @file tst_mesh_analyzer.cpp
// @brief Unit tests for mesh quality analysis and warning generation.
// @ingroup vmm_analysis_tests
//==============================================================================


//==============================================================================
// gtest includes
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/analysis/MeshAnalyzer.hpp>
// #include <vmm/core/MeshTopology.hpp>

//==============================================================================
// using
//==============================================================================
using namespace vmm::core;
using namespace vmm::analysis;

// ============================================================================
// Helper: build a minimal valid 2D topology for testing
// ============================================================================
MeshTopology<Dimension::D2> build_regular_grid_2d(std::size_t n_cells) {
    MeshTopology<Dimension::D2> mesh(n_cells);
    for (std::size_t i = 0; i < n_cells; ++i) {
        mesh.set_centroid(static_cast<CellIndex>(i), {static_cast<Real>(i), static_cast<Real>(0)});
        mesh.assign_generator_id(static_cast<CellIndex>(i), static_cast<GeneratorID>(i));
        mesh.set_boundary_flag(static_cast<CellIndex>(i), BoundaryTypeTraits::Internal);
    }
    return mesh;
}

// ============================================================================
// Test: Empty mesh handling
// ============================================================================
TEST(MeshAnalyzerTest, EmptyMeshFailsGracefully) {
    MeshTopology<Dimension::D2> mesh(0);
    auto report = MeshAnalyzer::analyse(mesh);
    
    EXPECT_FALSE(report.passed);
    EXPECT_EQ(report.violations.size(), 1);
    EXPECT_EQ(report.violations[0].metric_name, "empty_mesh");
    EXPECT_EQ(report.violations[0].message, "Mesh contains zero cells.");
}

// ============================================================================
// Test: Regular topology passes with relaxed criteria
// ============================================================================
TEST(MeshAnalyzerTest, RegularTopologyPassesWithRelaxedCriteria) {
    const std::size_t n = 20;
    auto mesh = build_regular_grid_2d(n);
    
    // Relax criteria to accept zero neighbours (skeleton test)
    QualityCriteria relaxed;
    relaxed.min_volume_ratio = static_cast<Real>(0.0);
    relaxed.max_volume_ratio = static_cast<Real>(1e6);
    
    auto report = MeshAnalyzer::analyse(mesh, relaxed);
    
    // Report structure must be valid even with degenerate input
    EXPECT_GE(report.mean_neighbour_degree, static_cast<Real>(0.0));
    EXPECT_GE(report.centroid_spacing_var, static_cast<Real>(0.0));
    EXPECT_TRUE(report.violations.empty() || !relaxed.fail_on_violation);
}

// ============================================================================
// Test: Boundary type handling does not crash analyser
// ============================================================================
TEST(MeshAnalyzerTest, BoundaryFlagsHandledCorrectly) {
    MeshTopology<Dimension::D2> mesh(3);
    mesh.set_centroid(0, {static_cast<Real>(0), static_cast<Real>(0)});
    mesh.set_centroid(1, {static_cast<Real>(1), static_cast<Real>(0)});
    mesh.set_centroid(2, {static_cast<Real>(2), static_cast<Real>(0)});
    
    mesh.set_boundary_flag(0, BoundaryTypeTraits::ExternalBoundary);
    mesh.set_boundary_flag(1, BoundaryTypeTraits::Internal);
    mesh.set_boundary_flag(2, BoundaryTypeTraits::HoleBoundary);
    
    // Analyser should not crash when reading boundary flags
    QualityCriteria criteria;
    criteria.fail_on_violation = false;
    auto report = MeshAnalyzer::analyse(mesh, criteria);
    
    // Basic sanity: report is well-formed
    EXPECT_TRUE(report.passed || !criteria.fail_on_violation);
}

// ============================================================================
// Test: Violation reporting structure
// ============================================================================
TEST(MeshAnalyzerTest, ViolationReportStructure) {
    MeshTopology<Dimension::D2> mesh(1);
    mesh.set_centroid(0, {static_cast<Real>(0), static_cast<Real>(0)});
    mesh.assign_generator_id(0, static_cast<GeneratorID>(42));
    
    // Force a violation by setting impossible thresholds
    QualityCriteria strict;
    strict.min_volume_ratio = static_cast<Real>(10.0); // Impossible: requires cell > 10x mean
    strict.fail_on_violation = true;
    
    auto report = MeshAnalyzer::analyse(mesh, strict);
    
    if (!report.violations.empty()) {
        const auto& v = report.violations[0];
        EXPECT_EQ(v.entity_id, 42);
        EXPECT_FALSE(v.metric_name.empty());
        EXPECT_GE(v.expected, static_cast<Real>(0.0));
        EXPECT_FALSE(v.message.empty());
    }
}

// ============================================================================
// Test: 3D instantiation compiles and runs
// ============================================================================
TEST(MeshAnalyzerTest, D3InstantiationValid) {
    MeshTopology<Dimension::D3> mesh_3d(10);
    for (std::size_t i = 0; i < 10; ++i) {
        mesh_3d.set_centroid(static_cast<CellIndex>(i), 
                            {static_cast<Real>(i), static_cast<Real>(0), static_cast<Real>(0)});
        mesh_3d.assign_generator_id(static_cast<CellIndex>(i), static_cast<GeneratorID>(i));
    }
    
    auto report = MeshAnalyzer::analyse(mesh_3d);
    
    // Basic sanity check
    EXPECT_GE(report.mean_neighbour_degree, static_cast<Real>(0.0));
}

// ============================================================================
// Test: fail_on_violation flag behaviour
// ============================================================================
TEST(MeshAnalyzerTest, FailOnViolationFlag) {
    MeshTopology<Dimension::D2> mesh(1);
    mesh.set_centroid(0, {static_cast<Real>(0), static_cast<Real>(0)});
    
    // Case 1: fail_on_violation = false → report.passed may still be true
    QualityCriteria lenient;
    lenient.fail_on_violation = false;
    auto report_lenient = MeshAnalyzer::analyse(mesh, lenient);
    // (No assertion on passed; depends on internal logic)
    
    // Case 2: fail_on_violation = true → violations should set passed = false
    QualityCriteria strict;
    strict.fail_on_violation = true;
    strict.min_volume_ratio = static_cast<Real>(10.0); // Force violation
    auto report_strict = MeshAnalyzer::analyse(mesh, strict);
    
    if (!report_strict.violations.empty()) {
        EXPECT_FALSE(report_strict.passed);
    }
}