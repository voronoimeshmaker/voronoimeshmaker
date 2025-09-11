//==============================================================================
// Name        : ut_Core.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Unit tests for Core module (types and constants).
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//==============================================================================

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/Core/constants.h>

//------------------------------------------------------------------------------
// Basic sanity checks for fundamental constants
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Minimal usage checks for core geometric types
//------------------------------------------------------------------------------
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
