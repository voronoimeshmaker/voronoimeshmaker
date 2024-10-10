//==============================================================================
// Name        : ExporterVTK2D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the ExporterVTK2D class for exporting 2D data to VTK format.
//
//               This class provides methods to export 2D shapes, nodes, and Voronoi diagrams
//               to VTK format.
//
// License     : GNU General Public License v3.0
//
//==============================================================================

/**
 * @file ExporterVTK2D.h
 * @brief Declares the ExporterVTK2D class for exporting 2D data to VTK format.
 *
 * This class provides methods to export 2D shapes, nodes, and Voronoi diagrams
 * to VTK format.
 *
 * @ingroup exporter
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License v3.0.
 */

#ifndef __VORONOIMESHMAKER_EXPORTERVTK2D_H__
#define __VORONOIMESHMAKER_EXPORTERVTK2D_H__

//==============================================================================
//  C++ includes
//==============================================================================

#include <string>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace_config.h>
#include <VoronoiMeshMaker/Misc/ExportOptions.h>
#include <VoronoiMeshMaker/Shape/Shape2D.h>
#include <VoronoiMeshMaker/Nodes/Nodes2D/Nodes2D.h>
#include <VoronoiMeshMaker/Voronoi/Voronoi2D/Voronoi2D.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class ExporterVTK2D
 * @brief Provides methods to export 2D data to VTK format.
 *
 * The ExporterVTK2D class contains static methods to export various 2D data types,
 * such as Shape2D, Nodes2D, and Voronoi2D, to the VTK file format.
 */
class ExporterVTK2D {

public:

    /**
     * @brief Exports a Shape2D object to VTK format.
     *
     * @param shape The Shape2D object to export.
     * @param filename The name of the output file.
     * @param options The export options.
     */
    static void exportShape(const Shape2D& shape, const std::string& filename, ExportOptions options);

    /**
     * @brief Exports a Nodes2D object to VTK format.
     *
     * @param nodes The Nodes2D object to export.
     * @param filename The name of the output file.
     * @param options The export options.
     */
    static void exportNodes(const Nodes2D& nodes, const std::string& filename, ExportOptions options);

    /**
     * @brief Exports a Voronoi2D object to VTK format.
     *
     * @param voronoi The Voronoi2D object to export.
     * @param filename The name of the output file.
     * @param options The export options.
     */
    static void exportVoronoi(const Voronoi2D& voronoi, const std::string& filename, ExportOptions options);

    // Additional methods for exporting other data types can be added here.

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_EXPORTERVTK2D_H__
