//==============================================================================
// Name        : Shape2D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Base class for 2D shapes, derived from Shape.
//               Provides common functionality for handling 2D geometric shapes.
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
 * @file Shape2D.h
 * @brief Defines the base class for all 2D shapes.
 * 
 * This class provides the implementation of methods to calculate
 * area and perimeter for 2D shapes using CGAL.
 * 
 * @ingroup geometry
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef SHAPE2D_H
#define SHAPE2D_H

#include <CGAL/Polygon_2.h>
#include <VoronoiMeshMaker/Shape/Shape.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Shape2D
 * @brief Base class for all 2D shapes.
 * 
 * This class provides implementations for calculating the area and perimeter
 * of 2D shapes, and for verifying whether a given set of points forms a valid polygon.
 */
class Shape2D : public Shape {
public:
    /**
     * @brief Constructor for Shape2D.
     * @param data A GeometricDataHolder containing the geometric data.
     */
    explicit Shape2D(const GeometricDataHolder& data);

    /**
     * @brief Get the class name.
     * @return The name of the class as a string view.
     */
    std::string_view className() const noexcept override;

    /**
     * @brief Get the class ID.
     * @return The unique ID for the class.
     */
    ClassID classID() const noexcept override;

    /**
     * @brief Calculate the area of the 2D shape.
     * 
     * Uses the CGAL `Polygon_2` object to compute the area.
     * 
     * @return The area of the 2D shape.
     */
    Real area() const override;

    /**
     * @brief Calculate the perimeter of the 2D shape.
     * 
     * Uses the CGAL `Polygon_2` object to compute the perimeter.
     * 
     * @return The perimeter of the 2D shape.
     */
    Real perimeter() const override;

    /**
     * @brief Check if the polygon is valid and ensure correct orientation.
     * 
     * This method checks whether the polygon is simple and has the correct orientation
     * (clockwise). If the orientation is counterclockwise, it inverts the points.
     * 
     * @return `true` if the polygon is valid and oriented correctly, `false` otherwise.
     */
    bool validatePolygon();

    /**
     * @brief Rotate the 2D shape.
     * @param data A GeometricDataHolder containing the rotation parameters.
     * @return `true` if the rotation was successful, `false` if there was an error.
     */
    bool rotate(const GeometricDataHolder& data) override;

    /**
     * @brief Translate the 2D shape.
     * @param data A GeometricDataHolder containing the translation parameters.
     * @return `true` if the translation was successful, `false` if there was an error.
     */
    bool translate(const GeometricDataHolder& data) override;

    /**
     * @brief Get the CGAL Polygon_2 object representing the shape.
     * @return A const reference to the `CGAL::Polygon_2` object.
     */
    const gtp::Polygon2D& getPolygon() const;

protected:
    /**
     * @brief CGAL Polygon object storing the 2D shape.
     */
    gtp::Polygon2D polygon_;
};

VORMAKER_NAMESPACE_CLOSE

#endif // SHAPE2D_H
