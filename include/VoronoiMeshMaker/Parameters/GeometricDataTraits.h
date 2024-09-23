/**
 * @file GeometricDataTraits.h
 * @brief Defines the GeometricDataTraits structure used for identifying and working with different data types.
 * 
 * @ingroup parameters
 * 
 * This file defines a generic trait structure, `GeometricDataTraits`, that provides type information and possible
 * operations for different data types.
 * 
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIFVMAKER_DATATRAITS_H__
#define __VORONOIFVMAKER_DATATRAITS_H__

//==============================================================================
//  c++ include
//==============================================================================
#include <string_view>

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================
#include <VoronoiMeshMaker/Misc/configure.h>



VORMAKER_NAMESPACE_OPEN

    /**
     * @struct GeometricDataTraits
     * @brief A generic trait structure for identifying and describing different data types.
     * 
     * Provides type information and operations for the data types used in the DataHolder class.
     * Specializations of this template will be provided for each supported data type.
     * 
     * @tparam T The type of the data.
     */
    template<typename T>
    struct GeometricDataTraits;

    /// Specialization for int
    template<>
    struct GeometricDataTraits<int> {
        static std::string name() { return "Integer"; }
        static constexpr bool is_arithmetic = true;
    };

    /// Specialization for double
    template<>
    struct GeometricDataTraits<Real> {
        static std::string name() { return "Real"; }
        static constexpr bool is_arithmetic = true;
    };

    /// Specialization for Point2D
    template<>
    struct GeometricDataTraits<gtp::Point2D> {
        static std::string name() { return "Point 2D"; }
        static constexpr bool is_arithmetic = false;
    };

    /// Specialization for Point3D
    template<>
    struct GeometricDataTraits<gtp::Point3D> {
        static std::string name() { return "Point 3D"; }
        static constexpr bool is_arithmetic = false;
    };

    /// Specialization for Vector2D
    template<>
    struct GeometricDataTraits<gtp::Vector2D> {
        static std::string name() { return "Vector 2D"; }
        static constexpr bool is_arithmetic = true;
    };

    /// Specialization for Vector3D
    template<>
    struct GeometricDataTraits<gtp::Vector3D> {
        static std::string name() { return "Vector 3D"; }
        static constexpr bool is_arithmetic = true;
    };

    /// Specialization for Vector3D
    template<>
    struct GeometricDataTraits<std::string_view> {
        static std::string name() { return "std::string_view"; }
        static constexpr bool is_arithmetic = false;
    };

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIFVMAKER_DATATRAITS_H__
