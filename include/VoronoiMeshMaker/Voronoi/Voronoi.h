//==============================================================================
// Name        : Voronoi.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Voronoi base class.
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
 * @file Voronoi.h
 * @brief Declares the Voronoi base class for representing Voronoi diagrams.
 *
 * This class serves as the base for Voronoi2D and Voronoi3D classes.
 *
 * @ingroup voronoi
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_VORONOI_H__
#define __VORONOIMESHMAKER_VORONOI_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class Voronoi
 * @brief Base class for Voronoi diagrams.
 *
 * The Voronoi class provides a common interface for Voronoi diagrams in 2D and 3D.
 */
class Voronoi {

public:

    Voronoi() noexcept = default;                   ///< Default constructor
    Voronoi(const Voronoi&) noexcept = default;     ///< Copy constructor
    Voronoi(Voronoi&&) noexcept = default;          ///< Move constructor

    virtual ~Voronoi() noexcept = default;          ///< Destructor

    Voronoi& operator=(const Voronoi&) noexcept = default; ///< Copy assignment
    Voronoi& operator=(Voronoi&&) noexcept = default;      ///< Move assignment
//
//    /**
//     * @brief Computes the Voronoi diagram.
//     *
//     * Pure virtual function to compute the Voronoi diagram.
//     */
//    virtual void computeDiagram() = 0;
//
//    /**
//     * @brief Returns the dimension of the Voronoi diagram.
//     *
//     * Pure virtual function to get the dimension (2 or 3).
//     *
//     * @return The dimension of the diagram.
//     */
//    virtual int dimension() const = 0;

protected:

    // Protected members can be added here in the future

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_VORONOI_H__
