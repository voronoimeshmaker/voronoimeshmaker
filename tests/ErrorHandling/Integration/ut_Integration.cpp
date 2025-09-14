//==============================================================================
// File        : ut_Integration.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1.0
// Description : Integration tests for Language <-> ErrorConfig
//               - default language
//               - scoped language guard (set/restore)
//               - concurrent readers (threads + barrier)
//               - parallel readers (<execution>::par)
// License     : GNU GPL v3
//==============================================================================

//==============================================================================
//      C++ Standard Library includes
//==============================================================================
#include <type_traits>
#include <barrier>
#include <execution>
#include <thread>
#include <vector>

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
// Uses shared_ptr from Config::get() to avoid relying on default-constructed
// state when there is not an active configuration yet.
//------------------------------------------------------------------------------
struct ScopedLanguage {
    std::shared_ptr<const ve::ErrorConfig> prev_;
    explicit ScopedLanguage(ve::Language lang) {
        prev_ = ve::Config::get();
        ve::ErrorConfig next = prev_ ? *prev_ : ve::ErrorConfig{};
        next.language = lang;
        ve::Config::set(next);
    }
    ~ScopedLanguage() {
        if (prev_) ve::Config::set(*prev_);
    }
};

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------
TEST(LanguageIntegration, IsEnumAndHasTwoLocales) {
    static_assert(std::is_enum_v<ve::Language>, "Language must be an enum");
    auto en = ve::Language::EnUS;
    auto pt = ve::Language::PtBR;
    (void)en; (void)pt;
    SUCCEED();
}

TEST(LanguageIntegration, DefaultIsEnUS) {
    auto cfg = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(cfg));
    EXPECT_EQ(cfg->language, ve::Language::EnUS);
}

TEST(LanguageIntegration, ChangeAndRestoreWithScopedGuard) {
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

//------------------------------------------------------------------------------
// Concurrent readers using std::barrier: all threads start together and verify
// the language is what we set on the main thread.
//------------------------------------------------------------------------------
TEST(LanguageIntegration, ConcurrentReadersBarrier) {
    // Set once on main thread; readers only read.
    {
        auto cur = ve::Config::get();
        ASSERT_TRUE(static_cast<bool>(cur));
        auto next = *cur;
        next.language = ve::Language::PtBR;
        ve::Config::set(next);
    }

    constexpr int Readers = 16;
    constexpr int Iters   = 2000;
    std::barrier sync(Readers);
    std::atomic<int> bad{0};

    auto worker = [&] {
        sync.arrive_and_wait();
        for (int i = 0; i < Iters; ++i) {
            auto cfg = ve::Config::get();
            if (!cfg || cfg->language != ve::Language::PtBR) {
                ++bad; break;
            }
            if ((i & 0xFF) == 0) std::this_thread::yield();
        }
    };

    std::vector<std::thread> th;
    th.reserve(Readers);
    for (int r = 0; r < Readers; ++r) th.emplace_back(worker);
    for (auto &t : th) t.join();

    EXPECT_EQ(bad.load(), 0);

    // Restore default for other tests
    {
        auto cur = ve::Config::get();
        ASSERT_TRUE(static_cast<bool>(cur));
        auto next = *cur;
        next.language = ve::Language::EnUS;
        ve::Config::set(next);
    }
}

//------------------------------------------------------------------------------
// Parallel readers using std::execution::par. Again, readers only.
//------------------------------------------------------------------------------
TEST(LanguageIntegration, ParallelReadersStdPar) {
    // Ensure EnUS before running
    {
        auto cur = ve::Config::get();
        ASSERT_TRUE(static_cast<bool>(cur));
        auto next = *cur;
        next.language = ve::Language::EnUS;
        ve::Config::set(next);
    }

    // Dummy index vector to drive parallel for_each
    std::vector<int> idx(5000);
    for (int i = 0; i < static_cast<int>(idx.size()); ++i) idx[i] = i;

    std::atomic<int> ok{0};
    std::for_each(std::execution::par, idx.begin(), idx.end(),
                  [&](int) {
                      auto cfg = ve::Config::get();
                      if (cfg && (cfg->language == ve::Language::EnUS ||
                                  cfg->language == ve::Language::PtBR)) {
                          ++ok;
                      }
                  });

    EXPECT_EQ(ok.load(), static_cast<int>(idx.size()));
}
