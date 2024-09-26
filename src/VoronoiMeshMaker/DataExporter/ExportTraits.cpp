//==============================================================================
// Name        : ExportTraits.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implements specialized export behavior for various data types 
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
 * @file ExportTraits.cpp
 * @brief Implements the specialized export behavior for different geometric data types.
 * 
 * This file contains the implementation of the ExportTraits specializations for
 * exporting different types of geometric data, including Sites, Shapes, Delaunay
 * triangles, and Voronoi diagrams.
 * 
 * @date 2024
 */

//==============================================================================
//  C++ includes
//==============================================================================

#include <iostream>
#include <stdexcept>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/DataExporter/ExportTraits.h>

VORMAKER_NAMESPACE_OPEN

// Other possible specializations can go here, but the Shape specialization
// is already defined in the header file.

// Specialization for Sites (example commented out)
// void ExportTraits<Sites>::exportData(const Sites& sites, ExportStrategy& strategy, const std::string& filename) {
//     std::cout << "Exporting sites to " << filename << std::endl;
//     // Logic for exporting Sites goes here
//     throw std::runtime_error("Exporting Sites is not implemented yet.");
// }

//// Specialization for Delaunay triangles
// void ExportTraits<Delaunay>::exportData(const Delaunay& delaunay, ExportStrategy& strategy, const std::string& filename) {
//     std::cout << "Exporting Delaunay triangles to " << filename << std::endl;
//     // Logic for exporting Delaunay triangles goes here
//     throw std::runtime_error("Exporting Delaunay triangles is not implemented yet.");
// }

//// Specialization for Voronoi diagrams
// void ExportTraits<Voronoi>::exportData(const Voronoi& voronoi, ExportStrategy& strategy, const std::string& filename) {
//     std::cout << "Exporting Voronoi diagrams to " << filename << std::endl;
//     // Logic for exporting Voronoi diagrams goes here
//     throw std::runtime_error("Exporting Voronoi diagrams is not implemented yet.");
// }

VORMAKER_NAMESPACE_CLOSE
