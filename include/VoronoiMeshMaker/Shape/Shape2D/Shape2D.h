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
 * 
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

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Shape2D
 * @brief Represents a 2D geometric shape.
 * 
 * The Shape2D class is derived from the Shape class and provides methods
 * for manipulating and calculating properties of 2D shapes, such as area,
 * perimeter, rotation, and translation.
 */
class Shape2D : public Shape {
    
public:
    
    /**
     * @brief Overloads the output stream operator for Shape2D.
     * 
     * This allows the Shape2D objects to be printed directly to output streams.
     * 
     * @param os The output stream.
     * @param holder The Shape2D object to be printed.
     * @return The output stream after printing the Shape2D.
     */
    friend std::ostream& operator<<(std::ostream& os, const Shape2D& holder) {
        return os; // Implementar a saída conforme necessário
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
    
    Shape2D() noexcept = default; ///< Default constructor
    Shape2D(const Shape2D&) noexcept = default; ///< Copy constructor
    Shape2D(Shape2D&&) noexcept = default; ///< Move constructor

    ~Shape2D() noexcept = default; ///< Destructor
    
    Shape2D& operator=(const Shape2D& other) noexcept = default; ///< Copy assignment
    Shape2D& operator=(Shape2D&& other) noexcept = default; ///< Move assignment
    
    /**
     * @brief Constructs a Shape2D object using a GeometricDataHolder.
     * 
     * This constructor initializes the Shape2D object based on the data provided
     * in the GeometricDataHolder.
     * 
     * @param data The GeometricDataHolder containing the shape parameters.
     */
    Shape2D(const GeometricDataHolder& data);
    
    /**
     * @brief Rotates the shape based on the provided data.
     * 
     * @param data The GeometricDataHolder containing rotation parameters.
     * @return True if the rotation was successful, false otherwise.
     */
    bool rotate(const GeometricDataHolder& data) override;

    /**
     * @brief Translates the shape based on the provided data.
     * 
     * @param data The GeometricDataHolder containing translation parameters.
     * @return True if the translation was successful, false otherwise.
     */
    bool translate(const GeometricDataHolder& data) override;

    /**
     * @brief Calculates the area of the shape.
     * 
     * @return The area of the shape.
     */
    Real area() const override;

    /**
     * @brief Returns the volume of the shape.
     * 
     * Since this is a 2D shape, it returns 0.
     * 
     * @return The volume, which is 0 for 2D shapes.
     */
    Real volume() const override; 

    /**
     * @brief Calculates the perimeter of the shape.
     * 
     * @return The perimeter of the shape.
     */
    Real perimeter() const override;

    /**
     * @brief Gets the polygon pointer of the shape.
     * 
     * @return A constant reference to the PtrPolygon2DShared.
     */
    inline const gtp::PtrPolygon2DShared& getPtrPolygonShared() const {
            return ptrPolygon2DShared_;
    }

protected:
    
    gtp::PtrPolygon2DShared  ptrPolygon2DShared_; ///< Pointer to the polygon representation of the shape

};

/**
 * @typedef PtrShape2DShared
 * @brief Defines a shared pointer to a Shape2D.
 *
 * This type is useful when multiple entities need to share ownership of a Shape2D instance.
 */
using PtrShape2DShared = std::shared_ptr<Shape2D>; ///< Smart pointer type for Shape2D

/**
 * @typedef PtrConstShape2DShared
 * @brief Defines a shared pointer to a const Shape2D.
 *
 * This type is useful when multiple entities need to share ownership of a const Shape2D instance,
 * ensuring that none of them can modify the instance.
 */
using PtrConstShape2DShared = std::shared_ptr<const Shape2D>; ///< Smart pointer type for constant Shape2D

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPE2D_H__
