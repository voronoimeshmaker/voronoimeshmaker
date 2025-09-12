//==============================================================================
// File        : ut_rectangle.cpp
// Purpose     : Unit tests for Rectangle and its interaction with
//               Boundary2DData (append/access).
// Author      : Joao Flavio Vieira de Vasconcellos
// License     : GNU GPL v3
//==============================================================================


//------------------------------------------------------------------------------
//      gtest includes
//------------------------------------------------------------------------------
#include <gtest/gtest.h>


//------------------------------------------------------------------------------
//      VoronoiMeshMaker includes
//------------------------------------------------------------------------------
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>
#include <VoronoiMeshMaker/Boundary2D/Policies/PolygonizePolicy.hpp>
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>

using namespace vmm::b2d;

namespace {

// Shoelace area (CCW positive)
double polygon_area(const std::vector<Point2>& v) {
    const std::size_t n = v.size();
    double a = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const auto& p = v[i];
        const auto& q = v[(i + 1) % n];
        a += static_cast<double>(p.x) * static_cast<double>(q.y) -
             static_cast<double>(q.x) * static_cast<double>(p.y);
    }
    return 0.5 * a;
}

}  // namespace

TEST(Rectangle, DefaultsAndArea) {
    Rectangle R{};  // cx=0,cy=0,w=1,h=1,theta=0
    EXPECT_TRUE(R.has_positive_dims());

    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);

    ASSERT_EQ(verts.size(), 4u);
    EXPECT_NEAR(polygon_area(verts), 1.0, 1e-12);

    // First vertex at (-0.5,-0.5) when theta=0
    EXPECT_NEAR(verts[0].x, -0.5, 1e-12);
    EXPECT_NEAR(verts[0].y, -0.5, 1e-12);
}

TEST(Rectangle, RotationKeepsArea) {
    Rectangle R{.cx = 0.0, .cy = 0.0, .w = 3.0, .h = 2.0,
                .theta = static_cast<Real>(std::numbers::pi_v<double> / 3.0)};
    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);

    ASSERT_EQ(verts.size(), 4u);
    EXPECT_NEAR(polygon_area(verts), 6.0, 1e-10);
}

TEST(Rectangle, PolygonizeInto_NoAlloc) {
    Rectangle R{.cx = 1.0, .cy = -2.0, .w = 4.0, .h = 2.0, .theta = 0.25};

    // Buffer larger than needed; only first 4 entries must be written.
    std::array<Point2, 8> buf{};
    for (std::size_t i = 4; i < buf.size(); ++i) {
        buf[i].x = std::numeric_limits<Real>::quiet_NaN();
        buf[i].y = std::numeric_limits<Real>::quiet_NaN();
    }

    std::span<Point2> dst(buf.data(), buf.size());
    R.polygonize_into(dst);

    // First 4 entries must be finite
    for (int i = 0; i < 4; ++i) {
        EXPECT_TRUE(std::isfinite(static_cast<double>(buf[i].x)));
        EXPECT_TRUE(std::isfinite(static_cast<double>(buf[i].y)));
    }
    // Remaining entries untouched (still NaN)
    for (std::size_t i = 4; i < buf.size(); ++i) {
        EXPECT_TRUE(std::isnan(static_cast<double>(buf[i].x)));
        EXPECT_TRUE(std::isnan(static_cast<double>(buf[i].y)));
    }
}

TEST(Rectangle, BBoxContainsVertices) {
    Rectangle R{.cx = 0.0, .cy = 0.0, .w = 3.0, .h = 2.0, .theta = 0.7};
    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);
    auto b = R.bbox();

    ASSERT_FALSE(b.is_empty());
    for (const auto& p : verts) {
        EXPECT_LE(b.min.x, p.x);
        EXPECT_LE(b.min.y, p.y);
        EXPECT_GE(b.max.x, p.x);
        EXPECT_GE(b.max.y, p.y);
    }
}

TEST(Rectangle, Determinism) {
    Rectangle R{.cx = 0.3, .cy = -0.4, .w = 2.5, .h = 1.5, .theta = 0.33};
    PolygonizePolicy pol{};
    auto a = R.polygonize(pol);
    auto b = R.polygonize(pol);

    ASSERT_EQ(a.size(), b.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        EXPECT_NEAR(static_cast<double>(a[i].x),
                    static_cast<double>(b[i].x), 0.0);
        EXPECT_NEAR(static_cast<double>(a[i].y),
                    static_cast<double>(b[i].y), 0.0);
    }
}

TEST(RectangleWithBoundaryData, AppendAndAccess) {
    Rectangle R{.cx = 0.0, .cy = 0.0, .w = 2.0, .h = 2.0, .theta = 0.0};
    PolygonizePolicy pol{};
    auto ring = R.polygonize(pol);

    Boundary2DData data;
    data.reserve(8, 2);
    data.append_ring(std::span<const Point2>(ring.data(), ring.size()),
                     LoopKind::Outer, RegionId{0});

    ASSERT_EQ(data.ring_count(), 1);
    ASSERT_EQ(static_cast<std::size_t>(data.vertex_count()), ring.size());

    auto s = data.ring(0);
    ASSERT_EQ(s.size(), ring.size());

    // Out-of-range safe accessor must throw.
    EXPECT_THROW((void)data.ring_at(-1), std::exception);

    // Non-throwing variant must fail and leave out empty.
    std::span<const Point2> out{};
    EXPECT_FALSE(data.try_ring(42, out));
    EXPECT_TRUE(out.empty());

    EXPECT_TRUE(data.invariant_ok());
}
