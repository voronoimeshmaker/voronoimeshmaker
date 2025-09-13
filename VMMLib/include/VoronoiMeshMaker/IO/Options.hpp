#pragma once
//==============================================================================
// Name        : Options.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO
// Description : Enums e opções para exportação (dialeto, topologia, encoding).
// License     : GNU GPL v3
// Version     : 0.2.0
//==============================================================================

/**
 * @file Options.hpp
 * @brief Define enums e opções comuns usadas pelos exportadores (VTK/VisIt).
 *
 * Diretrizes:
 * - Dialeto:
 *     - Legacy : formato VTK “clássico” (.vtk).
 *     - XML    : VTK XML PolyData (.vtp).
 * - Topologia:
 *     - PolyLines : contornos (rings) como linhas fechadas.
 *     - Polys     : superfícies (futuro; requer triangulação).
 * - Encoding:
 *     - ASCII       : texto legível (depuração).
 *     - Binary      : binário do Legacy (.vtk).
 *     - AppendedRaw : “binário” do XML (.vtp) sem Base64 (recomendado).
 *
 * Flags de execução:
 * - deterministic : força ordem/saída estável (recomendado para testes/golden).
 * - parallel_prep : permite paralelizar a fase de preparação (contagens,
 *                   prefix sums, buffers) — a escrita final continua sequencial.
 */

#include <VoronoiMeshMaker/Core/namespace.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

/// Dialeto/formato alvo do ecossistema VTK.
enum class Dialect  : unsigned char {
    Legacy,  ///< VTK legacy (.vtk)
    XML      ///< VTK XML PolyData (.vtp)
};

/// Topologia a ser escrita.
enum class Topology : unsigned char {
    PolyLines, ///< Contornos (rings) como linhas fechadas
    Polys      ///< Superfícies preenchidas (triangulado) — futuro
};

/// Codificação de saída.
enum class Encoding : unsigned char {
    ASCII,       ///< Texto (depuração)
    Binary,      ///< Binário do Legacy (.vtk)
    AppendedRaw  ///< Binário “anexado” do XML (.vtp), sem Base64
};

/// Opções de exportação VTK.
struct VtkOptions {
    Dialect  dialect      { Dialect::Legacy };      ///< Dialeto padrão
    Topology topology     { Topology::PolyLines };  ///< Topologia padrão
    Encoding encoding     { Encoding::ASCII };      ///< Encoding padrão

    int      precision    { 17 };    ///< Precisão p/ ASCII (stream)
    bool     write_z      { false }; ///< Se true, emite z=0 (3D)
    bool     cell_data    { true };  ///< Emite CELL_DATA (tags básicas)

    bool     deterministic{ true };  ///< Saída estável/reprodutível
    bool     parallel_prep{ false }; ///< Paraleliza preparação (não a escrita)
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
