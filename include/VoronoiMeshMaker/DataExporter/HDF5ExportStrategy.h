//==============================================================================
// Name        : HDF5ExportStrategy.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the HDF5 export strategy for geometric data.
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

#ifndef HDF5EXPORTSTRATEGY_H
#define HDF5EXPORTSTRATEGY_H

#include "VoronoiMeshMaker/DataExporter/ExportStrategy.h"

/**
 * @file HDF5ExportStrategy.h
 * @brief Defines the HDF5 export strategy for geometric data.
 * 
 * This file is part of the VoronoiMeshMaker library, which allows exporting data such as shapes, 
 * sites, Delaunay triangles, and Voronoi diagrams to the HDF5 format.
 * 
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

/**
 * @class HDF5ExportStrategy
 * @brief Concrete strategy for exporting geometric data to HDF5 format.
 */
class HDF5ExportStrategy : public ExportStrategy {
public:
    /**
     * @brief Exports the provided geometric data to HDF5 format.
     * 
     * @param data The geometric data to be exported.
     * @param filename The output file name.
     */
    void exportData(const GeometricDataHolder& data, const std::string& filename) const override;
};

VORMAKER_NAMESPACE_CLOSE

#endif // HDF5EXPORTSTRATEGY_H
