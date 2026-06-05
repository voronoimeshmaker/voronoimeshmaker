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
// @file tst_planar_cell_2d.cpp
// @brief Tests for rich planar 2D cells.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/PlanarCell2D.hpp>

#include <gtest/gtest.h>

#include <vector>

namespace {

TEST(PlanarCell2DTest, StoresGeometryAndGeneratorMetadata)
{
    const vmm::domain::Polygon2D exterior{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {2.0, 0.0}, {2.0, 2.0}, {0.0, 2.0}
    }};

    const vmm::domain::PlanarCell2D cell{
        exterior,
        {1.0, 1.0},
        42U,
        vmm::core::BoundaryTypeTraits::ExternalBoundary
    };

    EXPECT_DOUBLE_EQ(cell.area(), 4.0);
    EXPECT_DOUBLE_EQ(cell.centroid().x, 1.0);
    EXPECT_DOUBLE_EQ(cell.centroid().y, 1.0);
    EXPECT_EQ(cell.generator_id(), 42U);
    EXPECT_EQ(cell.boundary_type(), vmm::core::BoundaryTypeTraits::ExternalBoundary);
    EXPECT_FALSE(cell.has_holes());
}

TEST(PlanarCell2DTest, SubtractsHoleArea)
{
    const vmm::domain::Polygon2D exterior{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}
    }};
    const vmm::domain::Polygon2D hole{std::vector<vmm::domain::Point2D>{
        {1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}, {1.0, 2.0}
    }};

    const vmm::domain::PlanarCell2D cell{
        exterior,
        std::vector<vmm::domain::Polygon2D>{hole},
        {0.0, 0.0},
        7U
    };

    EXPECT_TRUE(cell.has_holes());
    EXPECT_DOUBLE_EQ(cell.area(), 15.0);
}

} // namespace
