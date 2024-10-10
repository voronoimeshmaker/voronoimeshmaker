//==============================================================================
// Name        : Nodes2D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Nodes2D class, derived from Nodes.
//
// Copyright   : Copyright (C) 2024
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
 * @file Nodes2D.h
 * @brief Declares the Nodes2D class for representing 2D nodes.
 *
 * The Nodes2D class provides methods for handling collections of nodes in 2D space.
 *
 * @ingroup nodes2d
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_NODES2D_H__
#define __VORONOIMESHMAKER_NODES2D_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Nodes/Nodes.h>
#include <VoronoiMeshMaker/Misc/namespace.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Nodes2D
 * @brief Represents a collection of 2D nodes.
 *
 * Derived from the Nodes base class, the Nodes2D class implements
 * methods specific to 2D nodes.
 */
class Nodes2D : public Nodes {

public:

    Nodes2D() noexcept = default;                      ///< Default constructor
    Nodes2D(const Nodes2D&) noexcept = default;        ///< Copy constructor
    Nodes2D(Nodes2D&&) noexcept = default;             ///< Move constructor

    ~Nodes2D() noexcept override = default;            ///< Destructor

    Nodes2D& operator=(const Nodes2D&) noexcept = default; ///< Copy assignment
//    Nodes2D& operator=(Nodes2D&&) noexcept = default;      ///< Move assignment
//
//    /**
//     * @brief Returns the dimension of the nodes.
//     *
//     * @return The dimension, which is 2 for Nodes2D.
//     */
//    int dimension() const override;
//
//    /**
//     * @brief Returns the number of nodes.
//     *
//     * @return The number of nodes in the collection.
//     */
//    std::size_t size() const override;
//
//    /**
//     * @brief Adds a node to the collection.
//     *
//     * @param point The 2D point to add.
//     */
//    void addNode(const gtp::Point2D& point);
//
//    /**
//     * @brief Gets the vector of nodes.
//     *
//     * @return A constant reference to the vector of 2D points.
//     */
//    const gtp::VecPoint2D& getNodes() const;

protected:

//    gtp::VecPoint2D nodes_;   ///< Vector of 2D nodes

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_NODES2D_H__
