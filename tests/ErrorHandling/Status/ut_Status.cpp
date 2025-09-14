//==============================================================================
// Name        : ut_Status.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.4
// Description : Tests for Status and StatusOr.
//               One test executable dedicated to this module.
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
#include <string>
#include <utility>
#include <thread>
#include <vector>
#include <atomic>
#include <barrier>

//==============================================================================
//      External libraries
//==============================================================================
#include <gtest/gtest.h>

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/Status.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// Tests: Status
//------------------------------------------------------------------------------
TEST(Status, OkIsOkAndHasZeroCode) {
    auto st = ve::Status::Ok();
    EXPECT_TRUE(st.ok());
    EXPECT_EQ(st.code(), 0u);
}

TEST(Status, ErrorCarriesFields) {
    ve::Status st(42u, "Bad thing happened", ve::Severity::Error);
    EXPECT_FALSE(st.ok());
    EXPECT_EQ(st.code(), 42u);
    EXPECT_EQ(st.message(), "Bad thing happened");
    EXPECT_EQ(st.severity(), ve::Severity::Error);
}

TEST(Status, CopyAndMoveSemantics) {
    ve::Status s1(7u, "X", ve::Severity::Warning);

    ve::Status s2 = s1;  // copy
    EXPECT_EQ(s2.code(), 7u);
    EXPECT_EQ(s2.message(), "X");
    EXPECT_EQ(s2.severity(), ve::Severity::Warning);

    ve::Status s3 = std::move(s1);  // move
    EXPECT_EQ(s3.code(), 7u);
    EXPECT_EQ(s3.message(), "X");
    EXPECT_EQ(s3.severity(), ve::Severity::Warning);
}

//------------------------------------------------------------------------------
// Tests: StatusOr<T>
//------------------------------------------------------------------------------
TEST(StatusOr, HoldsValue) {
    ve::StatusOr<int> so(7);
    EXPECT_TRUE(so.ok());
    EXPECT_EQ(so.value(), 7);
}

TEST(StatusOr, HoldsErrorStatus) {
    ve::Status err(2u, "nope", ve::Severity::Warning);
    ve::StatusOr<int> so(err);
    EXPECT_FALSE(so.ok());
    EXPECT_EQ(so.status().code(), 2u);
    EXPECT_EQ(so.status().severity(), ve::Severity::Warning);
    EXPECT_EQ(so.status().message(), "nope");
}

TEST(StatusOr, CopyMoveValueType) {
    struct P {
        int a{};
        std::string s;
    };

    ve::StatusOr<P> so(P{10, "ten"});
    ASSERT_TRUE(so.ok());

    auto p1 = so.value();  // copy-out
    EXPECT_EQ(p1.a, 10);
    EXPECT_EQ(p1.s, "ten");

    P p2 = std::move(so.value());  // move-out (ok em muitas impls; aqui é smoke)
    EXPECT_EQ(p2.a, 10);
    EXPECT_EQ(p2.s, "ten");
}

//------------------------------------------------------------------------------
// Extras leves: criação/inspeção em paralelo (somente leitura/criação)
// Não alteram a API nem a semântica; ajudam a flagrar problemas óbvios
// sob concorrência de uso comum.
//------------------------------------------------------------------------------
TEST(Status, Parallel_CreateAndInspect) {
    constexpr int T = 8;
    constexpr int N = 2000;

    std::barrier sync(T);
    std::atomic<int> bad{0};
    std::vector<std::thread> th;
    th.reserve(T);

    for (int t = 0; t < T; ++t) {
        th.emplace_back([&, t] {
            sync.arrive_and_wait();
            for (int i = 0; i < N; ++i) {
                ve::Status ok = ve::Status::Ok();
                if (!ok.ok() || ok.code() != 0u) { ++bad; break; }

                ve::Status err(static_cast<uint32_t>(t + 1),
                               "e", ve::Severity::Error);
                if (err.ok() || err.code() != static_cast<uint32_t>(t + 1) ||
                    err.message().empty()) {
                    ++bad; break;
                }
                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }
    for (auto& x : th) x.join();
    EXPECT_EQ(bad.load(), 0);
}

TEST(StatusOr, Parallel_ValueAndError) {
    constexpr int T = 6;
    constexpr int N = 1500;

    std::barrier sync(T);
    std::atomic<int> okcnt{0}, errcnt{0};
    std::vector<std::thread> th;
    th.reserve(T);

    for (int t = 0; t < T; ++t) {
        th.emplace_back([&, t] {
            sync.arrive_and_wait();
            for (int i = 0; i < N; ++i) {
                ve::StatusOr<int> v(i);
                if (v.ok() && v.value() == i) ++okcnt;

                ve::Status st(static_cast<uint32_t>(100 + t),
                               "x", ve::Severity::Warning);
                ve::StatusOr<int> e(st);
                if (!e.ok() && e.status().code() == static_cast<uint32_t>(100 + t))
                    ++errcnt;

                if ((i & 0xFF) == 0) std::this_thread::yield();
            }
        });
    }
    for (auto& x : th) x.join();

    EXPECT_EQ(okcnt.load(), T * N);
    EXPECT_EQ(errcnt.load(), T * N);
}
