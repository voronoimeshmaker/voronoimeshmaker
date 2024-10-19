//==============================================================================
// Name        : Star2D.cpp
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

#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>
#include <VoronoiMeshMaker/Shape/Shape2D/Star.h>



VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::VecPoint2D;

Star :: Star(const GeometricDataHolder& _holder) : Shape2D(_holder)
{

auto radius = _holder.get<Real>("Radius");

    try {
        
        if (!radius.has_value()) { 
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex ::NEGATIVELENGHT,
                true
            );
        }
            
        if (*radius <= 0) {
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex::NEGATIVELENGHT,
                true
            );            
        };
            
    } catch (const VMMException& exception) {
        std::cerr << exception;
        std::cerr << _holder << "\n";
        std::cerr << "\n\nVariaveis deste modelo: Radius.\n\n";
        PrintLine(std::cerr);
        std::exit(EXIT_FAILURE);
    }

const auto SQRT3  = sqrt(3) * *radius;
const auto SQRT32 = 0.5 * SQRT3;
const auto SQRT36 = SQRT3 / 6;
const auto SQRT33 = SQRT36 * 2;
const auto R2     = 0.5 * *radius;

VecPoint2D      vertex = {  Point2D(0, *radius)
                         ,  Point2D(SQRT36, R2 ) 
                         ,  Point2D(SQRT32, R2 ) 
                         ,  Point2D(SQRT33, 0 ) 
                         ,  Point2D(SQRT32, - R2 ) 
                         ,  Point2D(SQRT36, - R2 ) 
                        ,   Point2D(0, - *radius)
                         ,  Point2D(- SQRT36, - R2 ) 
                         ,  Point2D(- SQRT32, - R2 ) 
                        };
    


    ptrPolygon2DShared_ =  std::make_shared<gtp::Polygon2D>(
        std::begin(vertex), std::end(vertex)
    );
    vertex.clear();
    

}           
        
const bool registeredStar = []{
    vmm::ShapeFactory::registerShape<vmm::Star>();
    return true;
}();
        
VORMAKER_NAMESPACE_CLOSE

        
       