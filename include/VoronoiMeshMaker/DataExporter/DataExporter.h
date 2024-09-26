//==============================================================================
// Name        : DataExporter.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Class for handling the export of geometric data in various formats
//               such as VTK, HDF5, and more. This class uses the Strategy pattern 
//               for flexible export management and supports various types of data.
//
// Copyright   : Copyright (C) 2024  Joao Flavio Vasconcellos 
//                                      (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H

#include <memory>
#include <string>
#include <stdexcept>
#include "VoronoiMeshMaker/DataExporter/ExportStrategyFactory.h"
#include "VoronoiMeshMaker/DataExporter/ExportTraits.h"

/**
 * @file DataExporter.h
 * @brief Defines the DataExporter class responsible for exporting geometric data in various formats.
 * 
 * This file is part of the VoronoiMeshMaker library, which is used for the generation of Voronoi meshes
 * and related operations. It allows flexible exportation of geometric data such as sites, shapes, 
 * Delaunay triangles, and Voronoi diagrams to external formats like VTK and HDF5.
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

/**
 * @enum ExportOptions
 * @brief Defines flags for the various exportable components such as sites, shapes, Delaunay, and Voronoi.
 */
enum class ExportOptions {
    SITES = 1 << 0,       ///< Export sites only
    SHAPES = 1 << 1,      ///< Export shapes only
    DELAUNAY = 1 << 2,    ///< Export Delaunay triangulation only
    VORONOI = 1 << 3      ///< Export Voronoi diagrams only
};

/// Bitwise operator to combine export options.
inline ExportOptions operator|(ExportOptions lhs, ExportOptions rhs) {
    return static_cast<ExportOptions>(
        static_cast<std::underlying_type_t<ExportOptions>>(lhs) |
        static_cast<std::underlying_type_t<ExportOptions>>(rhs)
    );
}

/**
 * @class DataExporter
 * @brief Handles the exportation of geometric data to various formats (e.g., VTK, HDF5).
 * 
 * This class utilizes the Strategy pattern to switch between different export formats 
 * and the Traits pattern to handle different types of geometric data (sites, shapes, Delaunay, Voronoi).
 */
class DataExporter {
private:
    std::unique_ptr<ExportStrategy> strategy;  ///< Strategy for exporting data in a specific format
    ExportOptions exportOptions;               ///< Flags for specifying which data to export

public:
    /**
     * @brief Constructs a DataExporter with a specified format and options.
     * 
     * @param format The format to export data (e.g., "VTK", "HDF5").
     * @param options ExportOptions flags specifying what to export (default is sites and shapes).
     */
    DataExporter(const std::string& format, ExportOptions options = ExportOptions::SITES | ExportOptions::SHAPES);

    /**
     * @brief Sets the export options to specify which components to export.
     * 
     * @param options ExportOptions flags for the components to export.
     */
    void setExportOptions(ExportOptions options);

    /**
     * @brief Exports the provided geometric data based on the current options and strategy.
     * 
     * @tparam T Type of the geometric data (e.g., Sites, Shapes, Delaunay, Voronoi).
     * @param data The geometric data to be exported.
     * @param filename The output file name for the exported data.
     */
    template<typename T>
    void exportData(const T& data, const std::string& filename) const {
        ExportTraits<T>::exportData(data, *strategy, filename);  ///< Delegates to the traits for the correct data type
    }
};

VORMAKER_NAMESPACE_CLOSE

#endif // DATAEXPORTER_H
