// -----------------------------------------------------------------------------
// ut_severity.cpp
// Basic sanity tests for the Severity enum.
// One test executable dedicated to Severity.
// Comments/messages in English.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <type_traits>

// -----------------------------------------------------------------------------
//  include gtest
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

namespace ve = vmm::error;

// -----------------------------------------------------------------------------
//  tests
// -----------------------------------------------------------------------------

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
