// -----------------------------------------------------------------------------
// ut_core_errors.cpp
// Tests for CoreErrors enum and its basic traits/mappings.
// One test executable dedicated to this header/module.
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
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

namespace ve = vmm::error;

namespace {
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

TEST(CoreErrors, IsEnumType) {
    static_assert(std::is_enum_v<ve::CoreErr>, "CoreErr must be an enum");
    SUCCEED();
}

TEST(CoreErrors, ErrorCodesAreNonZeroAndDistinct) {
    // Pick three commonly used enumerators in the library:
    auto c1 = ve::error_code(ve::CoreErr::InvalidArgument);
    auto c2 = ve::error_code(ve::CoreErr::NotImplemented);
    auto c3 = ve::error_code(ve::CoreErr::AssertFailed);

    EXPECT_GT(c1, 0u);
    EXPECT_GT(c2, 0u);
    EXPECT_GT(c3, 0u);

    // Distinctness across the same domain is expected
    // (domain encoding + value should differ).
    EXPECT_NE(c1, c2);
    EXPECT_NE(c1, c3);
    EXPECT_NE(c2, c3);
}

TEST(CoreErrors, DefaultSeverityIsAValidEnumerator) {
    auto s1 = ve::ErrorTraits<ve::CoreErr>::default_severity(
                  ve::CoreErr::InvalidArgument);
    auto s2 = ve::ErrorTraits<ve::CoreErr>::default_severity(
                  ve::CoreErr::NotImplemented);
    auto s3 = ve::ErrorTraits<ve::CoreErr>::default_severity(
                  ve::CoreErr::AssertFailed);

    EXPECT_TRUE(is_valid(s1));
    EXPECT_TRUE(is_valid(s2));
    EXPECT_TRUE(is_valid(s3));
}
