//==============================================================================
// Name        : ExportTraits.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines traits for exporting different types of geometric data 
//               such as Sites, Shapes, Delaunay triangles, and Voronoi diagrams.
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

/**
 * @file ExportTraits.h
 * @brief Defines traits for exporting different types of geometric data.
 * 
 * This file defines specializations of the ExportTraits template for different
 * geometric data types (e.g., Sites, Shapes, Delaunay, Voronoi).
 * 
 * @date 2024
 */

#ifndef EXPORTTRAITS_H
#define EXPORTTRAITS_H

//==============================================================================
//  C++ includes
//==============================================================================

#include <iostream>
#include <stdexcept>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>
#include <VoronoiMeshMaker/DataExporter/ExportStrategy.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class ExportTraits
 * @brief Provides specialized behavior for exporting different types of geometric data.
 * 
 * The ExportTraits template is specialized for each data type that can be exported,
 * such as Sites, Shapes, Delaunay, and Voronoi. Each specialization defines how
 * that data type should be exported using the provided ExportStrategy.
 * 
 * @tparam T The data type to be exported (e.g., Sites, Shapes, Delaunay, Voronoi).
 */
template<typename T>
struct ExportTraits {
    /**
     * @brief Exports the specified data using the given strategy.
     * 
     * This default implementation throws an error, and should be specialized for
     * specific data types.
     * 
     * @param data The geometric data to export.
     * @param strategy The strategy to use for exporting.
     * @param filename The name of the file to export to.
     */
    static void exportData(const T& data, ExportStrategy& strategy, const std::string& filename) {
        throw std::runtime_error("Exporting this data type is not yet implemented.");
    }
};

// Specialization for Shapes
template<>
struct ExportTraits<Shape> {
    static void exportData(const Shape& shape, ExportStrategy& strategy, const std::string& filename) {
        std::cout << "Exporting shape to " << filename << std::endl;
        // Implementation for exporting Shapes
        throw std::runtime_error("Exporting Shapes is not implemented yet.");
    }
};

VORMAKER_NAMESPACE_CLOSE

#endif // EXPORTTRAITS_H
