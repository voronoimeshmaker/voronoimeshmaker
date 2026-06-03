//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file tst_geometries_2d.cpp
// @brief Tests for 2D polygonal domain geometry.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/Ellipse2D.hpp>
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/domain/Triangle2D.hpp>

#include <gtest/gtest.h>

#include <limits>
#include <vector>

namespace {

TEST(Polygon2DTest, ComputesAreaAndBounds)
{
    const vmm::domain::Polygon2D polygon{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {0.0, 1.0}
    }};

    EXPECT_DOUBLE_EQ(polygon.area(), 2.0);
    const auto bounds = polygon.bounds();
    EXPECT_DOUBLE_EQ(bounds.min.x, 0.0);
    EXPECT_DOUBLE_EQ(bounds.max.y, 1.0);
}

TEST(Polygon2DTest, RejectsDegeneratePolygon)
{
    EXPECT_THROW(static_cast<void>(vmm::domain::Polygon2D{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}
    }}), vmm::error::MeshException);
}

TEST(Polygon2DTest, RejectsSelfIntersection)
{
    EXPECT_THROW(static_cast<void>(vmm::domain::Polygon2D{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {2.0, 2.0}, {0.0, 2.0}, {2.0, 0.0}
    }}), vmm::error::MeshException);
}

TEST(Polygon2DTest, RejectsNonFiniteCoordinate)
{
    EXPECT_THROW(static_cast<void>(vmm::domain::Polygon2D{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {1.0, 0.0}, {std::numeric_limits<vmm::core::Real>::infinity(), 1.0}
    }}), vmm::error::MeshException);
}

TEST(PolygonalDomain2DTest, SubtractsHoleArea)
{
    const vmm::domain::Polygon2D exterior{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}
    }};
    const vmm::domain::Polygon2D hole{std::vector<vmm::domain::Point2D>{
        {1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}, {1.0, 2.0}
    }};
    const vmm::domain::PolygonalDomain2D domain{exterior, {hole}};

    EXPECT_DOUBLE_EQ(domain.area(), 15.0);
}

TEST(Geometries2DTest, RectangleTriangleAndEllipseCreateDomains)
{
    EXPECT_DOUBLE_EQ(vmm::domain::make_rectangle_2d(4.0, 2.0).area(), 8.0);
    EXPECT_DOUBLE_EQ(vmm::domain::make_triangle_2d({0.0, 0.0}, {2.0, 0.0}, {0.0, 2.0}).area(), 2.0);
    EXPECT_GT(vmm::domain::make_ellipse_2d(2.0, 1.0, {}, 64U).area(), 6.0);
}

} // namespace
