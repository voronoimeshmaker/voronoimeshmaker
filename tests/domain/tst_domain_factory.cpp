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
// @file tst_domain_factory.cpp
// @brief Tests for open domain factory registry.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/DomainFactory.hpp>

#include <gtest/gtest.h>

namespace {

TEST(DomainFactoryTest, BuildsDefaultRectangle)
{
    const auto registry = vmm::domain::default_domain_factory_registry();
    const vmm::domain::DomainParameterMap parameters{{"width", 3.0}, {"height", 2.0}};
    const auto domain = registry.make("rectangle", parameters);

    EXPECT_DOUBLE_EQ(domain.area(), 6.0);
}

TEST(DomainFactoryTest, AllowsUserRegisteredFactory)
{
    auto registry = vmm::domain::default_domain_factory_registry();
    registry.register_factory("unit_square", [](const vmm::domain::DomainParameterMap&) {
        return vmm::domain::make_rectangle_2d(1.0, 1.0);
    });

    EXPECT_DOUBLE_EQ(registry.make("unit_square", {}).area(), 1.0);
}

} // namespace
