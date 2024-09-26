//==============================================================================
// Name        : ShapeFactory.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the ShapeFactory methods.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//==============================================================================

/**
 * @file ShapeFactory.cpp
 * @brief Implements the ShapeFactory methods for creating shape objects.
 * This file contains the registration and creation logic for shapes.
 *
 * @ingroup shape
 * 
 * @version 1.0
 * @date 2024
 */

#include <VoronoiMeshMaker/Shape/ShapeFactory.h>
#include <stdexcept> // Para exceções

VORMAKER_NAMESPACE_OPEN

// Define the static shape registry that maps ClassID to creation functions
std::map<ClassID, std::function<std::unique_ptr<Shape>(const GeometricDataHolder&)>> ShapeFactory::shapeRegistry;

// Definição da função createShape
std::unique_ptr<Shape> ShapeFactory::createShape(ClassID classID, const GeometricDataHolder& data) {
    auto it = shapeRegistry.find(classID);
    if (it != shapeRegistry.end()) {
        return it->second(data);
    } else {
        throw std::runtime_error("Shape with ClassID not found in registry.");
    }
}

VORMAKER_NAMESPACE_CLOSE
