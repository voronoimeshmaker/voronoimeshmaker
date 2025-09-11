//==============================================================================
// Name        : ut_Config.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Concurrency tests for ErrorConfig/Config (atomic snapshot).
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
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// RAII guard to restore the original config at scope exit.
//------------------------------------------------------------------------------
struct ScopedConfig {
    ve::ErrorConfig old_;
    explicit ScopedConfig(const ve::ErrorConfig& next) {
        if (auto cur = ve::Config::get()) old_ = *cur;
        ve::Config::set(next);
    }
    ~ScopedConfig() { ve::Config::set(old_); }
};

static bool equals_on_core_fields(const ve::ErrorConfig& a,
                                  const ve::ErrorConfig& b) {
    return a.language          == b.language
        && a.min_severity      == b.min_severity
        && a.thread_buffer_cap == b.thread_buffer_cap;
}

//------------------------------------------------------------------------------
// Serial sanity baseline for defaults
//------------------------------------------------------------------------------
TEST(Config, DefaultIsSane_SerialBaseline) {
    auto cfg = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(cfg));
    EXPECT_TRUE(cfg->thread_buffer_cap >= 1);
    EXPECT_TRUE(cfg->language == ve::Language::EnUS ||
                cfg->language == ve::Language::PtBR);
    (void)cfg->min_severity; // field exists
}

//------------------------------------------------------------------------------
// Readers must only observe either version A or version B of the config,
// never a mixed/torn snapshot while a writer toggles.
//------------------------------------------------------------------------------
TEST(Config, ConcurrentReadersAndWriters_NoTornSnapshots) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));

    ve::ErrorConfig A = *base;
    A.language          = ve::Language::EnUS;
    A.min_severity      = ve::Severity::Warning;
    A.thread_buffer_cap = 17;

    ve::ErrorConfig B = *base;
    B.language          = ve::Language::PtBR;
    B.min_severity      = ve::Severity::Error;
    B.thread_buffer_cap = 19;

    // Publish known starting state (A).
    ScopedConfig guard(A);

    constexpr int Readers     = 8;
    constexpr int ReadIters   = 5'000;
    constexpr int WriterIters = 2'000;

    std::barrier sync(Readers + 1);

    // Each reader tracks violations and whether it has observed both versions.
    struct Obs { bool sawA=false, sawB=false, bad=false; };
    std::vector<Obs> obs(Readers);

    // Spawn readers
    std::vector<std::thread> th;
    th.reserve(Readers);
    for (int r = 0; r < Readers; ++r) {
        th.emplace_back([&, r]{
            sync.arrive_and_wait();
            for (int i = 0; i < ReadIters; ++i) {
                auto cur = ve::Config::get();
                if (!cur) { obs[r].bad = true; break; }
                if (equals_on_core_fields(*cur, A)) obs[r].sawA = true;
                else if (equals_on_core_fields(*cur, B)) obs[r].sawB = true;
                else { obs[r].bad = true; break; } // mixed/torn snapshot

                // tiny pause occasionally to allow interleaving
                if ((i & 0x3FF) == 0) std::this_thread::yield();
            }
        });
    }

    // Writer toggles between A and B.
    auto writer = std::thread([&]{
        sync.arrive_and_wait();
        for (int i = 0; i < WriterIters; ++i) {
            ve::Config::set((i & 1) ? B : A);
            if ((i & 0xFF) == 0) std::this_thread::yield();
        }
    });

    for (auto& t : th) t.join();
    writer.join();

    // No reader should have observed a torn/mixed snapshot.
    for (int r = 0; r < Readers; ++r) {
        EXPECT_FALSE(obs[r].bad) << "Reader " << r
                                 << " saw a mixed snapshot";
    }

    // Sanity: at least one reader must have seen both versions.
    bool any_both = false;
    for (auto& o : obs) any_both = any_both || (o.sawA && o.sawB);
    EXPECT_TRUE(any_both) << "No reader observed both A and B under toggling";
}

//------------------------------------------------------------------------------
// Many writers flip between two configs while readers continuously load;
// readers must still only see either A or B — never a partially-mixed state.
//------------------------------------------------------------------------------
TEST(Config, ManyWriters_ManyReaders_AllSnapshotsMatchAorB) {
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));

    ve::ErrorConfig A = *base;
    A.language          = ve::Language::EnUS;
    A.min_severity      = ve::Severity::Warning;
    A.thread_buffer_cap = 21;

    ve::ErrorConfig B = *base;
    B.language          = ve::Language::PtBR;
    B.min_severity      = ve::Severity::Error;
    B.thread_buffer_cap = 34;

    ScopedConfig guard(A);

    constexpr int Writers = 4;
    constexpr int Readers = 8;
    constexpr int Iters   = 3'000;

    std::barrier sync(Writers + Readers);

    // Readers
    std::vector<std::thread> readers;
    readers.reserve(Readers);
    std::atomic<int> bad_reads{0};
    for (int r = 0; r < Readers; ++r) {
        readers.emplace_back([&]{
            sync.arrive_and_wait();
            for (int i = 0; i < Iters; ++i) {
                auto cur = ve::Config::get();
                if (!cur || !(equals_on_core_fields(*cur, A) ||
                              equals_on_core_fields(*cur, B))) {
                    ++bad_reads;
                    break;
                }
                if ((i & 0x7F) == 0) std::this_thread::yield();
            }
        });
    }

    // Writers
    std::vector<std::thread> writers;
    writers.reserve(Writers);
    for (int w = 0; w < Writers; ++w) {
        writers.emplace_back([&, w]{
            sync.arrive_and_wait();
            for (int i = 0; i < Iters; ++i) {
                ve::Config::set(((i + w) & 1) ? B : A);
                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }

    for (auto& t : readers) t.join();
    for (auto& t : writers) t.join();

    EXPECT_EQ(bad_reads.load(), 0)
        << "Some reader observed a mixed/torn snapshot";
}
