//==============================================================================
// Name        : Shape.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the Shape class and its interface for all shapes.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//==============================================================================

/**
 * @file Shape.h
 * @brief Defines the Shape class, which serves as the abstract base class for all shapes.
 * This class provides the interface for methods related to shapes, including rotation, 
 * translation, area, volume, and perimeter calculations.
 *
 * @ingroup shape
 * 
 * @version 1.0
 * @date 2024
 */

#ifndef __VORONOIMESHMAKER_SHAPE_H__
#define __VORONOIMESHMAKER_SHAPE_H__

#include <iostream>
#include <memory>
#include <VoronoiMeshMaker/Misc/ID.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

class Shape : public ID {
    
//==============================================================================
// Constructors / Destructor
//==============================================================================

public:
        
    Shape() noexcept = default;
    Shape(const Shape&) noexcept = default;
    Shape(Shape&& other) noexcept = default;
    ~Shape() noexcept = default;
    
    Shape& operator=(const Shape& other) noexcept = default;
    Shape& operator=(Shape&& other) noexcept = default;
    
    Shape(const GeometricDataHolder&);
    
    
public:

    virtual std::string_view className() const noexcept override = 0;
    virtual ClassID classID() const noexcept override = 0;

    virtual bool rotate(const GeometricDataHolder& data) = 0;
    virtual bool translate(const GeometricDataHolder& data) = 0;
    virtual double area() const = 0;
    virtual double volume() const = 0;
    virtual double perimeter() const = 0;
    
    
private:    
    
 PtrGeometricDataHolder     ptrDataHolder_;    
};

using PtrShape = std::unique_ptr<Shape>;
using PtrConstShape = std::unique_ptr<Shape const>;

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPE_H__
