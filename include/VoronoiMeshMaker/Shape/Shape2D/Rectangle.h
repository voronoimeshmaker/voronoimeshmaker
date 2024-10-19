//==============================================================================
// Name        : Rectangle.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Class for handling two-dimensional surfaces in the form of 
//               a rectangle
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos 
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
//
//==============================================================================

#ifndef __VORONOMESHVMAKER_RECTANGLE_H__
#define __VORONOMESHVMAKER_RECTANGLE_H__

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Rectangle
 * @brief Class for handling two-dimensional surfaces in the form of a rectangle.
 * 
 * This class inherits from Boundary2D and is used to generate rectangular boundaries.
 * 
 * @ingroup Domain
 * @ingroup Domain2D
 * @ingroup Boundary2D
 */
class Rectangle : public Shape2D
{
    
    
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
        return "Rectangle";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
//    virtual ClassID classID() const noexcept override {
//        return ClassID::Rectangle;
//    }
    
    // Método estático que retorna o ClassID do Rectangle
    static ClassID getClassID() {
        return ClassID::Rectangle; 
    }      
    
//==============================================================================
// Constructors / Destructor
//==============================================================================
    
public:
    
    /**
     * @brief Default constructor.
     */
    Rectangle() noexcept = default;

    /**
     * @brief Copy constructor.
     */
    Rectangle(const Rectangle&) noexcept = default;
    Rectangle(Rectangle&&) noexcept = default;

    /**
     * @brief Destructor.
     */
    virtual ~Rectangle() noexcept = default;
    
    Rectangle(const GeometricDataHolder&);
             
    
    Rectangle& operator=(const Rectangle& other) noexcept = default;
    Rectangle& operator=(Rectangle&& other) noexcept = default;    
};  

//==============================================================================
// Typedefs
//==============================================================================

using PtrRectangleShared = std::shared_ptr<Rectangle>; ///< Smart pointer type for Rectangle
using PtrConstRectangleShared = std::shared_ptr<const Rectangle>; ///< Smart pointer type for constant Rectangle

VORMAKER_NAMESPACE_CLOSE

#endif /* __VORONOMESHVMAKER_RECTANGLE_H__ */
