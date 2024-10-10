//==============================================================================
// Name        : Voronoi3D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Voronoi3D class, derived from Voronoi.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
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

/**
 * @file Voronoi3D.h
 * @brief Declares the Voronoi3D class for representing 3D Voronoi diagrams.
 *
 * The Voronoi3D class provides methods for computing and accessing
 * Voronoi diagrams in 3D space.
 *
 * @ingroup voronoi3d
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_VORONOI3D_H__
#define __VORONOIMESHMAKER_VORONOI3D_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Voronoi/Voronoi.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Voronoi3D
 * @brief Represents a 3D Voronoi diagram.
 *
 * Derived from the Voronoi base class, the Voronoi3D class implements
 * methods specific to 3D Voronoi diagrams.
 */
class Voronoi3D : public Voronoi {

public:

    Voronoi3D() noexcept = default;                      ///< Default constructor
    Voronoi3D(const Voronoi3D&) noexcept = default;      ///< Copy constructor
    Voronoi3D(Voronoi3D&&) noexcept = default;           ///< Move constructor

    ~Voronoi3D() noexcept override = default;            ///< Destructor

    Voronoi3D& operator=(const Voronoi3D&) noexcept = default; ///< Copy assignment
    Voronoi3D& operator=(Voronoi3D&&) noexcept = default;      ///< Move assignment

//    /**
//     * @brief Computes the 3D Voronoi diagram.
//     *
//     * Implements the method to compute the Voronoi diagram in 3D.
//     */
//    void computeDiagram() override;
//
//    /**
//     * @brief Returns the dimension of the Voronoi diagram.
//     *
//     * @return The dimension, which is 3 for Voronoi3D.
//     */
//    int dimension() const override;
//
//    /**
//     * @brief Sets the input points for the Voronoi diagram.
//     *
//     * @param points A vector of 3D points.
//     */
//    void setInputPoints(const gtp::VecPoint3D& points);
//
//    /**
//     * @brief Gets the computed Voronoi cells.
//     *
//     * @return A vector of polyhedra representing the Voronoi cells.
//     */
//    const std::vector<gtp::Polyhedron3D>& getCells() const;

protected:

//    gtp::VecPoint3D inputPoints_;                      ///< Input points for the diagram
//    std::vector<gtp::Polyhedron3D> voronoiCells_;      ///< Computed Voronoi cells

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_VORONOI3D_H__
