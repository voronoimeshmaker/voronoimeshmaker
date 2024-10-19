//==============================================================================
// Name        : Rectangle2D.cpp
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

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Polygon.h>
#include <VoronoiMeshMaker/Error/MakerException.h>

VORMAKER_NAMESPACE_OPEN

///**
// * @brief Constructs a Rectangle2D with given dimensions and position.
// * 
// * @param _lx Length in the x-direction.
// * @param _ly Length in the y-direction.
// * @param _cornerPoint Upper left corner of the rectangle (default is origin).
// */
//Rectangle2D::Rectangle2D(const Real& _lx, const Real& _ly, const Point2D& _cornerPoint)
//    : lx_(_lx), ly_(_ly), upperLeft_(_cornerPoint) {
//
//    try {
//        if (lx_ <= 0 || ly_ <= 0) {
//            throw VMMException(
//                SourceInfo(__FILE__, __LINE__, __func__),
//                vmm::NEGATIVELENGHT,
//                true
//            );
//        }
//
//        if (!GenerateBoundary2D()) {
//            throw VMMException(
//                SourceInfo(__FILE__, __LINE__, __func__),
//                vmm::NULLPOLYGON,
//                true
//            );
//        }        
//    } catch (const VMMException& exception) {
//        std::cerr << exception;
//        std::exit(EXIT_FAILURE);
//    }
//    
//    auto flag = TestBoundary2D();
//}
//
///**
// * @brief Generates the boundary points for the rectangle.
// * 
// * @return True if the boundary is successfully generated.
// */
//bool Rectangle2D::GenerateBoundary2D() {
//
//    LstPoint2D lstPoint2D;
//
//    lstPoint2D.push_back(upperLeft_);                                            // Lower left corner                              
//    lstPoint2D.push_back(Point2D(upperLeft_.x(), upperLeft_.y() + ly_));         // Upper left corner
//    lstPoint2D.push_back(Point2D(upperLeft_.x() + lx_, upperLeft_.y() + ly_));   // Upper right corner
//    lstPoint2D.push_back(Point2D(upperLeft_.x() + lx_, upperLeft_.y()));         // Lower right corner
//    
//    ptrBoundaryPolygon_ = std::make_shared<Polygon2D>(
//        std::begin(lstPoint2D), std::end(lstPoint2D)
//    );
//  
//    lstPoint2D.clear();
//    
//
//    return true;
//}

VORMAKER_NAMESPACE_CLOSE
