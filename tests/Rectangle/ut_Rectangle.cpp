// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>


// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>

using namespace vmm::b2d;

namespace {

double polygon_area(const std::vector<Point2>& v) {
    const std::size_t n = v.size();
    if (n < 3) return 0.0;
    long double A = 0.0L;
    for (std::size_t i = 0; i < n; ++i) {
        const auto& p = v[i];
        const auto& q = v[(i + 1) % n];
        A += static_cast<long double>(p.x) * static_cast<long double>(q.y)
           - static_cast<long double>(p.y) * static_cast<long double>(q.x);
    }
    A *= 0.5L;
    return static_cast<double>(A);
}

TEST(Rectangle, DefaultsAndArea) {
    Rectangle R{}; // w=1, h=1; centrado na origem; eixo-alinhado
    EXPECT_TRUE(R.has_positive_dims());

    PolygonizePolicy pol{};
    auto verts_arr = R.polygonize(pol);
    std::vector<Point2> verts(verts_arr.begin(), verts_arr.end());

    // Área deve ser 1*1 = 1 (CCW positiva)
    EXPECT_NEAR(polygon_area(verts), 1.0, 1e-12);
}

TEST(Rectangle, PolygonizeInto_NoAlloc) {
    Rectangle R{ .w = 4.0, .h = 2.0 };
    std::array<Point2, 8> buf{};

    for (std::size_t i = 4; i < buf.size(); ++i) {
        buf[i].x = std::numeric_limits<Real>::quiet_NaN();
        buf[i].y = std::numeric_limits<Real>::quiet_NaN();
    }

    std::span<Point2> dst(buf.data(), buf.size());
    R.polygonize_into(dst);

    // Os quatro primeiros preenchidos e finitos
    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_TRUE(std::isfinite(static_cast<double>(buf[i].x)));
        EXPECT_TRUE(std::isfinite(static_cast<double>(buf[i].y)));
    }
    // Demais permanecem como estavam (NaN)
    for (std::size_t i = 4; i < buf.size(); ++i) {
        EXPECT_TRUE(std::isnan(static_cast<double>(buf[i].x)));
        EXPECT_TRUE(std::isnan(static_cast<double>(buf[i].y)));
    }
}

TEST(Rectangle, BBoxContainsVertices) {
    Rectangle R{ .w = 3.0, .h = 2.0 };
    PolygonizePolicy pol{};
    auto v = R.polygonize(pol);

    const Real hx = R.w * Real{0.5};
    const Real hy = R.h * Real{0.5};

    for (const auto& p : v) {
        EXPECT_LE(static_cast<double>(p.x), static_cast<double>( hx) + 1e-12);
        EXPECT_GE(static_cast<double>(p.x), static_cast<double>(-hx) - 1e-12);
        EXPECT_LE(static_cast<double>(p.y), static_cast<double>( hy) + 1e-12);
        EXPECT_GE(static_cast<double>(p.y), static_cast<double>(-hy) - 1e-12);
    }
}

TEST(Rectangle, Determinism) {
    Rectangle R{ .w = 2.5, .h = 1.5 };
    PolygonizePolicy pol{};

    auto a = R.polygonize(pol);
    auto b = R.polygonize(pol);

    // Mesmos vértices, mesma ordem
    for (int i = 0; i < 4; ++i) {
        EXPECT_DOUBLE_EQ(static_cast<double>(a[i].x), static_cast<double>(b[i].x));
        EXPECT_DOUBLE_EQ(static_cast<double>(a[i].y), static_cast<double>(b[i].y));
    }
}

TEST(RectangleWithBoundaryData, AppendAndAccess) {
    Rectangle R{ .w = 2.0, .h = 2.0 };
    PolygonizePolicy pol{};
    auto ring = R.polygonize(pol);

    Boundary2DData data;
    data.reserve(8, 2);
    data.append_ring(std::span<const Point2>(ring.data(), ring.size()),
                     LoopKind::Outer, RegionId{0});

    // Deve haver 1 ring com 4 vértices
    EXPECT_EQ(static_cast<std::size_t>(data.ring_count()), 1u);
    EXPECT_EQ(static_cast<std::size_t>(data.vertex_count()), 4u);

    // Acesso ao ring
    std::span<const Point2> out{};
    out = data.ring_at(Index{0});
    EXPECT_FALSE(out.empty());
    EXPECT_EQ(out.size(), 4u);
}

} // namespace
