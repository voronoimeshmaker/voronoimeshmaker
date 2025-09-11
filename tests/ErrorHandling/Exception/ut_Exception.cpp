//==============================================================================
// Name        : ut_Exception.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Tests for VMMException: code(), severity(), what(), message(),
//               and location(). One test executable dedicated to VMMException.
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
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// RAII helper: temporarily switch language; restore on scope exit.
//------------------------------------------------------------------------------
namespace {
struct ScopedLanguage {
    ve::ErrorConfig old_;
    explicit ScopedLanguage(ve::Language lang) {
        if (auto cur = ve::Config::get()) old_ = *cur;
        auto next     = old_;
        next.language = lang;
        ve::Config::set(next);
    }
    ~ScopedLanguage() { ve::Config::set(old_); }
};
} // namespace

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------
TEST(VMMException, CarriesCodeSeverityMessageAndLocation) {
    // Force PtBR to ensure message() is localized; what() must remain English.
    ScopedLanguage lang_guard(ve::Language::PtBR);

    constexpr ve::CoreErr err = ve::CoreErr::InvalidArgument;

    // Known placeholder {name} for InvalidArgument.
    ve::VMMException ex(err, {{"name", "beta"}});

    // code() must match composed domain+value per ErrorTraits.
    EXPECT_EQ(ex.code(), ve::error_code(err));

    // severity() must match the default severity from traits.
    EXPECT_EQ(ex.severity(),
              ve::ErrorTraits<ve::CoreErr>::default_severity(err));

    // what() (English, stable) and message() (localized) must be non-empty
    // and contain the interpolated placeholder value.
    std::string w = ex.what();
    std::string m = ex.message();
    EXPECT_FALSE(w.empty());
    EXPECT_FALSE(m.empty());
    EXPECT_NE(w.find("beta"), std::string::npos);
    EXPECT_NE(m.find("beta"), std::string::npos);

    // location() should have useful metadata.
    auto loc = ex.location();
    EXPECT_GT(loc.line(), 0u);
    EXPECT_FALSE(std::string(loc.file_name()).empty());
}

TEST(VMMException, WorksForDifferentErrorKinds) {
    // Smoke-check other enumerators (no strict text expectations).
    ve::VMMException ex1(ve::CoreErr::OutOfRange, {{"index", "5"}});
    EXPECT_EQ(ex1.code(), ve::error_code(ve::CoreErr::OutOfRange));
    EXPECT_FALSE(std::string(ex1.what()).empty());

    ve::VMMException ex2(ve::CoreErr::NotImplemented);
    EXPECT_EQ(ex2.code(), ve::error_code(ve::CoreErr::NotImplemented));
    EXPECT_FALSE(std::string(ex2.message()).empty());

    ve::VMMException ex3(ve::CoreErr::AssertFailed);
    EXPECT_EQ(ex3.code(), ve::error_code(ve::CoreErr::AssertFailed));
    EXPECT_FALSE(std::string(ex3.what()).empty());
}

TEST(VMMException, CopyAndMoveKeepStableTexts) {
    ScopedLanguage lang_guard(ve::Language::PtBR);
    ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name", "gamma"}});
    std::string what_before = ex.what();
    std::string msg_before  = ex.message();

    // copy
    ve::VMMException ex_copy = ex;
    EXPECT_EQ(ex_copy.code(), ex.code());
    EXPECT_FALSE(std::string(ex_copy.what()).empty());
    EXPECT_FALSE(std::string(ex_copy.message()).empty());

    // move
    ve::VMMException ex_moved = std::move(ex_copy);
    EXPECT_EQ(ex_moved.code(), ve::error_code(ve::CoreErr::InvalidArgument));
    EXPECT_NE(std::string(ex_moved.what()).find("gamma"), std::string::npos);
    EXPECT_NE(std::string(ex_moved.message()).find("gamma"), std::string::npos);

    // original 'ex' should still have stable strings
    EXPECT_FALSE(what_before.empty());
    EXPECT_FALSE(msg_before.empty());
}
