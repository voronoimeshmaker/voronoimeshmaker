//==============================================================================
// Name        : Ellipse.cpp
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
// Includes do c++
//==============================================================================

#include <execution> // Header necessário para execução paralela com STL

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Ellipse.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>


VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::VecPoint2D;
using gtp::Polygon2D;


Ellipse :: Ellipse(const GeometricDataHolder& _holder) : Shape2D(_holder)
{

auto lx = _holder.getValue<Real>("Length");
auto ly = _holder.getValue<Real>("Width");
auto npto = _holder.getValue<int>("NPoints");
const int NPTOMIN(4);
    try {
        
        if (!lx.has_value() || !ly.has_value()|| !npto.has_value()) { 
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

        if (*npto < NPTOMIN) {
            throw VMMException(
                SourceInfo(__FILE__, __LINE__, __func__),
                VMMExceptionIndex::WrongNPoints,
                true
            );
        }
        
    } catch (const VMMException& exception) {
        
        std::cerr << "\n";
        std::cerr << exception;
        std::cerr << _holder << "\n";
        std::cerr << "\n\nVariaveis deste modelo: Length, Width and NPoints\n\n";
        PrintLine(std::cerr);
        std::exit(EXIT_FAILURE);
    }

const Real          DTHETA = 2 * Pi() / *npto;    
const Real          LX = 0.5 * *lx;
const Real          LY = 0.5 * *ly;


VecPoint2D          vecPoint2D(*npto);    
int                 tPoint = 0;    
    
//==============================================================================
// Create an index vector to be used for parallel processing
//==============================================================================

    VecInt          indices(*npto);
    std::iota(indices.begin(), indices.end(), 0);
    
      // Parallelize the generation of ellipse points using a lambda function
    std::for_each(std::execution::par, indices.begin(), indices.end(), [&](int idx) {
        Real ang = DTHETA * idx;
        vecPoint2D[idx] = Point2D(LX * cos(ang), LY * sin(ang));
    });
    
    ptrPolygon2D_ = 
                std::make_unique<Polygon2D> (   std::begin(vecPoint2D)
                                            ,   std::end(vecPoint2D)
                                            );
 
    vecPoint2D.clear();

}        

const bool registeredEllipse = []{
    vmm::ShapeFactory::registerShape<vmm::Ellipse>();
    return true;
}();

VORMAKER_NAMESPACE_CLOSE






//
//
////==============================================================================
//// includes da biblioteca C++
////==============================================================================
//
//

//        
////==============================================================================
//// includes da biblioteca GridVoronoi++
////==============================================================================
//
//#include <VoronoiMeshMaker/Shape/Shape2D/Ellipse.h>
//#include <VoronoiMeshMaker/Error/MakerException.h>
//#include <VoronoiMeshMaker/Shape/ShapeFactory.h>
//
////==============================================================================
//// includes da biblioteca CGAL 
////==============================================================================
//
////
//////==============================================================================
////// Typedef
//////==============================================================================
////
////using VecInt = std::vector<int>;
////using VecPoint2D = std::vector<Point2D>;
////
////
//VORMAKER_NAMESPACE_OPEN
////
////#undef  __FUNCT__
////#define __FUNCT__ "Ellipse2D :: Ellipse2D (const Real&, const Real&, const int&)"
////Ellipse2D :: Ellipse2D    (   const Real&  _lx
////                            ,   const Real& _ly
////                            ,   const int& _nptos
////                            )  
////                            :   lx_(_lx)
////                            ,   ly_ (_ly)
////                            ,   nptos_(_nptos)
////{
////
////const int NPTOMIN(4);
////    
////        try {
////                if (lx_ <= 0 || ly_ <= 0) {
////                    throw  
////                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
////                                    ,   vmm::NEGATIVELENGHT
////                                    ,   true
////                                    );
////                }
////
////                if (nptos_ < NPTOMIN) {
////                    throw  
////                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
////                                    ,   vmm::WRONGNPOINTS
////                                    ,   "Numero de pontos deverá ser maior do que 3."
////                                    ,   true
////                                    );
////                }
////                
////                if (!GenerateBoundary2D ()) {
////                    throw  
////                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
////                                    ,   vmm::NULLPOLYGON
////                                    ,   true
////                                    );
////                }        
////                
////        } catch (const VMMException& _adsException) {
////            std::cout << _adsException;
////            exit (EXIT_FAILURE);
////        };
////        
////        auto flag = TestBoundary2D();
////        
////    
////}
////
////
////
////#undef  __FUNCT__
////#define __FUNCT__ "bool Boundary2D :: GenerateBoundary2D ()"
////bool 
////Ellipse2D :: GenerateBoundary2D () 
////{
////
//////==============================================================================
////// Calculando as variaveis do problema 
//////==============================================================================
////

////
////VecPoint2D          vecPoint2D(nptos_);    
////int                 tPoint = 0;    
////    
//////==============================================================================
////// Create an index vector to be used for parallel processing
//////==============================================================================
////
////    VecInt          indices(nptos_);
////    std::iota(indices.begin(), indices.end(), 0);
////    
////      // Parallelize the generation of ellipse points using a lambda function
////    std::for_each(std::execution::par, indices.begin(), indices.end(), [&](int idx) {
////        Real ang = DTHETA * idx;
////        vecPoint2D[idx] = Point2D(LX * cos(ang), LY * sin(ang));
////    });
////    
////    ptrBoundaryPolygon_ = 
////                std::make_shared<Polygon2D> (   std::begin(vecPoint2D)
////                                            ,   std::end(vecPoint2D)
////                                            );
//// 
////    vecPoint2D.clear();
////    
////    if (ptrBoundaryPolygon_->is_counterclockwise_oriented()) {
////        ptrBoundaryPolygon_->reverse_orientation();
////    }
////    
////    return true;
////};
////
//// Explicitly instantiate for known shapes
////template void ShapeFactory::registerShape<Shape2D>();
////template std::unique_ptr<Shape> ShapeFactory::createShape<Shape2D>(const GeometricDataHolder&);
////
//
//VORMAKER_NAMESPACE_CLOSE
