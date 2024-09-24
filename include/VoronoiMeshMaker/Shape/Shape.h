//==============================================================================
// Name        : Shape.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Base class for all shapes (2D and 3D).
//               This class provides common functionality for all geometric shapes.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file Shape.h
 * @brief Defines the base class for all shapes (2D and 3D).
 * 
 * This is the base class for geometric shapes. It provides pure virtual methods 
 * for calculating geometric properties like area, perimeter, and volume. 
 * It also includes methods for rotating and translating shapes using geometric data.
 * 
 * @ingroup geometry
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef SHAPE_H
#define SHAPE_H

//==============================================================================
//  Includes
//==============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <VoronoiMeshMaker/Misc/ID.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Shape
 * @brief Base class for all geometric shapes (2D and 3D).
 * 
 * This abstract class provides the interface for all shapes. 
 * Derived classes must implement methods for calculating area, perimeter, and volume.
 * It also includes pure virtual methods for rotating and translating shapes.
 */
class Shape : public ID {
public:
    /**
     * @brief Constructor for Shape.
     * @param data A GeometricDataHolder containing the geometric data.
     */
    explicit Shape(const GeometricDataHolder& data) : dataHolder_(data) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~Shape() = default;

    /**
     * @brief Get the class name.
     * @return The name of the class as a string view.
     */
    virtual std::string_view className() const noexcept override = 0;

    /**
     * @brief Get the class ID.
     * @return The unique ID for the class.
     */
    virtual ClassID classID() const noexcept override = 0;

    /**
     * @brief Pure virtual method to calculate the area of the shape.
     * 
     * This method must be overridden by 2D shapes.
     * 
     * @return The area of the shape.
     */
    virtual Real area() const = 0;

    /**
     * @brief Pure virtual method to calculate the perimeter of the shape.
     * 
     * This method must be overridden by 2D shapes.
     * 
     * @return The perimeter of the shape.
     */
    virtual Real perimeter() const = 0;

    /**
     * @brief Pure virtual method to calculate the volume of the shape.
     * 
     * This method must be overridden by 3D shapes.
     * 
     * @return The volume of the shape.
     */
    virtual Real volume() const = 0;

    /**
     * @brief Pure virtual method to rotate the shape.
     * @param data A GeometricDataHolder containing the rotation parameters.
     * 
     * @return `true` if the rotation was successful, `false` if there was an error.
     */
    virtual bool rotate(const GeometricDataHolder& data) = 0;

    /**
     * @brief Pure virtual method to translate the shape.
     * @param data A GeometricDataHolder containing the translation parameters.
     * 
     * @return `true` if the translation was successful, `false` if there was an error.
     */
    virtual bool translate(const GeometricDataHolder& data) = 0;

protected:
    /**
     * @brief Geometric data holder containing parameters for the shape.
     */
    GeometricDataHolder dataHolder_;
};

VORMAKER_NAMESPACE_CLOSE

#endif // SHAPE_H
