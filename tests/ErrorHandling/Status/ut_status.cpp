// -----------------------------------------------------------------------------
// ut_status.cpp
// Tests for Status and StatusOr.
// One test executable dedicated to this module.
// Comments/messages in English.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  include gtest
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Status.h>

namespace ve = vmm::error;

// -----------------------------------------------------------------------------
//  tests: Status
// -----------------------------------------------------------------------------

TEST(Status, OkIsOkAndHasZeroCode) {
    auto st = ve::Status::Ok();
    EXPECT_TRUE(st.ok());
    EXPECT_EQ(st.code(), 0u);
}

TEST(Status, ErrorCarriesFields) {
    ve::Status st(42u, "Bad thing happened", ve::Severity::Error);
    EXPECT_FALSE(st.ok());
    EXPECT_EQ(st.code(), 42u);
    EXPECT_EQ(st.message(), "Bad thing happened");
    EXPECT_EQ(st.severity(), ve::Severity::Error);
}

TEST(Status, CopyAndMoveSemantics) {
    ve::Status s1(7u, "X", ve::Severity::Warning);

    ve::Status s2 = s1;  // copy
    EXPECT_EQ(s2.code(), 7u);
    EXPECT_EQ(s2.message(), "X");
    EXPECT_EQ(s2.severity(), ve::Severity::Warning);

    ve::Status s3 = std::move(s1);  // move
    EXPECT_EQ(s3.code(), 7u);
    EXPECT_EQ(s3.message(), "X");
    EXPECT_EQ(s3.severity(), ve::Severity::Warning);
}

// -----------------------------------------------------------------------------
//  tests: StatusOr<T>
// -----------------------------------------------------------------------------

TEST(StatusOr, HoldsValue) {
    ve::StatusOr<int> so(7);
    EXPECT_TRUE(so.ok());
    EXPECT_EQ(so.value(), 7);
}

TEST(StatusOr, HoldsErrorStatus) {
    ve::Status err(2u, "nope", ve::Severity::Warning);
    ve::StatusOr<int> so(err);
    EXPECT_FALSE(so.ok());
    EXPECT_EQ(so.status().code(), 2u);
    EXPECT_EQ(so.status().severity(), ve::Severity::Warning);
    EXPECT_EQ(so.status().message(), "nope");
}

TEST(StatusOr, CopyMoveValueType) {
    struct P {
        int a{};
        std::string s;
    };

    ve::StatusOr<P> so(P{10, "ten"});
    ASSERT_TRUE(so.ok());

    auto p1 = so.value();  // copy-out
    EXPECT_EQ(p1.a, 10);
    EXPECT_EQ(p1.s, "ten");

    P p2 = std::move(so.value());  // move-out (allowed by many impls)
    EXPECT_EQ(p2.a, 10);
    EXPECT_EQ(p2.s, "ten");
}
