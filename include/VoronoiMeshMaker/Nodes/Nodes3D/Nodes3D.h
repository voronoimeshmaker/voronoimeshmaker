//==============================================================================
// Name        : Nodes3D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Declares the Nodes3D class, derived from Nodes.
//
// [Informações de direitos autorais e licença]
//==============================================================================

/**
 * @file Nodes3D.h
 * @brief Declares the Nodes3D class for representing 3D nodes.
 *
 * The Nodes3D class provides methods for handling collections of nodes in 3D space.
 *
 * @ingroup nodes3d
 *
 * @version 1.0
 * @date 2024
 *
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_NODES3D_H__
#define __VORONOIMESHMAKER_NODES3D_H__

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Nodes/Nodes.h>


VORMAKER_NAMESPACE_OPEN

/**
 * @class Nodes3D
 * @brief Represents a collection of 3D nodes.
 *
 * Derived from the Nodes base class, the Nodes3D class implements
 * methods specific to 3D nodes.
 */
class Nodes3D : public Nodes {

public:

    Nodes3D() noexcept = default;                      ///< Default constructor
    Nodes3D(const Nodes3D&) noexcept = default;        ///< Copy constructor
    Nodes3D(Nodes3D&&) noexcept = default;             ///< Move constructor

    ~Nodes3D() noexcept override = default;            ///< Destructor

    Nodes3D& operator=(const Nodes3D&) noexcept = default; ///< Copy assignment
    Nodes3D& operator=(Nodes3D&&) noexcept = default;      ///< Move assignment

//    /**
//     * @brief Returns the dimension of the nodes.
//     *
//     * @return The dimension, which is 3 for Nodes3D.
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
//     * @param point The 3D point to add.
//     */
//    void addNode(const gtp::Point3D& point);
//
//    /**
//     * @brief Gets the vector of nodes.
//     *
//     * @return A constant reference to the vector of 3D points.
//     */
//    const gtp::VecPoint3D& getNodes() const;
//
//protected:
//
//    gtp::VecPoint3D nodes_;   ///< Vector of 3D nodes

};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_NODES3D_H__
