//==============================================================================
// Name        : ExporterTraits.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the ExporterTraits struct for data exporting.
//
//               This file contains specializations of the ExporterTraits struct
//               for different combinations of data types and export formats.
//
// License     : GNU General Public License v3.0
//
//==============================================================================

/**
 * @file ExporterTraits.h
 * @brief Declares the ExporterTraits struct for data exporting.
 *
 * This file contains specializations of the ExporterTraits struct for different
 * combinations of data types and export formats. It enables the Exporter class
 * to adapt its behavior based on the data type and export format.
 *
 * @ingroup exporter
 *
 * @version 1.0
 * @date 2024
 */

#ifndef __VORONOIMESHMAKER_EXPORTERTRAITS_H__
#define __VORONOIMESHMAKER_EXPORTERTRAITS_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace_config.h>
#include <VoronoiMeshMaker/Shape/Shape2D.h>
#include <VoronoiMeshMaker/Shape/Shape3D.h>
#include <VoronoiMeshMaker/Nodes/Nodes2D/Nodes2D.h>
#include <VoronoiMeshMaker/Nodes/Nodes3D/Nodes3D.h>
#include <VoronoiMeshMaker/Voronoi/Voronoi2D/Voronoi2D.h>
#include <VoronoiMeshMaker/Voronoi/Voronoi3D/Voronoi3D.h>
#include <VoronoiMeshMaker/Misc/ExportOptions.h>

// Formatos de exportação
#include <VoronoiMeshMaker/Formats/VTKFormat.h>
#include <VoronoiMeshMaker/Formats/HDF5Format.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Traits class template for exporting data.
 *
 * The ExporterTraits struct provides a static method exportData that is specialized
 * for different data types and export formats.
 *
 * @tparam DataType The type of data to export.
 * @tparam Format The format to export the data in.
 */
template<typename DataType, typename Format>
struct ExporterTraits;

//==============================================================================
//  Specializations for Shape2D
//==============================================================================

/**
 * @brief Specialization of ExporterTraits for Shape2D and VTKFormat.
 */
template<>
struct ExporterTraits<Shape2D, VTKFormat> {
    static void exportData(const Shape2D& data, const std::string& filename, ExportOptions options) {
        // Call the specific exporter for Shape2D and VTK
        ExporterVTK2D::exportShape(data, filename, options);
    }
};

/**
 * @brief Specialization of ExporterTraits for Shape2D and HDF5Format.
 */
template<>
struct ExporterTraits<Shape2D, HDF5Format> {
    static void exportData(const Shape2D& data, const std::string& filename, ExportOptions options) {
        // Call the specific exporter for Shape2D and HDF5
        ExporterHDF52D::exportShape(data, filename, options);
    }
};

//==============================================================================
//  Specializations for Shape3D
//==============================================================================

/**
 * @brief Specialization of ExporterTraits for Shape3D and VTKFormat.
 */
template<>
struct ExporterTraits<Shape3D, VTKFormat> {
    static void exportData(const Shape3D& data, const std::string& filename, ExportOptions options) {
        // Call the specific exporter for Shape3D and VTK
        ExporterVTK3D::exportShape(data, filename, options);
    }
};

/**
 * @brief Specialization of ExporterTraits for Shape3D and HDF5Format.
 */
template<>
struct ExporterTraits<Shape3D, HDF5Format> {
    static void exportData(const Shape3D& data, const std::string& filename, ExportOptions options) {
        // Call the specific exporter for Shape3D and HDF5
        ExporterHDF53D::exportShape(data, filename, options);
    }
};

//==============================================================================
//  Additional specializations can be added here for Nodes2D, Nodes3D, Voronoi2D, Voronoi3D
//==============================================================================

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_EXPORTERTRAITS_H__
