//==============================================================================
// Name        : Shape2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the Shape2D class.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
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

#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

Shape2D::Shape2D(const GeometricDataHolder& data) : Shape(data) {
    // Initialize the polygon with points from the data holder if necessary
}

std::string_view Shape2D::className() const noexcept {
    return "Shape2D";
}

ClassID Shape2D::classID() const noexcept {
    return ClassID::Shape2D;
}

Real Shape2D::area() const {
    return std::abs(polygon_.area());  // Use CGAL to calculate the area
}

Real Shape2D::perimeter() const {
    Real perimeter = 0;
    for (auto edge = polygon_.edges_begin(); edge != polygon_.edges_end(); ++edge) {
        perimeter += std::sqrt(edge->squared_length());
    }
    return perimeter;
}

bool Shape2D::validatePolygon() {
    if (!polygon_.is_simple()) {
        return false;  // Invalid polygon
    }

    if (polygon_.is_counterclockwise_oriented()) {
        polygon_.reverse_orientation();  // Make the orientation clockwise
    }

    return true;
}

bool Shape2D::rotate(const GeometricDataHolder& data) {
    // Extract rotation parameters from the data holder
    // Apply rotation using CGAL or manual transformations
    return true;  // Return true if successful, false otherwise
}

bool Shape2D::translate(const GeometricDataHolder& data) {
    // Extract translation parameters from the data holder
    // Apply translation using CGAL or manual transformations
    return true;  // Return true if successful, false otherwise
}

const gtp::Polygon2D& Shape2D::getPolygon() const {
    return polygon_;
}

VORMAKER_NAMESPACE_CLOSE
