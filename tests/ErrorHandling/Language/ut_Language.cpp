// -----------------------------------------------------------------------------
// ut_language.cpp
// Tests for Language enum and interaction with ErrorConfig.
// One test executable dedicated to Language.
// Comments/messages in English.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  include gtest
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Language.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

namespace ve = vmm::error;

// -----------------------------------------------------------------------------
//  helpers
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
//  tests
// -----------------------------------------------------------------------------

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
