//==============================================================================
// Name        : Rectangle.cpp
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

#include <iostream>

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Rectangle.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>


VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::LstPoint2D;


Rectangle :: Rectangle(const GeometricDataHolder& _holder) : Shape2D(_holder)
{

auto lx = _holder.getValue<Real>("Length");
auto ly = _holder.getValue<Real>("Width");
auto cornerValue = _holder.getValue<Point2D>("BottomLeftCorner");

    try {
        
        if (!lx.has_value() || !ly.has_value()) { 
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex::DATANOTAVAILABLE,
                true
            );
        }
            
        if (*lx <= 0 || *ly <= 0) {
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex::NEGATIVELENGHT,
                true
            );
        }
        
    } catch (const VMMException& exception) {
        
        std::cerr << "\n";
        std::cerr << exception;
        std::cerr << _holder << "\n";
        std::cerr << "\n\nVariaveis deste modelo: Length, Width and BottomLeftCorner (optional).\n\n";
        PrintLine(std::cerr);
        std::exit(EXIT_FAILURE);
    }

Point2D corner(Point2D(0,0));
    if (cornerValue.has_value()) {
        corner = *cornerValue;
    } 


LstPoint2D lstPoint2D =     {   corner
                            ,   Point2D(corner.x(), corner.y() + *ly)
                            ,   Point2D(corner.x() + *lx, corner.y() + *ly)
                            ,   Point2D(corner.x() + *lx, corner.y())
                            };

    ptrPolygon2D_ =  std::make_unique<gtp::Polygon2D>(
        std::begin(lstPoint2D), std::end(lstPoint2D)
    );

    
    lstPoint2D.clear();
    

}        

const bool registeredRectangle = []{
    vmm::ShapeFactory::registerShape<vmm::Rectangle>();
    return true;
}();

VORMAKER_NAMESPACE_CLOSE
