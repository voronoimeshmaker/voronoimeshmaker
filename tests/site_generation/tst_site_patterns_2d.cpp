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
// @file tst_site_patterns_2d.cpp
// @brief Tests for built-in 2D point generator site patterns.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>

#include <gtest/gtest.h>

namespace {

TEST(SitePatterns2DTest, HexagonalPatternCreatesSitesInsideDomain)
{
    const auto domain = vmm::domain::make_rectangle_2d(4.0, 2.0);
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.5);

    ASSERT_FALSE(sites.empty());
    for(const auto site : sites) {
        EXPECT_TRUE(vmm::site_generation::detail::point_in_domain_2d(domain, site));
    }
}

} // namespace
