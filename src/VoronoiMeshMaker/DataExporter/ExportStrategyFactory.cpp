//==============================================================================
// Name        : ExportStrategyFactory.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the factory for creating export strategies 
//               based on the requested format (e.g., VTK, HDF5).
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

#include "VoronoiMeshMaker/DataExporter/ExportStrategyFactory.h"
#include "VoronoiMeshMaker/DataExporter/VTKExportStrategy.h"
#include "VoronoiMeshMaker/DataExporter/HDF5ExportStrategy.h"
#include <stdexcept>

/**
 * @file ExportStrategyFactory.cpp
 * @brief Implements the factory class for creating export strategies.
 * 
 * This file contains the implementation for creating export strategies based
 * on the format specified (e.g., VTK, HDF5).
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

std::unique_ptr<ExportStrategy> ExportStrategyFactory::createExporter(const std::string& format) {
    if (format == "VTK") {
        return std::make_unique<VTKExportStrategy>();
    } else if (format == "HDF5") {
        return std::make_unique<HDF5ExportStrategy>();
    }
    throw std::invalid_argument("Unknown export format: " + format);
}

VORMAKER_NAMESPACE_CLOSE
