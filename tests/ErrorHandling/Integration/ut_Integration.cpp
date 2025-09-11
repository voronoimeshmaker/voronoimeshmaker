//==============================================================================
// Name        : ut_Language.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Tests for Language enum and its interaction with ErrorConfig.
//               One test executable dedicated to Language.
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
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/Language.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// RAII helper: temporarily switch language; restore on scope exit.
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------
TEST(Language, IsEnumAndHasTwoLocales) {
    static_assert(std::is_enum_v<ve::Language>, "Language must be an enum");
    auto en = ve::Language::EnUS;
    auto pt = ve::Language::PtBR;
    (void)en; (void)pt;
    SUCCEED();
}

TEST(Language, DefaultIsEnUS) {
    auto cfg = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(cfg));
    EXPECT_EQ(cfg->language, ve::Language::EnUS);
}

TEST(Language, ChangeAndRestoreWithScopedGuard) {
    auto before = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(before));

    {
        ScopedLanguage guard(ve::Language::PtBR);
        auto now = ve::Config::get();
        ASSERT_TRUE(static_cast<bool>(now));
        EXPECT_EQ(now->language, ve::Language::PtBR);
    }

    auto after = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(after));
    EXPECT_EQ(after->language, before->language);
}
