//==============================================================================
// Name        : ShapeFactory.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Definition of the ShapeFactory class for creating shape objects
//               dynamically based on type registration.
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vieira de Vasconcellos
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

#ifndef __VORONOIMESHMAKER_SHAPEFACTORY_H__
#define __VORONOIMESHMAKER_SHAPEFACTORY_H__

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class ShapeFactory
 * @brief Factory for creating shape objects dynamically based on type registration.
 * 
 * This factory uses a registration mechanism to associate shape creation functions with a shared ClassID,
 * enabling shapes to be created dynamically based on this ID.
 * 
 * The ShapeFactory class provides static methods to register shapes and create instances of registered shapes.
 * It stores shape creation functions in a registry, allowing the user to create shapes by specifying a ClassID.
 * 
 * @ingroup shape
 * @version 1.0
 * @date 2024
 */
class ShapeFactory {
public:
    /**
     * @brief Registers a shape type in the factory using its ClassID.
     * 
     * This function associates a ClassID with a function that creates an instance of the shape
     * when provided with a GeometricDataHolder.
     * 
     * @tparam T The type of shape to be registered. T must derive from Shape.
     */
    template <typename T>
    static void registerShape() {
        FunctionLogger logger;
        static_assert(std::is_base_of<Shape, T>::value, "T must derive from Shape");

        shapeRegistry[T::getClassID()] = [](const GeometricDataHolder& data) -> std::shared_ptr<Shape> {
            return std::make_shared<T>(data);
        };
    }

    /**
     * @brief Creates a shape based on its ClassID.
     * 
     * Finds the associated shape creation function in the registry using the provided ClassID and
     * returns a shared pointer to the newly created shape, initialized with the provided data.
     * 
     * @param classID The shared ID of the shape to create.
     * @param data The geometric data used to initialize the shape.
     * @return std::shared_ptr<Shape> A shared pointer to the created shape.
     * @throws std::runtime_error if the ClassID is not found in the registry.
     */
    static std::shared_ptr<Shape> createShape(ClassID classID, const GeometricDataHolder& data);

private:
    /// Registry mapping ClassIDs to shape creation functions.
    static std::map<ClassID, std::function<std::shared_ptr<Shape>(const GeometricDataHolder&)>> shapeRegistry;
};

//==============================================================================
// Typedefs
//==============================================================================

/**
 * @typedef PtrShapeFactoryShared
 * @brief Defines a shared pointer to a ShapeFactory object.
 * 
 * This typedef is useful for managing shared ownership of ShapeFactory instances.
 */
using PtrShapeFactoryShared = std::shared_ptr<ShapeFactory>;

/**
 * @typedef PtrConstShapeFactoryShared
 * @brief Defines a shared pointer to a const ShapeFactory object.
 * 
 * This typedef is used when shared ownership of a constant ShapeFactory instance is required.
 */
using PtrConstShapeFactoryShared = std::shared_ptr<const ShapeFactory>;

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPEFACTORY_H__
