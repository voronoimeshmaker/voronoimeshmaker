//==============================================================================
// Name        : ut_MessageCatalog.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.5
// Description : Parallel tests for message rendering (EN/PT) using explicit
//               locale and under concurrent config toggling. Focus on the
//               thread-safety of render(). One test executable dedicated to
//               MessageCatalog.
//==============================================================================

//==============================================================================
//      C++ Standard Library includes
//==============================================================================
#include <barrier>
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include <string_view>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/MessageCatalog.hpp>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------
static bool contains(std::string_view hay, std::string_view needle) {
    return hay.find(needle) != std::string_view::npos;
}

// RAII to set a config and restore the previous one on scope exit.
// Usa shared_ptr para restaurar exatamente o estado anterior quando existir.
struct ScopedConfig {
    std::shared_ptr<const ve::ErrorConfig> prev_;
    explicit ScopedConfig(const ve::ErrorConfig& next) {
        prev_ = ve::Config::get();
        ve::Config::set(next);
    }
    ~ScopedConfig() {
        if (prev_) ve::Config::set(*prev_);
    }
};

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

// Parallel render with EXPLICIT locale (no shared global reads).
// Each thread picks a fixed locale and renders M messages with unique data.
// We only assert placeholder substitution occurred.
TEST(MessageCatalog, Parallel_Render_ExplicitLocale_NoGlobalState) {
    constexpr int T = 8;
    constexpr int M = 500;

    std::barrier sync(T);
    std::vector<std::thread> ths;
    ths.reserve(T);
    std::atomic<int> failures{0};

    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            sync.arrive_and_wait();
            ve::Language lang =
                (t % 2 == 0) ? ve::Language::EnUS : ve::Language::PtBR;

            for (int i = 0; i < M; ++i) {
                std::string val =
                    "t" + std::to_string(t) + "-" + std::to_string(i);
                std::string msg = ve::render(
                    ve::CoreErr::InvalidArgument, lang, {{"name", val}});
                if (msg.empty() || !contains(msg, val)) {
                    failures.fetch_add(1, std::memory_order_relaxed);
                    break;
                }
                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }
    for (auto& th : ths) th.join();

    EXPECT_EQ(failures.load(), 0)
        << "render() failed to substitute placeholders";
}

// Parallel render while CONFIG LANGUAGE TOGGLES concurrently.
// Writer flips between EnUS and PtBR; readers snapshot cfg->language and call
// render() with that explicit value (so render() sees a coherent locale).
// Ensures no crashes and proper placeholder substitution under churn.
TEST(MessageCatalog, Parallel_Render_ConfigDriven_WithLanguageToggling) {
    // Prepare two distinct languages.
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig A = *base; A.language = ve::Language::EnUS;
    ve::ErrorConfig B = *base; B.language = ve::Language::PtBR;

    // Publish starting state and auto-restore after the test.
    ScopedConfig guard(A);

    constexpr int Readers = 8;
    constexpr int Iters   = 3000;

    std::barrier sync(Readers + 1);
    std::atomic<int> failures{0};

    // Readers: repeatedly snapshot cfg->language and render with it.
    std::vector<std::thread> readers;
    readers.reserve(Readers);
    for (int r = 0; r < Readers; ++r) {
        readers.emplace_back([&, r] {  // capture 'r' by value (fix ASan)
            sync.arrive_and_wait();
            for (int i = 0; i < Iters; ++i) {
                auto cfg = ve::Config::get();
                if (!cfg) {
                    failures.fetch_add(1, std::memory_order_relaxed);
                    break;
                }
                auto lang = cfg->language;
                std::string val =
                    "R" + std::to_string(r) + "-" + std::to_string(i);
                std::string msg = ve::render(
                    ve::CoreErr::InvalidArgument, lang, {{"name", val}});
                if (msg.empty() || !contains(msg, val)) {
                    failures.fetch_add(1, std::memory_order_relaxed);
                    break;
                }
                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }

    // Writer: flip language many times.
    std::thread writer([&] {
        sync.arrive_and_wait();
        for (int i = 0; i < Iters; ++i) {
            ve::Config::set((i & 1) ? B : A);
            if ((i & 0x7F) == 0) std::this_thread::yield();
        }
    });

    for (auto& t : readers) t.join();
    writer.join();

    EXPECT_EQ(failures.load(), 0)
        << "render() failed under config toggling";
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
