//==============================================================================
// Name        : Shape.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the Shape class and its interface for all shapes.
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
 * @file Shape.h
 * @brief Defines the Shape class, which serves as the abstract base class for all shapes.
 *
 * This class provides the interface for methods related to shapes, including rotation,
 * translation, area, volume, and perimeter calculations.
 *
 * @ingroup shape
 * @version 1.0
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * @copyright GNU General Public License v3.0
 */

#ifndef __VORONOIMESHMAKER_SHAPE_H__
#define __VORONOIMESHMAKER_SHAPE_H__

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Misc/ID.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Shape
 * @brief Abstract base class for all shapes.
 *
 * The Shape class provides the common interface and properties that all shapes must implement.
 * It includes methods for geometric transformations and calculations.
 */
class Shape : public ID {

//==============================================================================
// Constructors / Destructor
//==============================================================================

public:
    /**
     * @brief Default constructor.
     */
    Shape() noexcept = default;

    /**
     * @brief Copy constructor.
     *
     * Initializes a new instance by copying the data from another Shape instance.
     * 
     * @param other The other Shape instance to copy from.
     */
    Shape(const Shape& other) noexcept = default;

    /**
     * @brief Move constructor.
     *
     * Initializes a new instance by moving the data from another Shape instance.
     *
     * @param other The other Shape instance to move from.
     */
    Shape(Shape&& other) noexcept = default;

    /**
     * @brief Destructor.
     *
     * Destroys the Shape instance.
     */
    ~Shape() noexcept = default;

    /**
     * @brief Copy assignment operator.
     *
     * Assigns the data from another Shape instance to this instance.
     *
     * @param other The other Shape instance to copy from.
     * @return Reference to the updated Shape.
     */
    Shape& operator=(const Shape& other) noexcept = default;

    /**
     * @brief Move assignment operator.
     *
     * Moves the data from another Shape instance to this instance.
     *
     * @param other The other Shape instance to move from.
     * @return Reference to the updated Shape.
     */
    Shape& operator=(Shape&& other) noexcept = default;

    /**
     * @brief Constructs a Shape with the given geometric data.
     *
     * @param _dataHolder A reference to a GeometricDataHolder containing geometric data.
     */
    Shape(const GeometricDataHolder& _dataHolder);

//==============================================================================
// Public Methods
//==============================================================================

public:
    /**
     * @brief Returns the name of the class.
     *
     * This method must be overridden by all derived classes to provide the name
     * of the specific shape class.
     *
     * @return A string view representing the name of the class.
     */
    virtual std::string_view className() const noexcept override = 0;

    /**
     * @brief Returns the unique ID of the class.
     *
     * This method must be overridden by all derived classes to provide a unique
     * identifier for each shape type.
     *
     * @return A ClassID representing the unique identifier of the class.
     */
    virtual ClassID classID() const noexcept override = 0;

    /**
     * @brief Rotates the shape based on the provided geometric data.
     *
     * @param data A reference to GeometricDataHolder containing rotation parameters.
     * @return True if the rotation was successful, false otherwise.
     */
    virtual bool rotate(const GeometricDataHolder& data) = 0;

    /**
     * @brief Translates the shape based on the provided geometric data.
     *
     * @param data A reference to GeometricDataHolder containing translation parameters.
     * @return True if the translation was successful, false otherwise.
     */
    virtual bool translate(const GeometricDataHolder& data) = 0;

    /**
     * @brief Calculates the area of the shape.
     *
     * This method must be implemented by all derived classes that represent shapes with an area.
     *
     * @return The area of the shape.
     */
    virtual Real area() const = 0;

    /**
     * @brief Calculates the volume of the shape.
     *
     * This method must be implemented by all derived classes that represent three-dimensional shapes.
     *
     * @return The volume of the shape.
     */
    virtual Real volume() const = 0;

    /**
     * @brief Calculates the perimeter of the shape.
     *
     * This method must be implemented by all derived classes that represent shapes with a perimeter.
     *
     * @return The perimeter of the shape.
     */
    virtual Real perimeter() const = 0;

//==============================================================================
// Private Members
//==============================================================================

private:
    /**
     * @brief Pointer to GeometricDataHolder.
     *
     * This pointer holds the geometric data associated with the shape.
     */
    PtrGeometricDataHolder ptrDataHolder_;
};

//==============================================================================
// Type Definitions
//==============================================================================

/**
 * @typedef PtrShape
 * @brief Defines a shared pointer to a Shape.
 *
 * This type is useful when multiple entities need to share ownership of a Shape instance.
 */
using PtrShape = std::shared_ptr<Shape>;

/**
 * @typedef PtrConstShape
 * @brief Defines a shared pointer to a const Shape.
 *
 * This type is useful when multiple entities need to share ownership of a const Shape instance,
 * ensuring that none of them can modify the instance.
 */
using PtrConstShape = std::shared_ptr<Shape const>;

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPE_H__
