// -----------------------------------------------------------------------------
// ut_integration.cpp   
// End-to-end parallel flow:
//  - A writer thread toggles Config::language (EnUS <-> PtBR)
//  - N worker threads concurrently:
//      * log with VMM_ERROR(CoreErr::InvalidArgument, {name=...})
//      * occasionally trigger VMM_REQUIRE(false, ...) and catch the exception
//  - Each worker flushes its per-thread log and we validate integrity
//
// One test executable dedicated to an integration scenario.
// Comments/messages in English.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <barrier>

// -----------------------------------------------------------------------------
//  include gtest
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/Macros.h>

namespace ve = vmm::error;

// RAII guard for config (restore at scope exit)
struct ScopedConfig {
    ve::ErrorConfig old_;
    explicit ScopedConfig(const ve::ErrorConfig& next) {
        if (auto cur = ve::Config::get()) old_ = *cur;
        ve::Config::set(next);
    }
    ~ScopedConfig() { ve::Config::set(old_); }
};

// Convenience: clear this thread's buffer
static void clean_thread_buffer() { (void)ve::ErrorManager::flush(); }

TEST(Integration, Parallel_LogThenThrow_UnderLanguageChurn) {
    // Parameters
    constexpr int T = 8;           // worker threads
    constexpr int M = 300;         // logs per thread (also cap)
    constexpr int K = 50;          // every K iterations an odd thread throws

    // Prepare two configs with different locales
    auto base = ve::Config::get();
    ASSERT_TRUE(static_cast<bool>(base));
    ve::ErrorConfig A = *base; A.language = ve::Language::EnUS; A.thread_buffer_cap = M;
    ve::ErrorConfig B = *base; B.language = ve::Language::PtBR; B.thread_buffer_cap = M;

    // Publish starting state and ensure cap is large enough
    ScopedConfig guard(A);

    // Synchronize start among writer + workers
    std::barrier sync(T + 1);

    // Per-thread results
    std::vector<std::vector<ve::ErrorRecord>> per_thread_logs(T);
    std::atomic<int> total_requires_thrown{0};

    // Spawn writer flipping language during the test
    std::thread writer([&]{
        sync.arrive_and_wait();
        // Flip a number of times larger than workers' inner loop to interleave well
        for (int i = 0; i < M * 4; ++i) {
            ve::Config::set((i & 1) ? B : A);
            if ((i & 0x1F) == 0) std::this_thread::yield();
        }
    });

    // Spawn workers
    std::vector<std::thread> workers;
    workers.reserve(T);
    for (int t = 0; t < T; ++t) {
        workers.emplace_back([&, t]{
            clean_thread_buffer();
            sync.arrive_and_wait();

            // Work loop: log M records; on odd threads, every K iterations throw via REQUIRE
            for (int i = 0; i < M; ++i) {
                // Non-throwing path: record an error with a known placeholder {name}
                // NOTE: VMM_ERROR uses current Config::language internally.
                VMM_ERROR(ve::CoreErr::InvalidArgument,
                          {{"name", "tid=" + std::to_string(t) + ";i=" + std::to_string(i)}});

                // Occasionally force a throw on odd threads; catch locally
                if ((t & 1) && (i % K == 0)) {
                    try {
                        VMM_REQUIRE(false, ve::CoreErr::InvalidArgument,
                                    {{"name", "tid=" + std::to_string(t) + ";REQUIRE"}});
                    } catch (const ve::VMMException&) {
                        total_requires_thrown.fetch_add(1, std::memory_order_relaxed);
                    }
                }
            }

            // Flush this thread's log and keep for validation
            per_thread_logs[t] = ve::ErrorManager::flush();
        });
    }

    for (auto& th : workers) th.join();
    writer.join();

    // ---- Validations (single-threaded assertions) ---------------------------

    // 1) No drops: cap == M, so each worker must have exactly M records
    for (int t = 0; t < T; ++t) {
        ASSERT_EQ(per_thread_logs[t].size(), static_cast<size_t>(M))
            << "Per-thread cap is " << M << ", but thread " << t << " has "
            << per_thread_logs[t].size();
    }

    // 2) Messages integrity: each record must contain this thread's tag "tid=t"
    for (int t = 0; t < T; ++t) {
        const std::string tag = "tid=" + std::to_string(t) + ";";
        for (const auto& r : per_thread_logs[t]) {
            EXPECT_NE(r.message.find(tag), std::string::npos)
                << "Missing per-thread tag in message: " << r.message;
        }
    }

    // 3) The number of REQUIRE throws equals the count of (odd threads) * ceil(M/K)
    const int odd_threads = T / 2; // half of 0..T-1 are odd
    const int throws_per_odd = (M + K - 1) / K;
    const int expected_total_throws = odd_threads * throws_per_odd;
    EXPECT_EQ(total_requires_thrown.load(), expected_total_throws);
}
