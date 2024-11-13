//==============================================================================
// Name        : Rectangle.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Class for handling two-dimensional surfaces in the form of
//               a rectangle.
//
// Copyright   : Copyright (C) 2024 João Flávio Vieira de Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Rectangle.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>
#include <VoronoiMeshMaker/Misc/Misc.h>

VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::LstPoint2D;

Rectangle::Rectangle(const GeometricDataHolder& _holder) : Shape2D(_holder)
{
    FunctionLogger logger;
    auto lx = _holder.get<Real>("Length");
    auto ly = _holder.get<Real>("Width");
    auto cornerValue = _holder.get<Point2D>("BottomLeftCorner");

    try {
        if (!lx.has_value() || !ly.has_value()) {
            throw MakerException(
                MakerErrorCode::NullPointer,
                "The length or width value is missing.",
                std::source_location::current()
            );
        }

        if (*lx <= 0 || *ly <= 0) {
            throw MakerException(
                MakerErrorCode::InvalidPolygon,
                "The length or width value must be positive.",
                std::source_location::current()
            );
        }
    } catch (const MakerException& exception) {
        std::cerr << "\n";
        std::cerr << exception.what() << "\n";
        std::cerr << _holder << "\n";
        std::cerr << "\n\nExpected variables: Length, Width, and BottomLeftCorner (optional).\n\n";
        PrintLine(std::cerr);
        std::exit(EXIT_FAILURE);
    }

    Point2D corner(0, 0);
    if (cornerValue.has_value()) {
        corner = *cornerValue;
    }

    LstPoint2D lstPoint2D = {
        corner,
        Point2D(corner.x(), corner.y() + *ly),
        Point2D(corner.x() + *lx, corner.y() + *ly),
        Point2D(corner.x() + *lx, corner.y())
    };

    ptrPolygon2DShared_ = std::make_shared<gtp::Polygon2D>(
        std::begin(lstPoint2D), std::end(lstPoint2D)
    );

    lstPoint2D.clear();
}

// Register the Rectangle shape in the ShapeFactory
const bool registeredRectangle = []{
    FunctionLogger logger;
    ShapeFactory::registerShape<vmm::Rectangle>();
    return true;
}();

VORMAKER_NAMESPACE_CLOSE