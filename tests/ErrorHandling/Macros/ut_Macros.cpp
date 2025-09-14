//==============================================================================
// Name        : ut_Macros.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.4
// Description : Parallel tests for ErrorHandling Macros:
//               - VMM_ERROR logging across threads
//               - VMM_REQUIRE mixed pass/throw in parallel
//               - VMM_ASSERT behavior (Debug vs Release)
// License     : GNU GPL v3
//==============================================================================

//==============================================================================
//      C++ Standard Library includes
//==============================================================================
#include <barrier>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/Macros.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorManager.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
// #include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// RAII guard to temporarily set ErrorConfig (and restore on scope exit).
//------------------------------------------------------------------------------
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
// Convenience: clear current thread buffer.
//------------------------------------------------------------------------------
static void clean_thread_buffer() {
    (void)ve::ErrorManager::flush();
}

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

// Parallel logging with VMM_ERROR: each thread logs M records and flushes.
// Ensures no throw and that per-thread buffers are isolated.
TEST(Macros, Parallel_VMM_ERROR_NoThrow_And_PerThreadIsolation) {
#ifdef VMM_ERROR
    constexpr int T = 8;
    constexpr int M = 300;

    // Ensure capacity ≥ M to avoid drops.
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = M;
    ScopedConfig guard(cfg);

    std::barrier sync(T);
    std::vector<size_t> counts(T);
    std::vector<std::thread> ths;
    ths.reserve(T);

    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            clean_thread_buffer();
            sync.arrive_and_wait();
            for (int i = 0; i < M; ++i) {
                // Use a code with a known placeholder {name}.
                EXPECT_NO_THROW(
                    VMM_ERROR(ve::CoreErr::InvalidArgument,
                              {{"name", "thr" + std::to_string(t)}})
                );
                if ((i & 0x7F) == 0) std::this_thread::yield();
            }
            auto out = ve::ErrorManager::flush();
            counts[t] = out.size();
            // All records from this thread should be present and tagged.
            ASSERT_EQ(out.size(), static_cast<size_t>(M));
            for (auto& r : out) {
                EXPECT_NE(
                    r.message.find("thr" + std::to_string(t)),
                    std::string::npos
                );
            }
        });
    }
    for (auto& th : ths) th.join();

    size_t total = 0;
    for (auto c : counts) total += c;
    EXPECT_EQ(total, static_cast<size_t>(T) * static_cast<size_t>(M));
#else
    GTEST_SKIP() << "VMM_ERROR not defined.";
#endif
}

// Parallel VMM_REQUIRE: half the threads pass (true), half throw (false).
// Each throwing thread must catch its own exception; no cross-thread impact.
TEST(Macros, Parallel_VMM_REQUIRE_MixedThrowAndPass) {
#ifdef VMM_REQUIRE
    constexpr int T = 12;
    std::barrier sync(T);
    std::atomic<int> threw{0}, passed{0};

    std::vector<std::thread> ths;
    ths.reserve(T);
    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            sync.arrive_and_wait();
            const bool should_throw = (t % 2 == 1);
            if (should_throw) {
                try {
                    VMM_REQUIRE(false, ve::CoreErr::InvalidArgument,
                                {{"name", "t" + std::to_string(t)}});
                } catch (const ve::VMMException&) {
                    threw.fetch_add(1, std::memory_order_relaxed);
                }
            } else {
                EXPECT_NO_THROW(
                    VMM_REQUIRE(true, ve::CoreErr::InvalidArgument,
                                {{"name", "t" + std::to_string(t)}})
                );
                passed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& th : ths) th.join();

    EXPECT_EQ(threw.load(), T / 2);
    EXPECT_EQ(passed.load(), (T + 1) / 2); // ceil for odd T
#else
    GTEST_SKIP() << "VMM_REQUIRE not defined.";
#endif
}

// Parallel VMM_ASSERT (Debug only): some threads trigger the assert.
// In Release (NDEBUG), the macro is a no-op and should not throw.
TEST(Macros, Parallel_VMM_ASSERT_Behavior) {
#if defined(VMM_ASSERT)
  #if !defined(NDEBUG)
    constexpr int T = 10;
    std::barrier sync(T);
    std::atomic<int> caught{0}, silent{0};

    std::vector<std::thread> ths;
    ths.reserve(T);
    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            sync.arrive_and_wait();
            if (t % 2 == 0) {
                try {
                    VMM_ASSERT(false);
                } catch (const ve::VMMException&) {
                    caught.fetch_add(1, std::memory_order_relaxed);
                }
            } else {
                EXPECT_NO_THROW(VMM_ASSERT(true));
                silent.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& th : ths) th.join();

    EXPECT_EQ(caught.load(), (T + 1) / 2); // even indices
    EXPECT_EQ(silent.load(), T / 2);
  #else
    // NDEBUG: assert is a no-op
    EXPECT_NO_THROW(VMM_ASSERT(false));
  #endif
#else
    GTEST_SKIP() << "VMM_ASSERT not defined.";
#endif
}

// Parallel pressure: mixed macro calls (ERROR + passing REQUIRE) together,
// ensuring no exceptions leak and logs remain per-thread.
TEST(Macros, Parallel_MixedPressure_NoThrowAndConsistentLogging) {
#if defined(VMM_ERROR) && defined(VMM_REQUIRE)
    constexpr int T = 6;
    constexpr int M = 200;

    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = M;
    ScopedConfig guard(cfg);

    std::barrier sync(T);
    std::vector<size_t> counts(T);
    std::vector<std::thread> ths;
    ths.reserve(T);

    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            clean_thread_buffer();
            sync.arrive_and_wait();
            for (int i = 0; i < M; ++i) {
                EXPECT_NO_THROW(
                    VMM_ERROR(ve::CoreErr::InvalidArgument,
                              {{"name", "mix" + std::to_string(t)}})
                );
                EXPECT_NO_THROW(
                    VMM_REQUIRE(true, ve::CoreErr::InvalidArgument,
                                {{"name", "ok"}})
                );
            }
            counts[t] = ve::ErrorManager::flush().size();
        });
    }
    for (auto& th : ths) th.join();

    size_t total = 0;
    for (auto c : counts) total += c;
    EXPECT_EQ(total, static_cast<size_t>(T) * static_cast<size_t>(M));
#else
    GTEST_SKIP() << "VMM_ERROR and/or VMM_REQUIRE not defined.";
#endif
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
