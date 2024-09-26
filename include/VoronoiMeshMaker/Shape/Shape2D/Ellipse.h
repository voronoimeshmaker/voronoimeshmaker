//==============================================================================
// Name        : Ellipse.h
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

#ifndef __VORONOMESHVMAKER_ELLIPSE_H__
#define __VORONOMESHVMAKER_ELLIPSE_H__

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Ellipse
 * @brief Class for handling two-dimensional surfaces in the form of a rectangle.
 * 
 * This class inherits from Boundary2D and is used to generate rectangular boundaries.
 * 
 * @ingroup Domain
 * @ingroup Domain2D
 * @ingroup Boundary2D
 */
class Ellipse : public Shape2D
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
        return "Ellipse";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
//    virtual ClassID classID() const noexcept override {
//        return ClassID::Ellipse;
//    }
    
    // Método estático que retorna o ClassID do Ellipse
    static ClassID getClassID() {
        return ClassID::Ellipse; 
    }      
    
//==============================================================================
// Constructors / Destructor
//==============================================================================
    
public:
    
    /**
     * @brief Default constructor.
     */
    Ellipse() noexcept = default;

    /**
     * @brief Copy constructor.
     */
    Ellipse(const Ellipse&) noexcept = default;
    Ellipse(Ellipse&&) noexcept = default;

    /**
     * @brief Destructor.
     */
    virtual ~Ellipse() noexcept = default;
    
    Ellipse(const GeometricDataHolder&);
             
    
    Ellipse& operator=(const Ellipse& other) noexcept = default;
    Ellipse& operator=(Ellipse&& other) noexcept = default;    
};  

//==============================================================================
// Typedefs
//==============================================================================

using PtrEllipse = std::shared_ptr<Ellipse>;
using PtrConstEllipse = std::shared_ptr<const Ellipse>;

VORMAKER_NAMESPACE_CLOSE

#endif /* __VORONOMESHVMAKER_ELLIPSE_H__ */
