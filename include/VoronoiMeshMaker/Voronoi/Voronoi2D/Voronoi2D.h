//==============================================================================
// Name        : Voronoi2D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Voronoi2D class, derived from Voronoi.
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
 * @file Voronoi2D.h
 * @brief Declares the Voronoi2D class for representing 2D Voronoi diagrams.
 *
 * The Voronoi2D class provides methods for computing and accessing
 * Voronoi diagrams in 2D space.
 *
 * @ingroup voronoi2d
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_VORONOI2D_H__
#define __VORONOIMESHMAKER_VORONOI2D_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Voronoi/Voronoi.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Voronoi2D
 * @brief Represents a 2D Voronoi diagram.
 *
 * Derived from the Voronoi base class, the Voronoi2D class implements
 * methods specific to 2D Voronoi diagrams.
 */
class Voronoi2D : public Voronoi {

public:

    Voronoi2D() noexcept = default;                      ///< Default constructor
    Voronoi2D(const Voronoi2D&) noexcept = default;      ///< Copy constructor
    Voronoi2D(Voronoi2D&&) noexcept = default;           ///< Move constructor

    ~Voronoi2D() noexcept override = default;            ///< Destructor

//    Voronoi2D& operator=(const Voronoi2D&) noexcept = default; ///< Copy assignment
//    Voronoi2D& operator=(Voronoi2D&&) noexcept = default;      ///< Move assignment
//
//    /**
//     * @brief Computes the 2D Voronoi diagram.
//     *
//     * Implements the method to compute the Voronoi diagram in 2D.
//     */
//    void computeDiagram() override;
//
//    /**
//     * @brief Returns the dimension of the Voronoi diagram.
//     *
//     * @return The dimension, which is 2 for Voronoi2D.
//     */
//    int dimension() const override;
//
//    /**
//     * @brief Sets the input points for the Voronoi diagram.
//     *
//     * @param points A vector of 2D points.
//     */
//    void setInputPoints(const gtp::VecPoint2D& points);
//
//    /**
//     * @brief Gets the computed Voronoi edges.
//     *
//     * @return A vector of segments representing the Voronoi edges.
//     */
//    const gtp::VecSegment2D& getEdges() const;

protected:
//
//    gtp::VecPoint2D inputPoints_;     ///< Input points for the diagram
//    gtp::VecSegment2D voronoiEdges_;  ///< Computed Voronoi edges

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_VORONOI2D_H__
