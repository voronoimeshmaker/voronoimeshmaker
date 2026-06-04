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
// @file tst_tessellator_2d.cpp
// @brief Integration tests for the public 2D Voronoi tessellator.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>

#include <gtest/gtest.h>

#include <cmath>

namespace {

TEST(Tessellator2DTest, GeneratesTopologyFromHexagonalSites)
{
    const auto domain = vmm::domain::make_rectangle_2d(3.0, 2.0);
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.5);
    const auto mesh = vmm::tessellation::generate_voronoi_2d(sites, domain);

    EXPECT_EQ(mesh.cell_count(), sites.size());
    EXPECT_EQ(mesh.generator_ids().size(), sites.size());
    EXPECT_GT(mesh.total_neighbours(), 0U);
}

TEST(Tessellator2DTest, GeneratesClippedCellPolygons)
{
    const auto domain = vmm::domain::make_rectangle_2d(3.0, 2.0);
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.5);
    const auto result = vmm::tessellation::generate_clipped_voronoi_2d(sites, domain);

    ASSERT_EQ(result.planar_cells.size(), sites.size());
    ASSERT_EQ(result.cells.size(), sites.size());
    ASSERT_EQ(result.areas.size(), sites.size());
    EXPECT_EQ(result.topology.cell_count(), sites.size());

    vmm::core::Real area_sum{};
    for(const auto& cell : result.planar_cells) {
        EXPECT_GT(cell.area(), 0.0);
        EXPECT_FALSE(cell.has_holes());
        area_sum += cell.area();
    }
    EXPECT_NEAR(area_sum, domain.area(), 1.0e-8);
}

} // namespace
