//==============================================================================
// Name        : ut_Maneger.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Tests for ErrorManeger: append, capacity/drop policy,
//               thread-local isolation, and multi-thread stress.
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
#include <barrier>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/ErrorManeger.h>
namespace ve = vmm::error;

//------------------------------------------------------------------------------
// RAII guard to temporarily set ErrorConfig (and restore on scope exit).
//------------------------------------------------------------------------------
struct ScopedConfig {
    ve::ErrorConfig old_;
    explicit ScopedConfig(const ve::ErrorConfig& next) {
        if (auto cur = ve::Config::get()) old_ = *cur;
        ve::Config::set(next);
    }
    ~ScopedConfig() { ve::Config::set(old_); }
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
TEST(ErrorManeger, LogAndFlushWithinCap) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 8;
    ScopedConfig guard(cfg);

    (void)ve::ErrorManeger::flush(); // clean this thread

    ve::ErrorManeger::log(make_rec(ve::CoreErr::InvalidArgument, "r1"));
    ve::ErrorManeger::log(make_rec(ve::CoreErr::NotImplemented, "r2"));

    auto out = ve::ErrorManeger::flush();
    ASSERT_EQ(out.size(), 2u);
    EXPECT_EQ(out[0].message, "r1");
    EXPECT_EQ(out[1].message, "r2");

    EXPECT_TRUE(ve::ErrorManeger::flush().empty()); // empty after flush
}

TEST(ErrorManeger, DropsWhenExceedingCap) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 2;  // tiny cap to exercise drop policy
    ScopedConfig guard(cfg);

    (void)ve::ErrorManeger::flush(); // clean

    ve::ErrorManeger::log(make_rec(ve::CoreErr::InvalidArgument, "first"));
    ve::ErrorManeger::log(make_rec(ve::CoreErr::NotImplemented, "second"));
    ve::ErrorManeger::log(make_rec(ve::CoreErr::OutOfRange, "third"));

    auto out = ve::ErrorManeger::flush();
    ASSERT_EQ(out.size(), 2u);
    EXPECT_EQ(out[0].message, "first");
    EXPECT_EQ(out[1].message, "second");
}

TEST(ErrorManeger, ThreadLocalIsolation) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig cfg = *base;
    cfg.thread_buffer_cap = 8;
    ScopedConfig guard(cfg);

    (void)ve::ErrorManeger::flush(); // clean main thread

    // Log 1 record in main thread.
    ve::ErrorManeger::log(make_rec(ve::CoreErr::InvalidArgument, "main-1"));

    // In a separate thread, we should see an independent buffer.
    std::vector<ve::ErrorRecord> other_out;
    std::thread t([&] {
        (void)ve::ErrorManeger::flush(); // clean worker thread
        ve::ErrorManeger::log(make_rec(ve::CoreErr::NotImplemented, "thr-1"));
        ve::ErrorManeger::log(make_rec(ve::CoreErr::OutOfRange, "thr-2"));
        other_out = ve::ErrorManeger::flush();
    });
    t.join();

    ASSERT_EQ(other_out.size(), 2u);
    EXPECT_EQ(other_out[0].message, "thr-1");
    EXPECT_EQ(other_out[1].message, "thr-2");

    // Main thread buffer still has its own record.
    auto main_out = ve::ErrorManeger::flush();
    ASSERT_EQ(main_out.size(), 1u);
    EXPECT_EQ(main_out[0].message, "main-1");
}

//------------------------------------------------------------------------------
// Parallel stress tests
//------------------------------------------------------------------------------
TEST(ErrorManeger, MultiThreadStressNoLoss) {
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
            (void)ve::ErrorManeger::flush(); // clean buffer of this thread
            sync.arrive_and_wait();          // aligned start
            for (int i = 0; i < M; ++i) {
                ve::ErrorManeger::log(
                    make_rec(ve::CoreErr::InvalidArgument,
                             "t" + std::to_string(t) + ":" +
                             std::to_string(i))
                );
            }
            auto out = ve::ErrorManeger::flush();
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

TEST(ErrorManeger, MultiThreadDropsRespectCapPerThread) {
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
            (void)ve::ErrorManeger::flush();
            sync.arrive_and_wait();
            for (int i = 0; i < M; ++i) {
                ve::ErrorManeger::log(
                    make_rec(ve::CoreErr::NotImplemented, "x"));
            }
            counts[t] = ve::ErrorManeger::flush().size();
        });
    }
    for (auto& th : ths) th.join();

    for (auto c : counts) {
        EXPECT_LE(c, static_cast<size_t>(cfg.thread_buffer_cap));
    }
}
