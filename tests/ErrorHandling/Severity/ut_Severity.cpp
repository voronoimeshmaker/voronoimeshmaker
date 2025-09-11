//==============================================================================
// Name        : ut_Severity.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Basic sanity tests for the Severity enum.
//               One test executable dedicated to Severity.
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
//      C++ Standard Library includes
//==============================================================================
#include <type_traits>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------
TEST(Severity, IsEnumType) {
    static_assert(std::is_enum_v<ve::Severity>, "Severity must be an enum");
    SUCCEED();
}

TEST(Severity, EnumeratorsAreDistinct) {
    // We do not assume any particular numeric ordering—only distinct values.
    auto info    = static_cast<int>(ve::Severity::Info);
    auto warning = static_cast<int>(ve::Severity::Warning);
    auto error   = static_cast<int>(ve::Severity::Error);
    auto fatal   = static_cast<int>(ve::Severity::Fatal);

    EXPECT_NE(info, warning);
    EXPECT_NE(info, error);
    EXPECT_NE(info, fatal);
    EXPECT_NE(warning, error);
    EXPECT_NE(warning, fatal);
    EXPECT_NE(error, fatal);
}

TEST(Severity, NoAccidentalAliasesBetweenCommonValues) {
    // Only checks that common levels are not aliased to each other.
    EXPECT_NE(static_cast<int>(ve::Severity::Warning),
              static_cast<int>(ve::Severity::Error));
    EXPECT_NE(static_cast<int>(ve::Severity::Info),
              static_cast<int>(ve::Severity::Warning));
}
