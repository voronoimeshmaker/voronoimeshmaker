//==============================================================================
// Name        : Exporter.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the template class Exporter for data exporting.
//
//               This class provides a generic interface for exporting different
//               types of data in various formats.
//
//               It utilizes templates and traits to adapt to different data
//               types and export formats.
//
// License     : GNU General Public License v3.0
//
//==============================================================================

/**
 * @file Exporter.h
 * @brief Declares the template class Exporter for data exporting.
 *
 * This class provides a generic interface for exporting different types of data
 * in various formats. It utilizes templates and traits to adapt to different
 * data types and export formats.
 *
 * @ingroup exporter
 *
 * @version 1.0
 * @date 2024
 */

#ifndef __VORONOIMESHMAKER_EXPORTER_H__
#define __VORONOIMESHMAKER_EXPORTER_H__

//==============================================================================
//  C++ includes
//==============================================================================

#include <string>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>
#include <VoronoiMeshMaker/Misc/ExportOptions.h>
#include <VoronoiMeshMaker/Exporter/ExporterTraits.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Exporter
 * @brief Template class for data exporting.
 *
 * The Exporter class provides a generic interface for exporting different types
 * of data in various formats. It uses templates and traits to adapt to
 * different data types and export formats.
 *
 * @tparam DataType The type of data to export.
 * @tparam Format The format to export the data in.
 */
template<typename DataType, typename Format>
class Exporter {

public:

    /**
     * @brief Constructor that takes the data to export and export options.
     *
     * @param data The data to export.
     * @param options The export options.
     */
    Exporter(const DataType& data, ExportOptions options)
        : data_(data), options_(options) {}

    /**
     * @brief Exports the data to a file.
     *
     * @param filename The name of the output file.
     */
    void exportData(const std::string& filename) {
        ExporterTraits<DataType, Format>::exportData(data_, filename, options_);
    }

private:

    const DataType& data_;       ///< The data to export.
    ExportOptions options_;      ///< The export options.

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_EXPORTER_H__
