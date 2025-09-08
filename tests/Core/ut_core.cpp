//==============================================================================
// Name        : ut_core.cpp
// Purpose     : Smoke tests for Core headers (namespace/type/constants).
//               Verifies API presence and a few invariants.
//==============================================================================

#include <gtest/gtest.h>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/Core/type.h>
#include <VoronoiMeshMaker/Core/constants.h>

#include <type_traits>
#include <numbers>

TEST(CoreConstants, BasicRanges) {
    // Real must be floating-point-like
    EXPECT_TRUE(std::is_floating_point_v<vmm::Real>);

    // ZERO should be tiny but positive; EPSILON > ZERO
    EXPECT_GT(vmm::ZERO, static_cast<vmm::Real>(0));
    EXPECT_GT(vmm::EPSILON, vmm::ZERO);

    // PI close to std::numbers::pi
    auto ref_pi = static_cast<vmm::Real>(std::numbers::pi_v<long double>);
    EXPECT_NEAR(static_cast<double>(vmm::PI),
                static_cast<double>(ref_pi), 1e-12);
}

TEST(CoreTypes, CanInstantiateBasicCGALTypes) {
    // Points should be constructible
    vmm::gtp::Point2D p2{0.0, 0.0};
    vmm::gtp::Point3D p3{0.0, 0.0, 0.0};

    // Polygon_2 should accept push_back and report size
    vmm::gtp::Polygon2D poly;
    poly.push_back(vmm::gtp::Point2D{0, 0});
    poly.push_back(vmm::gtp::Point2D{1, 0});
    poly.push_back(vmm::gtp::Point2D{0, 1});
    EXPECT_EQ(poly.size(), 3u);
    
}
