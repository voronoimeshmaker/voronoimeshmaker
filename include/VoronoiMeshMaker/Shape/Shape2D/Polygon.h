////==============================================================================
//// Name        : Rectangle2D.h
//// Author      : Joao Flavio Vieira de Vasconcellos
//// Version     : 1.0
//// Description : Class for handling two-dimensional surfaces in the form of 
////               a rectangle
////
//// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos 
////                                      (jflavio at iprj.uerj.br)
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////
////==============================================================================
//
//#ifndef __VORONOMESHVMAKER_RECTANGLE2D_H__
//#define __VORONOMESHVMAKER_RECTANGLE2D_H__
//
////==============================================================================
//// Includes da biblioteca VoronoiMeshMaker
////==============================================================================
//
//#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>
//
//VORMAKER_NAMESPACE_OPEN
//
///**
// * @class Rectangle2D
// * @brief Class for handling two-dimensional surfaces in the form of a rectangle.
// * 
// * This class inherits from Boundary2D and is used to generate rectangular boundaries.
// * 
// * @ingroup Domain
// * @ingroup Domain2D
// * @ingroup Boundary2D
// */
//class Rectangle2D //: public Boundary2D
//{
////
//////==============================================================================
////// Class ID
//////==============================================================================
////
////public:    
////    
////    DefineIdentity("Rectangle2D", ID::Rectangle2D);
////
//////==============================================================================
////// Constructors / Destructor
//////==============================================================================
////    
////public:
////    
////    /**
////     * @brief Default constructor.
////     */
////    Rectangle2D() noexcept = default;
////
////    /**
////     * @brief Copy constructor.
////     */
////    Rectangle2D(const Rectangle2D&) noexcept = default;
////
////    /**
////     * @brief Destructor.
////     */
////    virtual ~Rectangle2D() noexcept = default;
////    
////    /**
////     * @brief Constructs a Rectangle2D with given dimensions and position.
////     * 
////     * @param lx Length in the x-direction.
////     * @param ly Length in the y-direction.
////     * @param upperLeft Upper left corner of the rectangle (default is origin).
////     */
////    Rectangle2D(const Real& lx, const Real& ly, const Point2D& upperLeft = Point2D(0, 0));
////
//////==============================================================================
////// Operator Overloads
//////==============================================================================
////
////public:
////        
////    /**
////     * @brief Copy assignment operator.
////     */
////    Rectangle2D& operator=(const Rectangle2D&) noexcept = default;
////
//////==============================================================================
////// Pure Virtual Functions
//////==============================================================================
////    
////private:
////
////    /**
////     * @brief Generates the 2D boundary for the rectangle.
////     * 
////     * @return True if the boundary is successfully generated, false otherwise.
////     */
////    [[nodiscard]]
////    virtual bool GenerateBoundary2D() override;
////
//////==============================================================================
////// Member Variables
//////==============================================================================
////
////private:
////
////    Real    lx_ = 0;               ///< Length of the rectangle in the x-direction.
////    Real    ly_ = 0;               ///< Length of the rectangle in the y-direction.
////    Point2D upperLeft_ = {0, 0};   ///< Upper left corner of the rectangle.
//
//};  
//
////==============================================================================
//// Typedefs
////==============================================================================
//
////using PtrRectangle2D = std::shared_ptr<Rectangle2D>;
////using PtrConstRectangle2D = std::shared_ptr<const Rectangle2D>;
//
//VORMAKER_NAMESPACE_CLOSE
//
//#endif /* __VORONOMESHVMAKER_RECTANGLE2D_H__ */
