//==============================================================================
// File        : ut_CoreErrors.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.1
// Description : Unified tests for CoreErrors (GTest + GMock + parallel checks)
// License     : GNU GPL v3
//==============================================================================

//------------------------------------------------------------------------------
// c++ includes
//------------------------------------------------------------------------------
// #include <atomic>
#include <cstdint>
#include <execution>
#include <string>
#include <string_view>
#include <thread>
#include <vector>


//------------------------------------------------------------------------------
// external includes
//------------------------------------------------------------------------------
#include <gtest/gtest.h>
#include <gmock/gmock.h>


//------------------------------------------------------------------------------
// VoronoiMeshMaker includes
//------------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

using namespace vmm::error;
using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;
using sv = std::string_view;

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------
static bool is_valid(Severity s) {
    return s == Severity::Info || s == Severity::Warning ||
           s == Severity::Error || s == Severity::Fatal;
}

//------------------------------------------------------------------------------
// 1) Enum layout / counts (kept as in original tests)
//------------------------------------------------------------------------------
TEST(CoreErrors, EnumValues) {
    EXPECT_EQ(static_cast<std::uint16_t>(CoreErr::InvalidArgument), 1);
    EXPECT_EQ(static_cast<std::uint16_t>(CoreErr::OutOfRange),      2);
    EXPECT_EQ(static_cast<std::uint16_t>(CoreErr::NotImplemented),  3);
    EXPECT_EQ(static_cast<std::uint16_t>(CoreErr::AssertFailed),    4);
    EXPECT_EQ(static_cast<std::uint16_t>(CoreErr::_Count),          5);
}

//------------------------------------------------------------------------------
// 2) Domain info
//------------------------------------------------------------------------------
TEST(CoreErrors, DomainInfo) {
    EXPECT_EQ(ErrorTraits<CoreErr>::domain_id(), 0x0001);
    auto name = ErrorTraits<CoreErr>::domain_name();
    EXPECT_TRUE(name == sv{"Core"});
}

//------------------------------------------------------------------------------
// 3) Default severities
//------------------------------------------------------------------------------
TEST(CoreErrors, DefaultSeverities) {
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(
                  CoreErr::InvalidArgument),
              Severity::Error);
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(
                  CoreErr::OutOfRange),
              Severity::Error);
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(
                  CoreErr::NotImplemented),
              Severity::Warning);
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(
                  CoreErr::AssertFailed),
              Severity::Fatal);
}

//------------------------------------------------------------------------------
// 4) Keys + messages (EN/PT)
//------------------------------------------------------------------------------
TEST(CoreErrors, KeysAndMessages) {
    EXPECT_EQ(ErrorTraits<CoreErr>::key(CoreErr::InvalidArgument),
              sv{"CORE_INVALID_ARG"});
    EXPECT_EQ(ErrorTraits<CoreErr>::key(CoreErr::OutOfRange),
              sv{"CORE_OUT_OF_RANGE"});
    EXPECT_EQ(ErrorTraits<CoreErr>::key(CoreErr::NotImplemented),
              sv{"CORE_NOT_IMPLEMENTED"});
    EXPECT_EQ(ErrorTraits<CoreErr>::key(CoreErr::AssertFailed),
              sv{"CORE_ASSERT_FAILED"});

    EXPECT_EQ(ErrorTraits<CoreErr>::enUS(CoreErr::InvalidArgument),
              sv{"Invalid argument: {name}."});
    EXPECT_EQ(ErrorTraits<CoreErr>::enUS(CoreErr::OutOfRange),
              sv{"Index out of range: {index}."});
    EXPECT_EQ(ErrorTraits<CoreErr>::enUS(CoreErr::NotImplemented),
              sv{"Feature not implemented."});
    EXPECT_EQ(ErrorTraits<CoreErr>::enUS(CoreErr::AssertFailed),
              sv{"Assertion failed."});

    EXPECT_EQ(ErrorTraits<CoreErr>::ptBR(CoreErr::InvalidArgument),
              sv{"Argumento inválido: {name}."});
    EXPECT_EQ(ErrorTraits<CoreErr>::ptBR(CoreErr::OutOfRange),
              sv{"Índice fora do intervalo: {index}."});
    EXPECT_EQ(ErrorTraits<CoreErr>::ptBR(CoreErr::NotImplemented),
              sv{"Recurso não implementado."});
    EXPECT_EQ(ErrorTraits<CoreErr>::ptBR(CoreErr::AssertFailed),
              sv{"Falha de asserção."});
}

//------------------------------------------------------------------------------
// 5) Detail access (table entries)
//------------------------------------------------------------------------------
TEST(CoreErrors, DetailAccess) {
    const auto &inv = detail::get_info(CoreErr::InvalidArgument);
    EXPECT_EQ(inv.key, sv{"CORE_INVALID_ARG"});
    EXPECT_EQ(inv.severity, Severity::Error);

    const auto &ni = detail::get_info(CoreErr::NotImplemented);
    EXPECT_EQ(ni.key, sv{"CORE_NOT_IMPLEMENTED"});
    EXPECT_EQ(ni.severity, Severity::Warning);
}

//------------------------------------------------------------------------------
// 6) Bounds safety / coverage over whole enum
//------------------------------------------------------------------------------
TEST(CoreErrors, BoundsSafety) {
    for (std::uint16_t i = 1;
         i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
        auto e   = static_cast<CoreErr>(i);
        auto key = ErrorTraits<CoreErr>::key(e);
        auto sev = ErrorTraits<CoreErr>::default_severity(e);
        EXPECT_FALSE(key.empty());
        EXPECT_TRUE(is_valid(sev));
    }
}

//------------------------------------------------------------------------------
// 7) Thread-safety: benign concurrent readers (manual threads)
//------------------------------------------------------------------------------
TEST(CoreErrors, ThreadSafeReaders) {
    constexpr int nthreads = 4;
    constexpr int iters    = 100;
    std::atomic<int> ok{0};

    auto worker = [&]() {
        for (int k = 0; k < iters; ++k) {
            for (std::uint16_t i = 1;
                 i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
                CoreErr e = static_cast<CoreErr>(i);
                auto ksv  = ErrorTraits<CoreErr>::key(e);
                auto sev  = ErrorTraits<CoreErr>::default_severity(e);
                if (!ksv.empty() && is_valid(sev)) ++ok;
            }
        }
    };

    std::vector<std::thread> ts;
    for (int t = 0; t < nthreads; ++t) ts.emplace_back(worker);
    for (auto &t : ts) t.join();

    const int expected =
        nthreads * iters *
        (static_cast<std::uint16_t>(CoreErr::_Count) - 1);
    EXPECT_EQ(ok.load(), expected);
}

//------------------------------------------------------------------------------
// 8) Parallel algorithms (std::execution::par): readers
//------------------------------------------------------------------------------
TEST(CoreErrors, ParallelReaders_StdPar) {
    std::vector<std::uint16_t> ids;
    for (std::uint16_t i = 1;
         i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
        ids.push_back(i);
    }

    std::atomic<int> ok{0};
    std::for_each(std::execution::par, ids.begin(), ids.end(),
                  [&](std::uint16_t i) {
                      CoreErr e = static_cast<CoreErr>(i);
                      auto ksv  = ErrorTraits<CoreErr>::key(e);
                      auto sev  = ErrorTraits<CoreErr>::default_severity(e);
                      if (!ksv.empty() && is_valid(sev)) ++ok;
                  });

    EXPECT_EQ(ok.load(), static_cast<int>(ids.size()));
}

//------------------------------------------------------------------------------
// 9) Determinism smoke: parallel pass repeated should match
//------------------------------------------------------------------------------
TEST(CoreErrors, ParallelDeterminism_Smoke) {
    auto run = [] {
        std::vector<std::uint16_t> ids;
        for (std::uint16_t i = 1;
             i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
            ids.push_back(i);
        }
        std::vector<std::string> keys(ids.size());
        std::for_each(std::execution::par, ids.begin(), ids.end(),
                      [&](std::uint16_t i) {
                          CoreErr e = static_cast<CoreErr>(i);
                          keys[i - 1] =
                              std::string(ErrorTraits<CoreErr>::key(e));
                      });
        return keys;
    };

    auto a = run();
    auto b = run();
    ASSERT_EQ(a.size(), b.size());
    EXPECT_EQ(a, b);
}

//------------------------------------------------------------------------------
// 10) Memory patterns: dynamic allocation + no leaks
//------------------------------------------------------------------------------
TEST(CoreErrors, NoLeaks_DynamicUse) {
    auto *acc = new std::vector<sv>;
    for (std::uint16_t i = 1;
         i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
        acc->push_back(ErrorTraits<CoreErr>::key(
            static_cast<CoreErr>(i)));
    }
    EXPECT_EQ(acc->size(),
              static_cast<std::size_t>(
                  static_cast<std::uint16_t>(CoreErr::_Count) - 1));
    delete acc;  // Valgrind should report no leaks here.
}

//------------------------------------------------------------------------------
// 11) Stress test: build many small strings
//------------------------------------------------------------------------------
TEST(CoreErrors, StressStrings) {
    constexpr int N = 500;
    std::vector<std::string> out;
    out.reserve(N * (static_cast<std::uint16_t>(CoreErr::_Count) - 1));

    for (int r = 0; r < N; ++r) {
        for (std::uint16_t i = 1;
             i < static_cast<std::uint16_t>(CoreErr::_Count); ++i) {
            CoreErr e = static_cast<CoreErr>(i);
            out.emplace_back(
                std::string(ErrorTraits<CoreErr>::key(e)));
        }
    }
    EXPECT_EQ(out.size(),
              static_cast<std::size_t>(N) *
                  (static_cast<std::uint16_t>(CoreErr::_Count) - 1));
}

//------------------------------------------------------------------------------
// 12) Tiny perf smoke (not a benchmark): constant lookups
//------------------------------------------------------------------------------
TEST(CoreErrors, Perf_Smoke) {
    constexpr int N = 10000;

    // Use a volatile pointer so the loop isn't optimized away.
    // (key() returns a string_view to static storage.)
    volatile const char* sink = nullptr;

    for (int i = 0; i < N; ++i) {
        sink = ErrorTraits<CoreErr>::key(
                   CoreErr::InvalidArgument).data();
    }

    (void)sink; // keep the side-effect
}

//------------------------------------------------------------------------------
// 13) Google Mock: example integration with a mockable sink
//------------------------------------------------------------------------------
class MockErrorSink {
public:
    MOCK_METHOD(void, handle,
        (CoreErr code, Severity sev, std::string_view msg));
    MOCK_METHOD(bool, should_log, (Severity sev));
};

TEST(CoreErrors, GMock_HandlerFlow) {
    MockErrorSink sink;

    EXPECT_CALL(sink, should_log(Severity::Error))
        .WillOnce(Return(true));
    EXPECT_CALL(sink,
        handle(CoreErr::InvalidArgument,
               Severity::Error,
               HasSubstr("Invalid")))
        .Times(1);

    if (sink.should_log(Severity::Error)) {
        sink.handle(CoreErr::InvalidArgument, Severity::Error,
                    ErrorTraits<CoreErr>::enUS(
                        CoreErr::InvalidArgument));
    }
}

TEST(CoreErrors, GMock_FilterFatal) {
    MockErrorSink sink;

    EXPECT_CALL(sink, should_log(Severity::Fatal))
        .WillOnce(Return(false));
    EXPECT_CALL(sink, handle(_, _, _)).Times(0);

    if (sink.should_log(Severity::Fatal)) {
        sink.handle(CoreErr::AssertFailed, Severity::Fatal,
                    "Should not log");
    }
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
