// -----------------------------------------------------------------------------
//  include GTest
// -----------------------------------------------------------------------------
#include <gtest/gtest.h>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DTypes.hpp>
// Incluído para ter acesso à função vmm::error::error_code()
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>

// Usando os namespaces para simplificar o código do teste
using namespace vmm::b2d;
using vmm::error::CoreErr;
using vmm::Real;
using vmm::error::VMMException;

namespace {

// --- Função Helper para calcular a área de um polígono ---
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

// =============================================================================
//      TESTES PARA OS CONSTRUTORES E VALIDAÇÃO
// =============================================================================

TEST(RectangleConstructors, ThrowsOnNonPositiveDimensions) {
    EXPECT_THROW({
        try {
            Rectangle R(0.0, 1.0);
        } catch (const VMMException& e) {
            // ✅ CORREÇÃO FINAL: Comparamos o código da exceção com o código
            //    completo gerado pela função helper da sua biblioteca.
            EXPECT_EQ(e.code(), ::vmm::error::error_code(CoreErr::InvalidArgument));
            throw;
        }
    }, VMMException);
    EXPECT_THROW(Rectangle(-1.0, 1.0), VMMException);
    EXPECT_THROW(Rectangle(1.0, 0.0), VMMException);

    Point2 ll{10.0, 10.0};
    EXPECT_THROW(Rectangle(ll, 0.0, 1.0), VMMException);
    EXPECT_THROW(Rectangle(ll, 1.0, -1.0), VMMException);

    EXPECT_THROW(Rectangle(0.0, 1.0, Rectangle::Centered), VMMException);
    EXPECT_THROW(Rectangle(1.0, -1.0, Rectangle::Centered), VMMException);
}

TEST(RectangleConstructors, DefaultAnchoredAtOrigin) {
    Rectangle R(4.0, 3.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.w), 4.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.h), 3.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.x), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.y), 0.0);

    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);
    ASSERT_EQ(verts.size(), 4u);
    EXPECT_NEAR(polygon_area(verts), 12.0, 1e-12);
}

TEST(RectangleConstructors, UserDefinedAnchor) {
    Point2 ll{-5.0, 10.0};
    Rectangle R(ll, 2.0, 5.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.x), -5.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.y), 10.0);

    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);
    ASSERT_EQ(verts.size(), 4u);
    EXPECT_NEAR(polygon_area(verts), 10.0, 1e-12);
}

TEST(RectangleConstructors, CenteredAtOrigin) {
    Rectangle R(10.0, 4.0, Rectangle::Centered);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.x), -5.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(R.ll.y), -2.0);

    PolygonizePolicy pol{};
    auto verts = R.polygonize(pol);
    ASSERT_EQ(verts.size(), 4u);
    EXPECT_NEAR(polygon_area(verts), 40.0, 1e-12);
}

// =============================================================================
//      TESTES DE MÉTODOS
// =============================================================================

TEST(RectangleMethods, PolygonizeInto) {
    Rectangle R(10.0, 4.0, Rectangle::Centered);
    std::array<Point2, 5> buffer{};
    std::span<Point2> dst(buffer);

    EXPECT_NO_THROW(R.polygonize_into(dst));

    std::array<Point2, 3> small_buffer{};
    std::span<Point2> small_dst(small_buffer);
    EXPECT_THROW({
        try {
            R.polygonize_into(small_dst);
        } catch (const VMMException& e) {
            // ✅ CORREÇÃO FINAL: Usando a mesma abordagem aqui.
            EXPECT_EQ(e.code(), ::vmm::error::error_code(CoreErr::OutOfRange));
            throw;
        }
    }, VMMException);
}

} // namespace