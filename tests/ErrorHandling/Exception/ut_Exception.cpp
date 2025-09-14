//==============================================================================
// File        : ut_Exception.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.2.2
// Description : Tests for VMMException:
//               - code(), severity(), what(), message(), location()
//               - copy/move stability
//               - parallel creation & inspection (threads, <execution>, barrier)
//               - test-only definition of ErrorConfig() to fix link
// License     : GNU GPL v3
//==============================================================================

//------------------------------------------------------------------------------
// c++ includes
//------------------------------------------------------------------------------
#include <barrier>
#include <execution>

//------------------------------------------------------------------------------
// external includes
//------------------------------------------------------------------------------
#include <gtest/gtest.h>
#include <gmock/gmock.h>


//------------------------------------------------------------------------------
// VoronoiMeshMaker includes
//------------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

namespace ve = vmm::error;
using ::testing::HasSubstr;
using sv = std::string_view;

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------
static bool is_valid(ve::Severity s) {
    return s == ve::Severity::Info || s == ve::Severity::Warning ||
           s == ve::Severity::Error || s == ve::Severity::Fatal;
}

// RAII to temporarily switch global language config.
namespace {
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
} // namespace

//------------------------------------------------------------------------------
// 1) Basic: carries code/severity/messages/location
//------------------------------------------------------------------------------
TEST(VMMException, CarriesCodeSeverityMessageAndLocation) {
    ScopedLanguage lang_guard(ve::Language::PtBR); // message() in pt-BR
    constexpr ve::CoreErr err = ve::CoreErr::InvalidArgument;

    ve::VMMException ex(err, {{"name", "beta"}});

    EXPECT_EQ(ex.code(), ve::error_code(err));
    EXPECT_EQ(ex.severity(),
              ve::ErrorTraits<ve::CoreErr>::default_severity(err));

    std::string w = ex.what();     // English (stable)
    std::string m = ex.message();  // Localized (runtime)
    EXPECT_FALSE(w.empty());
    EXPECT_FALSE(m.empty());
    EXPECT_NE(w.find("beta"), std::string::npos);
    EXPECT_NE(m.find("beta"), std::string::npos);

    auto loc = ex.location();
    EXPECT_GT(loc.line(), 0u);
    EXPECT_FALSE(std::string(loc.file_name()).empty());
}

//------------------------------------------------------------------------------
// 2) Other enumerators: smoke
//------------------------------------------------------------------------------
TEST(VMMException, WorksForDifferentErrorKinds) {
    ve::VMMException ex1(ve::CoreErr::OutOfRange, {{"index", "5"}});
    EXPECT_EQ(ex1.code(), ve::error_code(ve::CoreErr::OutOfRange));
    EXPECT_FALSE(std::string(ex1.what()).empty());

    ve::VMMException ex2(ve::CoreErr::NotImplemented);
    EXPECT_EQ(ex2.code(), ve::error_code(ve::CoreErr::NotImplemented));
    EXPECT_FALSE(std::string(ex2.message()).empty());

    ve::VMMException ex3(ve::CoreErr::AssertFailed);
    EXPECT_EQ(ex3.code(), ve::error_code(ve::CoreErr::AssertFailed));
    EXPECT_FALSE(std::string(ex3.what()).empty());
}

//------------------------------------------------------------------------------
// 3) what() must be English; message() is localized
//------------------------------------------------------------------------------
TEST(VMMException, WhatIsEnglishMessageIsLocalized) {
    // pt-BR
    {
        ScopedLanguage g(ve::Language::PtBR);
        ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name","x"}});
        EXPECT_THAT(ex.what(), HasSubstr("Invalid argument"));
        EXPECT_THAT(ex.message(), HasSubstr("Argumento inválido"));
    }
    // en-US
    {
        ScopedLanguage g(ve::Language::EnUS);
        ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name","x"}});
        EXPECT_THAT(ex.what(), HasSubstr("Invalid argument"));
        EXPECT_THAT(ex.message(), HasSubstr("Invalid argument"));
    }
}

//------------------------------------------------------------------------------
// 4) Copy/move preserve texts
//------------------------------------------------------------------------------
TEST(VMMException, CopyAndMoveKeepStableTexts) {
    ScopedLanguage lang_guard(ve::Language::PtBR);
    ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name", "gamma"}});
    std::string what_before = ex.what();
    std::string msg_before  = ex.message();

    ve::VMMException ex_copy = ex;         // copy
    EXPECT_EQ(ex_copy.code(), ex.code());
    EXPECT_FALSE(std::string(ex_copy.what()).empty());
    EXPECT_FALSE(std::string(ex_copy.message()).empty());

    ve::VMMException ex_moved = std::move(ex_copy); // move
    EXPECT_EQ(ex_moved.code(), ve::error_code(ve::CoreErr::InvalidArgument));
    EXPECT_NE(std::string(ex_moved.what()).find("gamma"), std::string::npos);
    EXPECT_NE(std::string(ex_moved.message()).find("gamma"), std::string::npos);

    EXPECT_FALSE(what_before.empty());
    EXPECT_FALSE(msg_before.empty());
}

//------------------------------------------------------------------------------
// 5) Throw/catch as std::exception
//------------------------------------------------------------------------------
TEST(VMMException, ThrowsAndCatchesAsStdException) {
    try {
        throw ve::VMMException(ve::CoreErr::InvalidArgument, {{"name","z"}});
    } catch (const std::exception& e) {
        std::string w = e.what();
        EXPECT_FALSE(w.empty());
        EXPECT_NE(w.find("Invalid argument"), std::string::npos);
    }
}

//------------------------------------------------------------------------------
// 6) Parallel (threads): mass create/inspect with barrier
//------------------------------------------------------------------------------
TEST(VMMException, Parallel_Threads_CreateAndInspect) {
    constexpr int Readers = 12;
    constexpr int Iters   = 1500;
    std::barrier sync(Readers);
    std::atomic<int> bad{0};

    // Fix language to avoid global mutations during the test
    ScopedLanguage g(ve::Language::EnUS);

    auto worker = [&] {
        sync.arrive_and_wait();
        for (int i = 0; i < Iters; ++i) {
            ve::VMMException ex(ve::CoreErr::InvalidArgument,
                                {{"name","thr"}});
            if (ex.code() != ve::error_code(ve::CoreErr::InvalidArgument) ||
                !is_valid(ex.severity()) ||
                std::string(ex.what()).empty() ||
                std::string(ex.message()).empty() ||
                ex.location().line() == 0) {
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
}

//------------------------------------------------------------------------------
// 7) Parallel (std::execution::par): batch create and sum codes
//------------------------------------------------------------------------------
TEST(VMMException, Parallel_StdPar_BatchCreate) {
    // Pre-generate ids to avoid noisy allocations inside the loop
    constexpr int N = 5000;
    std::vector<int> ids(N);
    for (int i = 0; i < N; ++i) ids[i] = i;

    ScopedLanguage g(ve::Language::EnUS);

    std::atomic<std::uint64_t> sum{0};
    std::for_each(std::execution::par, ids.begin(), ids.end(),
                  [&](int i) {
                      ve::VMMException ex(ve::CoreErr::OutOfRange,
                                          {{"index", std::to_string(i)}});
                      // ex.code() returns a 32-bit unsigned int.
                      sum += static_cast<std::uint64_t>(ex.code());
                  });

    EXPECT_GT(sum.load(), 0u);
}

//------------------------------------------------------------------------------
// 8) Perf smoke (not a benchmark): create and read what() many times
//------------------------------------------------------------------------------
TEST(VMMException, Perf_Smoke_CreateAndWhat) {
    constexpr int N = 20000;
    volatile std::size_t sink = 0; // prevent aggressive optimization

    ScopedLanguage g(ve::Language::EnUS);

    for (int i = 0; i < N; ++i) {
        ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name","p"}});
        sink ^= std::string(ex.what()).size();
    }
    (void)sink;
}

//------------------------------------------------------------------------------
// 9) Messages must interpolate placeholders (name/index)
//------------------------------------------------------------------------------
TEST(VMMException, PlaceholdersAreInterpolated) {
    {
        ve::VMMException ex(ve::CoreErr::InvalidArgument, {{"name","alpha"}});
        EXPECT_NE(std::string(ex.what()).find("alpha"), std::string::npos);
        EXPECT_NE(std::string(ex.message()).find("alpha"), std::string::npos);
    }
    {
        ve::VMMException ex(ve::CoreErr::OutOfRange, {{"index","7"}});
        EXPECT_NE(std::string(ex.what()).find("7"), std::string::npos);
        EXPECT_NE(std::string(ex.message()).find("7"), std::string::npos);
    }
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
