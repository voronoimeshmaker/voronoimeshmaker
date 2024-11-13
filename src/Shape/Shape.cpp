//==============================================================================
// Name        : Shape.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the Shape class methods.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : This program is free software: you can redistribute it and/or modify
//               it under the terms of the GNU General Public License as published by
//               the Free Software Foundation, either version 3 of the License, or
//               (at your option) any later version.
//
//               This program is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty of
//               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//               GNU General Public License for more details.
//
//               You should have received a copy of the GNU General Public License
//               along with this program. If not, see <https://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file Shape.cpp
 * @brief Implementation of the Shape class methods.
 *
 * This file contains the implementation of the methods for the Shape class,
 * which is part of the Shape module of the VoronoiMeshMaker library.
 *
 * @author
 * João Flávio Vieira de Vasconcellos
 * @version 1.0
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>

VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Constructor Implementation
//==============================================================================

/**
 * @brief Constructs a Shape with the given geometric data.
 *
 * This constructor initializes the Shape object using the provided GeometricDataHolder.
 * It creates a shared pointer to store the geometric data for the shape.
 *
 * @param _dataHolder A constant reference to a GeometricDataHolder containing geometric data.
 */
Shape::Shape(const GeometricDataHolder& _dataHolder) {
    FunctionLogger logger;
    ptrDataHolder_ = std::make_shared<GeometricDataHolder>(_dataHolder);
}

VORMAKER_NAMESPACE_CLOSE
