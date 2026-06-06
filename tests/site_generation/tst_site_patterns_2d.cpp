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

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/CartesianSitePattern2D.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/site_generation/RadialSitePattern2D.hpp>
#include <vmm/site_generation/RandomSitePattern2D.hpp>

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

TEST(SitePatterns2DTest, CartesianPatternCreatesSitesInsideDomain)
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const auto sites = vmm::site_generation::make_cartesian_sites_2d(domain, 0.5);

    ASSERT_FALSE(sites.empty());
    for(const auto site : sites) {
        EXPECT_TRUE(vmm::site_generation::detail::point_in_domain_2d(domain, site));
    }
}

TEST(SitePatterns2DTest, RadialPatternCreatesSitesInsideDomain)
{
    const auto domain = vmm::domain::make_rectangle_2d(4.0, 4.0);
    const auto sites = vmm::site_generation::RadialSitePattern2D{0.5, 12U}.generate(domain);

    ASSERT_FALSE(sites.empty());
    for(const auto site : sites) {
        EXPECT_TRUE(vmm::site_generation::detail::point_in_domain_2d(domain, site));
    }
}

TEST(SitePatterns2DTest, RandomPatternIsDeterministicForFixedSeed)
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const auto first = vmm::site_generation::RandomSitePattern2D{8U, 123U}.generate(domain);
    const auto second = vmm::site_generation::RandomSitePattern2D{8U, 123U}.generate(domain);

    ASSERT_EQ(first.size(), 8U);
    ASSERT_EQ(second.size(), first.size());
    for(std::size_t i = 0U; i < first.size(); ++i) {
        EXPECT_DOUBLE_EQ(first[i].x, second[i].x);
        EXPECT_DOUBLE_EQ(first[i].y, second[i].y);
        EXPECT_TRUE(vmm::site_generation::detail::point_in_domain_2d(domain, first[i]));
    }
}

} // namespace
