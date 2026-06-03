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
// @file tst_transform_2d.cpp
// @brief Tests for 2D polygon transformations.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/domain/Transform2D.hpp>

#include <gtest/gtest.h>

#include <vector>

namespace {

TEST(Transform2DTest, TranslatesPolygon)
{
    const vmm::domain::Polygon2D polygon{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}
    }};
    const auto moved = polygon.translated(2.0, 3.0);

    EXPECT_DOUBLE_EQ(moved.vertices()[0].x, 2.0);
    EXPECT_DOUBLE_EQ(moved.vertices()[0].y, 3.0);
}

TEST(Transform2DTest, AppliesRigidTransform)
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const vmm::domain::Transform2D transform{1.0, 2.0, 0.0};
    const auto moved = transform.apply(domain);

    const auto bounds = moved.bounds();
    EXPECT_DOUBLE_EQ(bounds.min.x, 0.0);
    EXPECT_DOUBLE_EQ(bounds.min.y, 1.0);
}

} // namespace
