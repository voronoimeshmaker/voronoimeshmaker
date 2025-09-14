//==============================================================================
// Name        : ut_Severity.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.4
// Description : Basic sanity tests for the Severity enum + parallel read smoke.
//==============================================================================

#include <type_traits>
#include <string_view>
#include <vector>
#include <thread>
#include <barrier>
#include <atomic>
#include <execution>

#include <gtest/gtest.h>

#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------
constexpr std::string_view sev_name(ve::Severity s) {
    switch (s) {
        case ve::Severity::Info:    return "Info";
        case ve::Severity::Warning: return "Warning";
        case ve::Severity::Error:   return "Error";
        case ve::Severity::Fatal:   return "Fatal";
    }
    return {}; // unreachable if enum stays closed
}

static bool is_valid(ve::Severity s) {
    switch (s) {
        case ve::Severity::Info:
        case ve::Severity::Warning:
        case ve::Severity::Error:
        case ve::Severity::Fatal:
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------
TEST(Severity, IsEnumType) {
    static_assert(std::is_enum_v<ve::Severity>, "Severity must be an enum");
    SUCCEED();
}

TEST(Severity, EnumeratorsAreDistinct) {
    // We do not assume numeric ordering—only distinctness.
    auto info    = static_cast<int>(ve::Severity::Info);
    auto warning = static_cast<int>(ve::Severity::Warning);
    auto error   = static_cast<int>(ve::Severity::Error);
    auto fatal   = static_cast<int>(ve::Severity::Fatal);

    EXPECT_NE(info, warning);
    EXPECT_NE(info, error);
    EXPECT_NE(info, fatal);
    EXPECT_NE(warning, error);
    EXPECT_NE(warning, fatal);
    EXPECT_NE(error, fatal);
}

TEST(Severity, NoAccidentalAliasesBetweenCommonValues) {
    EXPECT_NE(static_cast<int>(ve::Severity::Warning),
              static_cast<int>(ve::Severity::Error));
    EXPECT_NE(static_cast<int>(ve::Severity::Info),
              static_cast<int>(ve::Severity::Warning));
}

TEST(Severity, NamesExistForAllEnumerators) {
    EXPECT_FALSE(sev_name(ve::Severity::Info).empty());
    EXPECT_FALSE(sev_name(ve::Severity::Warning).empty());
    EXPECT_FALSE(sev_name(ve::Severity::Error).empty());
    EXPECT_FALSE(sev_name(ve::Severity::Fatal).empty());
}

TEST(Severity, ParallelReadSmoke_ThreadsAndParAlgo) {
    // Threads: confirm we can read/inspect without races (it's an enum).
    constexpr int T = 8;
    constexpr int I = 2000;
    std::barrier sync(T);
    std::atomic<int> bad{0};
    std::vector<std::thread> th;
    th.reserve(T);

    for (int t = 0; t < T; ++t) {
        th.emplace_back([&] {
            sync.arrive_and_wait();
            for (int i = 0; i < I; ++i) {
                // Round-robin through the four values
                ve::Severity s = static_cast<ve::Severity>(i % 4);
                if (!is_valid(s) || sev_name(s).empty()) {
                    ++bad; break;
                }
                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }
    for (auto &x : th) x.join();
    EXPECT_EQ(bad.load(), 0);

    // std::execution::par: same idea with a small array
    std::vector<ve::Severity> arr{
        ve::Severity::Info, ve::Severity::Warning,
        ve::Severity::Error, ve::Severity::Fatal
    };
    std::atomic<int> ok{0};
    std::for_each(std::execution::par, arr.begin(), arr.end(),
                  [&](ve::Severity s){
                      if (is_valid(s) && !sev_name(s).empty()) ++ok;
                  });
    EXPECT_EQ(ok.load(), static_cast<int>(arr.size()));
}
