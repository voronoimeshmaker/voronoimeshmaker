#pragma once
//==============================================================================
// Name        : Rectangle.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : Boundary2D / Shapes
// Description : Retângulo eixo-alinhado, centrado na origem (sem rotação/offset)
// License     : GNU GPL v3
// Version     : 0.3.1
//==============================================================================

/**
 * @file Rectangle.hpp
 * @brief Retângulo eixo-alinhado e centrado na origem.
 *
 * Decisão: transformações (rotação/translação) ficam fora das shapes.
 * `polygonize(pol)` retorna std::vector<Point2> (exigência do Shape2DLike).
 * `polygonize_into(span<Point2>)` oferece caminho sem alocação.
 */

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/Boundary2D/Boundary2DConcepts.hpp>
#include <VoronoiMeshMaker/Boundary2D/Policies/PolygonizePolicy.hpp>

VORMAKER_NAMESPACE_OPEN
BOUNDARY2D_NAMESPACE_OPEN

struct Rectangle {
    Real w{1.0};
    Real h{1.0};

    [[nodiscard]] bool has_positive_dims() const noexcept {
        return (w > Real{0}) && (h > Real{0});
    }

    /**
     * @brief Gera os 4 vértices CCW do retângulo na origem.
     * @return std::vector<Point2> (exigido por Shape2DLike)
     */
    [[nodiscard]] std::vector<Point2>
    polygonize(const ::vmm::b2d::PolygonizePolicy& /*pol*/) const
    {
        using ::vmm::error::CoreErr;
        if (!has_positive_dims()) {
            VMM_THROW(CoreErr::InvalidArgument,
                      {{"shape","Rectangle"}, {"what","non_positive_dims"}});
        }
        const Real hx = w * Real{0.5};
        const Real hy = h * Real{0.5};
        std::vector<Point2> out;
        out.reserve(4);
        out.emplace_back(Point2{-hx, -hy});
        out.emplace_back(Point2{ hx, -hy});
        out.emplace_back(Point2{ hx,  hy});
        out.emplace_back(Point2{-hx,  hy});
        return out;
    }

    /**
     * @brief Escreve os 4 vértices no `dst` (precisa de espaço ≥ 4).
     */
    void polygonize_into(std::span<Point2> dst) const
    {
        using ::vmm::error::CoreErr;
        if (dst.size() < 4) {
            VMM_THROW(CoreErr::OutOfRange,
                      {{"where","Rectangle::polygonize_into"},
                       {"need","4"}});
        }
        const Real hx = w * Real{0.5};
        const Real hy = h * Real{0.5};
        dst[0] = Point2{-hx, -hy};
        dst[1] = Point2{ hx, -hy};
        dst[2] = Point2{ hx,  hy};
        dst[3] = Point2{-hx,  hy};
    }
};

// Checagem conceitual agora passa (retorno é std::vector<Point2>)
static_assert(Shape2DLike<Rectangle, ::vmm::b2d::PolygonizePolicy>);

BOUNDARY2D_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
