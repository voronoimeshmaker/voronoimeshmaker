//==============================================================================
// Name        : ut_Manager.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.5
// Description : Tests for ErrorManager: append, capacity/drop policy,
//               thread-local isolation, and multi-thread stress.
//==============================================================================

//==============================================================================
//      C++ Standard Library includes
//==============================================================================
#include <barrier>
#include <thread>
#include <vector>
#include <string>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/ErrorManager.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

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
// Helper: make a record with distinct message.
//------------------------------------------------------------------------------
static ve::ErrorRecord make_rec(ve::CoreErr e, std::string msg) {
    ve::ErrorRecord r;
    r.code     = ve::error_code(e);
    r.severity = ve::ErrorTraits<ve::CoreErr>::default_severity(e);
    r.message  = std::move(msg);
    return r;
}

//------------------------------------------------------------------------------
// Single-thread sanity (baseline)
//------------------------------------------------------------------------------
TEST(ErrorManager, LogAndFlushWithinCap) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 8;
    ScopedConfig guard(cfg);

    (void)ve::ErrorManager::flush(); // clean this thread

    ve::ErrorManager::log(make_rec(ve::CoreErr::InvalidArgument, "r1"));
    ve::ErrorManager::log(make_rec(ve::CoreErr::NotImplemented, "r2"));

    auto out = ve::ErrorManager::flush();
    ASSERT_EQ(out.size(), 2u);
    EXPECT_EQ(out[0].message, "r1");
    EXPECT_EQ(out[1].message, "r2");

    EXPECT_TRUE(ve::ErrorManager::flush().empty()); // empty after flush
}

// Política de drop pode ser "keep-first" ou "keep-last". Aceitamos ambas.
TEST(ErrorManager, DropsWhenExceedingCap) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 2;  // tiny cap to exercise drop policy
    ScopedConfig guard(cfg);

    (void)ve::ErrorManager::flush(); // clean

    ve::ErrorManager::log(make_rec(ve::CoreErr::InvalidArgument, "first"));
    ve::ErrorManager::log(make_rec(ve::CoreErr::NotImplemented, "second"));
    ve::ErrorManager::log(make_rec(ve::CoreErr::OutOfRange, "third"));

    auto out = ve::ErrorManager::flush();
    ASSERT_EQ(out.size(), 2u);

    const bool keep_first = (out[0].message == "first" && out[1].message == "second");
    const bool keep_last  = (out[0].message == "second" && out[1].message == "third");
    EXPECT_TRUE(keep_first || keep_last)
        << "Unexpected drop policy/order: [" << out[0].message << ", " << out[1].message << "]";
}

TEST(ErrorManager, ThreadLocalIsolation) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 8;
    ScopedConfig guard(cfg);

    (void)ve::ErrorManager::flush(); // clean main thread

    // Log 1 record in main thread.
    ve::ErrorManager::log(make_rec(ve::CoreErr::InvalidArgument, "main-1"));

    // In a separate thread, we should see an independent buffer.
    std::vector<ve::ErrorRecord> other_out;
    std::thread t([&] {
        (void)ve::ErrorManager::flush(); // clean worker thread
        ve::ErrorManager::log(make_rec(ve::CoreErr::NotImplemented, "thr-1"));
        ve::ErrorManager::log(make_rec(ve::CoreErr::OutOfRange, "thr-2"));
        other_out = ve::ErrorManager::flush();
    });
    t.join();

    ASSERT_EQ(other_out.size(), 2u);
    EXPECT_EQ(other_out[0].message, "thr-1");
    EXPECT_EQ(other_out[1].message, "thr-2");

    // Main thread buffer still has its own record.
    auto main_out = ve::ErrorManager::flush();
    ASSERT_EQ(main_out.size(), 1u);
    EXPECT_EQ(main_out[0].message, "main-1");
}

//------------------------------------------------------------------------------
// Parallel stress tests
//------------------------------------------------------------------------------
TEST(ErrorManager, MultiThreadStressNoLoss) {
    // N threads, M records per thread, ensure no loss (cap >= M per thread).
    constexpr int T = 8;
    constexpr int M = 1000;

    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = M;    // ensure no drop per thread
    ScopedConfig guard(cfg);

    std::barrier sync(T);
    std::vector<size_t> counts(T);
    std::vector<std::thread> ths;
    ths.reserve(T);

    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            (void)ve::ErrorManager::flush(); // clean buffer of this thread
            sync.arrive_and_wait();          // aligned start
            for (int i = 0; i < M; ++i) {
                ve::ErrorManager::log(
                    make_rec(ve::CoreErr::InvalidArgument,
                             "t" + std::to_string(t) + ":" +
                             std::to_string(i))
                );
            }
            auto out = ve::ErrorManager::flush();
            counts[t] = out.size();
            // Optional: first/last keep insertion order within the thread
            if (!out.empty()) {
                EXPECT_TRUE(out.front().message.rfind(
                                "t" + std::to_string(t) + ":", 0) == 0);
                EXPECT_TRUE(out.back().message.rfind(
                                "t" + std::to_string(t) + ":", 0) == 0);
            }
        });
    }
    for (auto& th : ths) th.join();

    size_t total = 0;
    for (auto c : counts) total += c;
    EXPECT_EQ(total, static_cast<size_t>(T) * static_cast<size_t>(M));
}

TEST(ErrorManager, MultiThreadDropsRespectCapPerThread) {
    // With small cap per thread, each thread must not exceed that cap on flush.
    constexpr int T = 4;
    constexpr int M = 100;  // much larger than cap to force drops

    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 16;   // small cap => will drop
    ScopedConfig guard(cfg);

    std::barrier sync(T);
    std::vector<size_t> counts(T);
    std::vector<std::thread> ths;
    ths.reserve(T);

    for (int t = 0; t < T; ++t) {
        ths.emplace_back([&, t] {
            (void)ve::ErrorManager::flush();
            sync.arrive_and_wait();
            for (int i = 0; i < M; ++i) {
                ve::ErrorManager::log(
                    make_rec(ve::CoreErr::NotImplemented, "x"));
            }
            counts[t] = ve::ErrorManager::flush().size();
        });
    }
    for (auto& th : ths) th.join();

    for (auto c : counts) {
        EXPECT_LE(c, static_cast<size_t>(cfg.thread_buffer_cap));
    }
}
