//==============================================================================
// Name        : ExportStrategyFactory.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Factory class for creating export strategies based on the format 
//               requested by the user (e.g., VTK, HDF5).
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

#ifndef EXPORTSTRATEGYFACTORY_H
#define EXPORTSTRATEGYFACTORY_H

#include <memory>
#include <stdexcept>
#include "VoronoiMeshMaker/DataExporter/VTKExportStrategy.h"
#include "VoronoiMeshMaker/DataExporter/HDF5ExportStrategy.h"

/**
 * @file ExportStrategyFactory.h
 * @brief Factory class for creating export strategies based on the specified format.
 * 
 * This file is part of the VoronoiMeshMaker library and defines the factory class responsible
 * for creating the appropriate export strategy (e.g., VTK, HDF5) based on the requested format.
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

/**
 * @class ExportStrategyFactory
 * @brief Factory for creating different export strategies based on the format.
 * 
 * This factory class creates instances of export strategies (e.g., VTKExportStrategy, HDF5ExportStrategy)
 * based on the format specified by the user.
 */
class ExportStrategyFactory {
public:
    /**
     * @brief Creates an export strategy based on the specified format.
     * 
     * @param format The export format (e.g., "VTK", "HDF5").
     * @return A unique pointer to the created export strategy.
     * @throws std::invalid_argument if the format is unknown.
     */
    static std::unique_ptr<ExportStrategy> createExporter(const std::string& format);
};

VORMAKER_NAMESPACE_CLOSE

#endif // EXPORTSTRATEGYFACTORY_H
