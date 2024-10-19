//==============================================================================
// Name        : ShapeFactory.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the ShapeFactory methods.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : This program is free software: you can redistribute it and/or modify
//               it under the terms of the GNU General Public License as published by
//               the Free Software Foundation, either version 3 of the License.
//
//               This program is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty of
//               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//               GNU General Public License for more details.
//
//               You should have received a copy of the GNU General Public License
//               along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file ShapeFactory.cpp
 * @brief Implements the ShapeFactory methods for creating shape objects.
 * This file contains the registration and creation logic for shapes.
 *
 * @ingroup shape
 * @version 1.0
 * @date 2024
 * @author Joao Flavio Vieira de Vasconcellos
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>


VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Static Member Initialization
//==============================================================================

/// Definition of the static shape registry
std::map<ClassID, std::function<std::shared_ptr<Shape>(const GeometricDataHolder&)>> ShapeFactory::shapeRegistry;

//==============================================================================
// Public Methods
//==============================================================================

/**
 * @brief Creates a shape based on its ClassID.
 *
 * This method looks up the provided ClassID in the shape registry to find the
 * corresponding creation function. If found, it uses this function to create
 * and return an instance of the shape.
 *
 * @param classID The shared ID of the shape to create.
 * @param data The geometric data used to initialize the shape.
 * @return std::shared_ptr<Shape> A shared pointer to the created shape.
 * @throws std::runtime_error if the ClassID is not found in the registry.
 */
std::shared_ptr<Shape> ShapeFactory::createShape(ClassID classID, const GeometricDataHolder& data) {
    auto it = shapeRegistry.find(classID);
    
    try {    
    if (it != shapeRegistry.end()) {
        return it->second(data);
    } else {
//        throw std::runtime_error("Shape with ClassID not found in registry.");
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex::SHAPENOTFOUNDREGISTRY,
                true
            );        
    }
    } catch (const VMMException& exception) {
        
        std::cerr << "\n";
        std::cerr << exception;
        std::exit(EXIT_FAILURE);
    }    
    
}

VORMAKER_NAMESPACE_CLOSE
