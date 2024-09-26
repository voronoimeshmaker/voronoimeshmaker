//==============================================================================
// Name        : Shape2D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the Shape2D class, derived from Shape.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//==============================================================================

/**
 * @file Shape2D.h
 * @brief Defines the Shape2D class, which represents 2D geometric shapes.
 * This class provides methods for calculating area, perimeter, and handling
 * rotation and translation of the shape.
 *
 * @ingroup shape2d
 * 
 * @version 1.0
 * @date 2024
 */

#ifndef __VORONOIMESHMAKER_SHAPE2D_H__
#define __VORONOIMESHMAKER_SHAPE2D_H__


//#include <CGAL/Polygon_2.h>


//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>

VORMAKER_NAMESPACE_OPEN

class Shape2D : public Shape {
    
public:
    
    friend std::ostream& operator<<(std::ostream& os, const Shape2D& holder) {
        return os;
    }

    
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
        return "Shape2D";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
    virtual ClassID classID() const noexcept override {
        return ClassID::Shape2D;
    }
    
public:
    
    Shape2D() noexcept = default;
    Shape2D(const Shape2D&) noexcept = default;
    Shape2D(Shape2D&&) noexcept = default;

    ~Shape2D() noexcept = default;
    
    Shape2D& operator=(const Shape2D& other) noexcept = default;
    Shape2D& operator=(Shape2D&& other) noexcept = default;
    
    Shape2D(const GeometricDataHolder& data);
    
    bool rotate(const GeometricDataHolder& data) override;
    bool translate(const GeometricDataHolder& data) override;
    double area() const override;
    double volume() const override; // Should return 0 for 2D shapes
    double perimeter() const override;

protected:
    
    gtp::PtrPolygon2D  ptrPolygon2D_;

};

using PtrShape2D = std::unique_ptr<Shape2D>;
using PtrConstShape2D = std::unique_ptr<const Shape2D>;

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPE2D_H__
