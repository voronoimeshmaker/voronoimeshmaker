//==============================================================================
// Name        : ut_Core_Errors.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Unit tests for CoreErrors enum and its default traits/mappings.
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
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

namespace ve = vmm::error;

namespace {
//------------------------------------------------------------------------------
// Helper: check that a Severity enumerator is one of the supported values.
//------------------------------------------------------------------------------
inline bool is_valid(ve::Severity s) {
    switch (s) {
        case ve::Severity::Info:
        case ve::Severity::Warning:
        case ve::Severity::Error:
        case ve::Severity::Fatal:
            return true;
        default:
            return false;
    }
}
} // namespace

//------------------------------------------------------------------------------
// Basic type sanity
//------------------------------------------------------------------------------
TEST(CoreErrors, IsEnumType) {
    static_assert(std::is_enum_v<ve::CoreErr>, "CoreErr must be an enum");
    SUCCEED();
}

//------------------------------------------------------------------------------
// Error codes should be positive (non-zero) and distinct within the same domain.
//------------------------------------------------------------------------------
TEST(CoreErrors, ErrorCodesAreNonZeroAndDistinct) {
    const auto c1 = ve::error_code(ve::CoreErr::InvalidArgument);
    const auto c2 = ve::error_code(ve::CoreErr::NotImplemented);
    const auto c3 = ve::error_code(ve::CoreErr::AssertFailed);

    EXPECT_GT(c1, 0u);
    EXPECT_GT(c2, 0u);
    EXPECT_GT(c3, 0u);

    EXPECT_NE(c1, c2);
    EXPECT_NE(c1, c3);
    EXPECT_NE(c2, c3);
}

//------------------------------------------------------------------------------
// Traits must map each enumerator to a valid default severity.
//------------------------------------------------------------------------------
TEST(CoreErrors, DefaultSeverityIsAValidEnumerator) {
    const auto s1 =
        ve::ErrorTraits<ve::CoreErr>::default_severity(
            ve::CoreErr::InvalidArgument);
    const auto s2 =
        ve::ErrorTraits<ve::CoreErr>::default_severity(
            ve::CoreErr::NotImplemented);
    const auto s3 =
        ve::ErrorTraits<ve::CoreErr>::default_severity(
            ve::CoreErr::AssertFailed);

    EXPECT_TRUE(is_valid(s1));
    EXPECT_TRUE(is_valid(s2));
    EXPECT_TRUE(is_valid(s3));
}
