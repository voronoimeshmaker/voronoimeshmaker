//==============================================================================
// Name        : ut_CoreErrors.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1.0
// Description : Unit tests for CoreErrors/ErrorTraits (incl. parallel read).
//==============================================================================

#include <gtest/gtest.h>

#include <atomic>
#include <barrier>
#include <cstdint>
#include <execution>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
#include <array>
#include <algorithm>

#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/Language.h>
#include <VoronoiMeshMaker/ErrorHandling/MessageCatalog.hpp>

namespace ve = vmm::error;
using sv = std::string_view;

//------------------------------------------------------------------------------
// helpers
//------------------------------------------------------------------------------
static bool valid_sev(ve::Severity s) {
    switch (s) {
        case ve::Severity::Info:
        case ve::Severity::Warning:
        case ve::Severity::Error:
        case ve::Severity::Fatal:
            return true;
    }
    return false;
}

static std::array<ve::CoreErr, 4> all_errors() {
    return { ve::CoreErr::InvalidArgument,
             ve::CoreErr::OutOfRange,
             ve::CoreErr::NotImplemented,
             ve::CoreErr::AssertFailed };
}

//------------------------------------------------------------------------------
// 1) enum sanity & domain id
//------------------------------------------------------------------------------
TEST(CoreErrors, EnumAndDomainSanity) {
    static_assert(std::is_enum_v<ve::CoreErr>, "CoreErr must be an enum");
    SUCCEED();

    const auto dom = ve::ErrorTraits<ve::CoreErr>::domain_id();
    EXPECT_NE(dom, 0u);
}

//------------------------------------------------------------------------------
// 2) error_code() uniqueness and common domain high-bits
//------------------------------------------------------------------------------
TEST(CoreErrors, ErrorCodesAreUniqueAndShareDomainHighBits) {
    auto errs = all_errors();

    std::vector<std::uint32_t> codes;
    codes.reserve(errs.size());
    for (auto e : errs) codes.push_back(ve::error_code(e));

    auto codes_sorted = codes;
    std::sort(codes_sorted.begin(), codes_sorted.end());
    auto it = std::unique(codes_sorted.begin(), codes_sorted.end());
    EXPECT_EQ(std::distance(codes_sorted.begin(), it),
              static_cast<long>(codes.size()));

    const std::uint32_t dom = ve::ErrorTraits<ve::CoreErr>::domain_id();
    for (auto c : codes) {
        EXPECT_EQ((c >> 16), static_cast<std::uint32_t>(dom));
    }
}

//------------------------------------------------------------------------------
// 3) keys, enUS/ptBR messages
//------------------------------------------------------------------------------
TEST(CoreErrors, KeysAndMessagesExistAndAreReasonable) {
    for (auto e : all_errors()) {
        sv key  = ve::ErrorTraits<ve::CoreErr>::key(e);
        sv msgE = ve::ErrorTraits<ve::CoreErr>::enUS(e);
        sv msgP = ve::ErrorTraits<ve::CoreErr>::ptBR(e);

        EXPECT_FALSE(key.empty());
        EXPECT_FALSE(msgE.empty());
        EXPECT_FALSE(msgP.empty());
    }

    // Keys devem ser únicas
    auto errs = all_errors();
    std::vector<std::string> keys;
    keys.reserve(errs.size());
    for (auto e : errs)
        keys.emplace_back(ve::ErrorTraits<ve::CoreErr>::key(e));
    std::sort(keys.begin(), keys.end());
    auto it = std::unique(keys.begin(), keys.end());
    EXPECT_EQ(std::distance(keys.begin(), it),
              static_cast<long>(errs.size()));
}

//------------------------------------------------------------------------------
// 4) default severity
//------------------------------------------------------------------------------
TEST(CoreErrors, DefaultSeverityIsValidForAll) {
    for (auto e : all_errors()) {
        auto s = ve::ErrorTraits<ve::CoreErr>::default_severity(e);
        EXPECT_TRUE(valid_sev(s));
    }
}

//------------------------------------------------------------------------------
// 5) Message rendering with placeholder substitution
//------------------------------------------------------------------------------
TEST(CoreErrors, RenderInterpolatesPlaceholders) {
    std::string rendered_en = ve::render(
        ve::CoreErr::InvalidArgument, ve::Language::EnUS,
        {{"name", "alpha"}});
    std::string rendered_pt = ve::render(
        ve::CoreErr::InvalidArgument, ve::Language::PtBR,
        {{"name", "alpha"}});

    EXPECT_FALSE(rendered_en.empty());
    EXPECT_FALSE(rendered_pt.empty());
    EXPECT_NE(rendered_en.find("alpha"), std::string::npos);
    EXPECT_NE(rendered_pt.find("alpha"), std::string::npos);
}

//------------------------------------------------------------------------------
// 6) Parallel read: threads + barrier
//------------------------------------------------------------------------------
TEST(CoreErrors, Parallel_ReadTraitsAcrossThreads) {
    constexpr int T = 12;
    constexpr int I = 1500;

    std::barrier sync(T);
    std::atomic<int> bad{0};
    auto errs = all_errors();

    auto worker = [&] {
        sync.arrive_and_wait();
        for (int i = 0; i < I; ++i) {
            auto e = errs[static_cast<size_t>(i) % errs.size()];
            if (ve::ErrorTraits<ve::CoreErr>::key(e).empty() ||
                ve::ErrorTraits<ve::CoreErr>::enUS(e).empty() ||
                ve::ErrorTraits<ve::CoreErr>::ptBR(e).empty() ||
                !valid_sev(ve::ErrorTraits<ve::CoreErr>::default_severity(e)) ||
                (ve::error_code(e) >> 16) != ve::ErrorTraits<ve::CoreErr>::domain_id()) {
                ++bad; break;
            }
            if ((i & 0xFF) == 0) std::this_thread::yield();
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(T);
    for (int t = 0; t < T; ++t) threads.emplace_back(worker);
    for (auto& th : threads) th.join();

    EXPECT_EQ(bad.load(), 0);
}

//------------------------------------------------------------------------------
// 7) Parallel read: std::execution::par
//------------------------------------------------------------------------------
TEST(CoreErrors, Parallel_StdPar_RenderAndCode) {
    std::vector<int> ids(6000);
    for (int i = 0; i < (int)ids.size(); ++i) ids[i] = i;

    auto errs = all_errors();
    std::atomic<std::uint64_t> acc{0};

    std::for_each(std::execution::par, ids.begin(), ids.end(),
                  [&](int i) {
                      const auto e = errs[static_cast<size_t>(i) % errs.size()];
                      const auto lang = (i & 1) ? ve::Language::EnUS : ve::Language::PtBR;
                      auto s = ve::render(e, lang, {{"name", "x"}});
                      if (s.empty()) acc.fetch_add(1, std::memory_order_relaxed);
                      acc.fetch_add(ve::error_code(e), std::memory_order_relaxed);
                  });

    EXPECT_GT(acc.load(), 0u);
}

//------------------------------------------------------------------------------
// 8) Perf smoke: trait lookups & render
//------------------------------------------------------------------------------
TEST(CoreErrors, Perf_Smoke_LookupAndRender) {
    constexpr int N = 20000;
    volatile std::size_t sink = 0;

    auto errs = all_errors();
    for (int i = 0; i < N; ++i) {
        auto e = errs[static_cast<size_t>(i) % errs.size()];
        sink ^= ve::ErrorTraits<ve::CoreErr>::key(e).size();
        sink ^= ve::render(e, ve::Language::EnUS, {{"name","p"}}).size();
    }
    (void)sink;
}
