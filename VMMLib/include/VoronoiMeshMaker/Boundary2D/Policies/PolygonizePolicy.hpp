#pragma once
//==============================================================================
// Name        : PolygonizePolicy.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : Boundary2D
// Description : Minimal polygonization policy tag used by shapes.
// License     : GNU GPL v3
// Version     : 1.0.0
//==============================================================================

/**
 * @file PolygonizePolicy.hpp
 * @brief Minimal policy for polygonization of parametric shapes.
 *
 * ## Visão geral
 * Este tipo existe principalmente para:
 * - Fornecer um identificador estável em compile-time para contratos
 *   (concepts/traits/overloads) no núcleo do Boundary2D.
 * - Permitir evolução futura sem quebrar a API binária/fonte.
 *
 * ## Diretrizes
 * - Mantenha este header **leve** (sem CGAL nem dependências pesadas).
 * - O núcleo pode aceitar este tipo por `const&` em funções como
 *   `polygonize(const PolygonizePolicy&)`.
 *
 * ## Extensões futuras (exemplos)
 * - Tolerância numérica (`epsilon`), controle de amostragem adaptativa,
 *   número mínimo de segmentos para discretização, etc.
 *
 * @note No momento não há campos — é um "tag type". Adicione campos no futuro
 *       somente quando necessários e com valores padrão estáveis.
 */

#include <VoronoiMeshMaker/Core/namespace.h>

VORMAKER_NAMESPACE_OPEN
BOUNDARY2D_NAMESPACE_OPEN

/// @brief Placeholder policy (extensível no futuro).
struct PolygonizePolicy { };

BOUNDARY2D_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
