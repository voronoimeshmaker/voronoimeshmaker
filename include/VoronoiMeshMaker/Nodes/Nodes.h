//==============================================================================
// Name        : Nodes.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Nodes base class.
//
// Copyright   : Copyright (C) 2024 Joao
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
 * @file Nodes.h
 * @brief Declares the Nodes base class for representing collections of nodes.
 *
 * This class serves as the base for Nodes2D and Nodes3D classes.
 *
 * @ingroup nodes
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_NODES_H__
#define __VORONOIMESHMAKER_NODES_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Nodes
 * @brief Base class for collections of nodes.
 *
 * The Nodes class provides a common interface for handling nodes in 2D and 3D.
 */
class Nodes {

public:

    Nodes() noexcept = default;                   ///< Default constructor
    Nodes(const Nodes&) noexcept = default;       ///< Copy constructor
    Nodes(Nodes&&) noexcept = default;            ///< Move constructor

    virtual ~Nodes() noexcept = default;          ///< Destructor

    Nodes& operator=(const Nodes&) noexcept = default; ///< Copy assignment
    Nodes& operator=(Nodes&&) noexcept = default;      ///< Move assignment

    /**
     * @brief Returns the dimension of the nodes.
     *
     * Pure virtual function to get the dimension (2 or 3).
     *
     * @return The dimension of the nodes.
     */
    virtual int dimension() const = 0;

    /**
     * @brief Returns the number of nodes.
     *
     * Pure virtual function to get the number of nodes.
     *
     * @return The number of nodes.
     */
    virtual std::size_t size() const = 0;

protected:

    // Protected members can be added here in the future

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_NODES_H__
