#pragma once
//==============================================================================
// Name        : VTK_Legacy_PolyLines.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO / Boundary2D / Writers
// Description : Writer VTK Legacy (.vtk) POLYDATA + POLYLINES (ASCII)
// License     : GNU GPL v3
// Version     : 0.1.0
//==============================================================================

/**
 * @file VTK_Legacy_PolyLines.hpp
 * @brief Serializa `vmm::b2d::Boundary2DData` como VTK Legacy (.vtk) POLYDATA
 *        com entidades `POINTS` e `POLYLINES` (cada ring é um polyline fechado).
 *
 * Características:
 * - **ASCII** (debug/inspeção). Binário Legacy pode ser adicionado depois.
 * - `CELL_DATA` com `loop_kind` (0=Outer, 1=Hole) e `region_id` (int).
 * - Z emitido como 0 para cada ponto (compatível com leitores 3D).
 *
 * Contrato:
 * - Não altera o container; lê via spans/CSR.
 * - Lança `VMMException` com `CoreErr` em caso de argumentos inválidos ou
 *   inconsistências internas (ex.: ring com <3 vértices).
 *
 * Uso:
 *  - Este writer é um functor compatível com `Exporter2DLike` (Concepts.hpp):
 *    `VTK_Legacy_PolyLinesWriter{}(b2d, options, sink);`
 */

#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/IO/Options.hpp>
#include <VoronoiMeshMaker/IO/Sinks.hpp>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DTypes.hpp>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>

// Error handling
#include <VoronoiMeshMaker/ErrorHandling/Macros.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

/// @brief Computa o total de índices para a seção POLYLINES (n+1 por ring).
inline std::size_t total_polyline_indices(const ::vmm::b2d::Boundary2DData& b2d) {
    std::size_t total = 0;
    const auto rc = static_cast<std::size_t>(b2d.ring_count());
    for (std::size_t r = 0; r < rc; ++r) {
        const auto begin = static_cast<std::size_t>(b2d.ring_off[r]);
        const auto end   = static_cast<std::size_t>(b2d.ring_off[r + 1]);
        const auto n     = end - begin;
        total += 1 + (n + 1); // "len" + indices + repeat first
    }
    return total;
}

DETAIL_NAMESPACE_CLOSE

/**
 * @brief Functor writer: VTK Legacy (.vtk) POLYDATA com POLYLINES (ASCII).
 *
 * Compatível com `Exporter2DLike<Writer, Sink>`.
 */
struct VTK_Legacy_PolyLinesWriter {
    template <class Sink>
    void operator()(const ::vmm::b2d::Boundary2DData& b2d,
                    const VtkOptions& opt,
                    Sink& sink) const
    {
        using namespace ::vmm::b2d;
        using ::vmm::error::CoreErr;

        // Checagens básicas
        if (!b2d.invariant_ok()) {
            VMM_THROW(CoreErr::AssertFailed, {{"where","b2d.invariant_ok"}});
        }

        // Por enquanto, somente ASCII (binário legacy pode ser adicionado).
        if (opt.encoding != Encoding::ASCII) {
            VMM_THROW(CoreErr::NotImplemented,
                      {{"where","VTK_Legacy_PolyLinesWriter(encoding)"},
                       {"hint","use ASCII or prefer XML+AppendedRaw for binary"}});
        }

        const auto rc = static_cast<std::size_t>(b2d.ring_count());
        const auto pc = static_cast<std::size_t>(b2d.vertex_count());
        const auto total_idx = detail::total_polyline_indices(b2d);

        // Header VTK
        {
            std::ostringstream os;
            os << "# vtk DataFile Version 4.2\n";
            os << "VMM Boundary2D (POLYLINES)\n";
            os << "ASCII\n";
            os << "DATASET POLYDATA\n";
            sink.write(os.str());
        }

        // POINTS (sempre 3 componentes; z=0)
        {
            std::ostringstream os;
            os << "POINTS " << pc << " double\n";
            os << std::setprecision(opt.precision);
            for (const auto& p : b2d.points) {
                os << static_cast<double>(p.x) << " "
                   << static_cast<double>(p.y) << " "
                   << 0.0 << "\n";
            }
            sink.write(os.str());
        }

        // POLYLINES: cada ring vira um polyline fechado
        {
            std::ostringstream os;
            os << "POLYLINES " << rc << " " << total_idx << "\n";

            for (std::size_t r = 0; r < rc; ++r) {
                const auto b = static_cast<std::size_t>(b2d.ring_off[r]);
                const auto e = static_cast<std::size_t>(b2d.ring_off[r + 1]);
                const auto n = e - b;

                if (n < 3) {
                    VMM_THROW(CoreErr::InvalidArgument,
                              {{"ring","<3_vertices"}, {"where","POLYLINES"}});
                }

                os << (n + 1); // length including repeated first index
                for (std::size_t i = 0; i < n; ++i) {
                    os << " " << (b + i);
                }
                os << " " << b << "\n"; // fecha o polyline
            }

            sink.write(os.str());
        }

        // CELL_DATA (loop_kind, region_id) — opcional via opção
        if (opt.cell_data) {
            std::ostringstream os;
            os << "CELL_DATA " << rc << "\n";

            os << "SCALARS loop_kind int 1\n";
            os << "LOOKUP_TABLE default\n";
            for (std::size_t r = 0; r < rc; ++r) {
                const int lk = (b2d.kinds[r] == LoopKind::Hole) ? 1 : 0;
                os << lk << "\n";
            }

            os << "SCALARS region_id int 1\n";
            os << "LOOKUP_TABLE default\n";
            for (std::size_t r = 0; r < rc; ++r) {
                os << static_cast<int>(b2d.regions[r].value) << "\n";
            }

            sink.write(os.str());
        }

        sink.flush();
    }
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
