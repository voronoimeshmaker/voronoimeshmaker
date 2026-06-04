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
// @file tst_site_pattern_concepts.cpp
// @brief Tests for site pattern concepts.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/site_generation/SitePatternConcept.hpp>

#include <gtest/gtest.h>

namespace {

TEST(SitePatternConceptTest, HexagonalPatternModelsSitePattern2D)
{
    EXPECT_TRUE(vmm::site_generation::SitePattern2D<vmm::site_generation::HexagonalSitePattern2D>);
}

} // namespace
