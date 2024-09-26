//==============================================================================
// Name        : ExportStrategy.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the interface for exporting geometric data in various formats.
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
 * @file ExportStrategy.h
 * @brief Defines the interface for exporting geometric data in various formats.
 * 
 * This file is part of the VoronoiMeshMaker library, which is used for exporting data such as shapes, 
 * sites, Delaunay triangles, and Voronoi diagrams to different formats.
 * 
 * @date 2024
 */


#ifndef __VORONOMESHVMAKER_EXPORTSTRATEGY_H__
#define __VORONOMESHVMAKER_EXPORTSTRATEGY_H__

#include <string>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/ID.h>                   // class ID


VORMAKER_NAMESPACE_OPEN

class GeometricDataHolder;  ///< Forward declaration of the GeometricDataHolder class

/**
 * @class ExportStrategy
 * @brief Interface for exporting geometric data to different formats.
 * 
 * Each concrete export strategy (e.g., VTK, HDF5) implements this interface.
 */
class ExportStrategy : public ID {
    
//==============================================================================
// Class ID
//==============================================================================    
    
public:       
    /**
     * @brief Get the class name.
     *
     * @return A string view representing the name of this class.
     */
    virtual std::string_view className() const noexcept override {
        return "ExportStrategy";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
    
    // Método estático que retorna o ClassID do ExportStrategy
    static ClassID getClassID() {
        return ClassID::ExportStrategy; 
    }  
    
public:
    /// Virtual destructor to ensure proper cleanup of derived classes.
    virtual ~ExportStrategy() = default;

    /**
     * @brief Exports the provided geometric data to the specified file.
     * 
     * @param data The geometric data to be exported.
     * @param filename The output file name.
     */
    virtual void exportData(const GeometricDataHolder& data, const std::string& filename) const = 0;
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_EXPORTSTRATEGY_H__
