#ifndef __VORMAKER_ELLIPSE_H__
#define __VORMAKER_ELLIPSE_H__

//==============================================================================
// Name        : Ellipse.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.1
// Description : Class for handling two-dimensional surfaces in the form of 
//               an ellipse
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
//==============================================================================

//==============================================================================
// Includes from C++
//==============================================================================
#include <execution>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================
#include <VoronoiMeshMaker/Shape/Shape2D/Ellipse.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>

VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::VecPoint2D;
using gtp::Polygon2D;

/**
 * @ingroup shape2d
 * @class Ellipse
 * @brief Class for representing a two-dimensional ellipse shape.
 *
 * This class handles the construction of an ellipse based on the parameters provided
 * through a GeometricDataHolder. It makes use of MakerException for error handling.
 */
Ellipse::Ellipse(const GeometricDataHolder& _holder) : Shape2D(_holder)
{
    auto lx = _holder.get<Real>("Length");
    auto ly = _holder.get<Real>("Width");
    auto npto = _holder.get<int>("NPoints");
    const int NPTOMIN(4);

    try {
        if (!lx.has_value() || !ly.has_value() || !npto.has_value()) {
            throw MakerException(
                MakerErrorCode::NullPointer,
                "Missing required geometric parameters: Length, Width, or NPoints.",
                std::source_location::current()
            );
        }

        if (*lx <= 0 || *ly <= 0) {
            throw MakerException(
                MakerErrorCode::InvalidPolygon,
                "Length and Width must be greater than zero.",
                std::source_location::current()
            );
        }

        if (*npto < NPTOMIN) {
            throw MakerException(
                MakerErrorCode::InvalidPolygon,
                "Number of points must be greater than or equal to 4.",
                std::source_location::current()
            );
        }
    } catch (const MakerException& exception) {
        std::cerr << "\nException: " << exception.what() << "\n";
        std::exit(EXIT_FAILURE);
    }

    const Real DTHETA = 2 * M_PI / *npto;
    const Real LX = 0.5 * *lx;
    const Real LY = 0.5 * *ly;

    VecPoint2D vecPoint2D(*npto);
    VecInt indices(*npto);
    std::iota(indices.begin(), indices.end(), 0);

    // Parallelize the generation of ellipse points using a lambda function
    std::for_each(std::execution::par, indices.begin(), indices.end(), [&](int idx) {
        Real ang = DTHETA * idx;
        vecPoint2D[idx] = Point2D(LX * std::cos(ang), LY * std::sin(ang));
    });

    ptrPolygon2DShared_ = std::make_shared<Polygon2D>(std::begin(vecPoint2D), std::end(vecPoint2D));

    vecPoint2D.clear();
}

/**
 * @brief Registers the Ellipse shape within the ShapeFactory.
 */
const bool registeredEllipse = [] {
    vmm::ShapeFactory::registerShape<vmm::Ellipse>();
    return true;
}();

//// Explicitly instantiate the template for known shapes
//template void vmm::ShapeFactory::registerShape<Ellipse>();
//template std::unique_ptr<vmm::Shape> vmm::ShapeFactory::createShape<Ellipse>(const GeometricDataHolder&);

VORMAKER_NAMESPACE_CLOSE

#endif // __VORMAKER_ELLIPSE_H__
