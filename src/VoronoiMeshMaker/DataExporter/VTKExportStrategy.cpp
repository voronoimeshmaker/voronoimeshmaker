//==============================================================================
// Name        : VTKExportStrategy.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the VTK export strategy for geometric data.
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

#include "VoronoiMeshMaker/DataExporter/VTKExportStrategy.h"
#include <iostream>
#include <stdexcept>

/**
 * @file VTKExportStrategy.cpp
 * @brief Implements the VTK export strategy for geometric data.
 * 
 * This file contains the implementation for exporting geometric data in VTK format.
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

void VTKExportStrategy::exportData(const GeometricDataHolder& data, const std::string& filename) const {
    std::cout << "Exporting data in VTK format to " << filename << std::endl;
    // Placeholder: Actual VTK export logic should go here
    throw std::runtime_error("VTK export functionality is not implemented yet.");
}

VORMAKER_NAMESPACE_CLOSE
