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
// @file tst_site_factory.cpp
// @brief Tests for open site pattern factories.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/SiteFactory.hpp>

#include <gtest/gtest.h>

namespace {

TEST(SiteFactoryTest, BuildsDefaultHexagonalPattern)
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const auto registry = vmm::site_generation::default_site_factory_registry_2d();
    const auto sites = registry.make("hexagonal_2d", domain, {{"spacing", 0.5}});
    EXPECT_FALSE(sites.empty());
}

TEST(SiteFactoryTest, BuildsAdditionalDefault2DPatterns)
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const auto registry = vmm::site_generation::default_site_factory_registry_2d();

    EXPECT_FALSE(registry.make("cartesian_2d", domain, {{"spacing", 0.5}}).empty());
    EXPECT_FALSE(registry.make("radial_2d", domain, {{"spacing", 0.5}, {"angular_count", 8.0}}).empty());
    EXPECT_EQ(registry.make("random_2d", domain, {{"count", 5.0}, {"seed", 7.0}}).size(), 5U);
}

} // namespace
