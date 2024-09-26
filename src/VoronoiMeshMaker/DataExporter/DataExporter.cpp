//==============================================================================
// Name        : DataExporter.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the DataExporter class, responsible for handling 
//               the export of geometric data in various formats such as VTK, HDF5.
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


//==============================================================================
//  C++ includes
//==============================================================================

//#include <iostream>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/DataExporter/DataExporter.h>
//#include <VoronoiMeshMaker/DataExporter/VTKExportStrategy.h>
//#include <VoronoiMeshMaker/DataExporter/ExportStrategyFactory.h>


/**
 * @file DataExporter.cpp
 * @brief Implements the DataExporter class, handling the export of geometric data.
 * 
 * This file contains the implementation of the DataExporter class, which supports
 * exporting data in formats like VTK, HDF5, and others. It uses the Strategy pattern
 * for flexible export behavior.
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

DataExporter::DataExporter(const std::string& format, ExportOptions options)
    : strategy(ExportStrategyFactory::createExporter(format)), exportOptions(options) {}

void DataExporter::setExportOptions(ExportOptions options) {
    exportOptions = options;
}

VORMAKER_NAMESPACE_CLOSE
