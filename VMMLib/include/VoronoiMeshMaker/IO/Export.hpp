#pragma once
//==============================================================================
// Name        : Export.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO
// Description : API de alto nível (fachada) para exportar dados da biblioteca.
// License     : GNU GPL v3
// Version     : 1.0.1 (Corrected Includes)
//==============================================================================

#include <filesystem>
#include <string_view>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp> // Dependência base

// ✅ CORREÇÃO: Inclui os componentes de I/O na ordem lógica de dependência.
#include <VoronoiMeshMaker/IO/Options.hpp>
#include <VoronoiMeshMaker/IO/Sinks.hpp>
#include <VoronoiMeshMaker/IO/PathUtils.hpp>
#include <VoronoiMeshMaker/IO/Boundary2D/Topology/PolyLinesView.hpp>     // Depende de Boundary2DData
#include <VoronoiMeshMaker/IO/Boundary2D/Writers/VTK_Legacy_PolyLines.hpp> // Depende de PolyLinesView

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

inline void export_to_vtk(const vmm::b2d::Boundary2DData& data,
                          std::string_view folder,
                          std::string_view filename,
                          const VtkOptions& options)
{
    const auto final_path = join_with_extension(folder, filename, options);
    FileSink sink(final_path.string());

    if (options.dialect == Dialect::Legacy && options.topology == Topology::PolyLines) {
        PolyLinesView view(data);
        VTK_Legacy_PolyLinesWriter{}(view, options, sink);
    }
    // else if (outras combinações) { ... }
}

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE