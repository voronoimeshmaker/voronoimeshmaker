//==============================================================================
// Name        : Star2D.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Class for handling two-dimensional surfaces in the form of
//               a star.
//
// Copyright   : Copyright (C) 2024 João Flávio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : This program is free software: you can redistribute it and/or modify
//               it under the terms of the GNU General Public License as published by
//               the Free Software Foundation, version 3 of the License.
//
//               This program is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty of
//               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//               GNU General Public License for more details.
//
//               You should have received a copy of the GNU General Public License
//               along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes from c++ library
//==============================================================================

#include <numbers>

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Misc/Misc.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>
#include <VoronoiMeshMaker/Shape/Shape2D/Star.h>

VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::VecPoint2D;

Star::Star(const GeometricDataHolder& _holder) : Shape2D(_holder)
{
    // Logger to track function entry and exit
    vmm::FunctionLogger logger;

    // Get the "Radius" value from the GeometricDataHolder
    auto radius = _holder.get<Real>("Radius");

    try {
        // Check if the radius value was found and if it is valid
        if (!radius.has_value() || *radius <= 0) {
            throw MakerException(
                MakerErrorCode::InvalidPolygon,
                "The radius value was not found or is invalid.",
                std::source_location::current()
            );
        }
    } catch (const MakerException& exception) {
        // Log the exception
        std::cerr << exception.what() << std::endl;
        std::cerr << _holder << "\n";
        std::cerr << "\n\nExpected variables: Radius.\n\n";
        PrintLine(std::cerr);
        std::exit(EXIT_FAILURE);
    }

    // Define constants for vertex calculations
    constexpr Real SQRT3 = std::numbers::sqrt3;
    const auto R = *radius;
    const auto SQRT32 = 0.5 * SQRT3 * R;
    const auto SQRT36 = SQRT3 * R / 6;
    const auto SQRT33 = SQRT36 * 2;
    const auto R2 = 0.5 * R;

    // Define the points of the star polygon
    VecPoint2D vertex = {
        Point2D(0, R),
        Point2D(SQRT36, R2),
        Point2D(SQRT32, R2),
        Point2D(SQRT33, 0),
        Point2D(SQRT32, -R2),
        Point2D(SQRT36, -R2),
        Point2D(0, -R),
        Point2D(-SQRT36, -R2),
        Point2D(-SQRT32, -R2)
    };

    // Create the polygon with the defined points
    ptrPolygon2DShared_ = std::make_shared<gtp::Polygon2D>(std::begin(vertex), std::end(vertex));
}

// Register the Star shape in the ShapeFactory
const bool registeredStar = []{
    vmm::FunctionLogger logger;
    vmm::ShapeFactory::registerShape<vmm::Star>();
    return true;
}();

VORMAKER_NAMESPACE_CLOSE