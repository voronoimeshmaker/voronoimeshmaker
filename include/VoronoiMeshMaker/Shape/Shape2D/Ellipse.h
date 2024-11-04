////==============================================================================
//// Name        : Ellipse.h
//// Author      : Joao Flavio Vieira de Vasconcellos
//// Version     : 1.0
//// Description : Class for handling two-dimensional surfaces in the form of 
////               an ellipse
////
//// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos 
////                                      (jflavio at iprj.uerj.br)
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////==============================================================================
//
//#ifndef __VORONOMESHVMAKER_ELLIPSE_H__
//#define __VORONOMESHVMAKER_ELLIPSE_H__
//
////==============================================================================
//// Includes da biblioteca VoronoiMeshMaker
////==============================================================================
//
//#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>
//
//VORMAKER_NAMESPACE_OPEN
//
///**
// * @class Ellipse
// * @brief Class for handling two-dimensional surfaces in the form of an ellipse.
// * 
// * This class inherits from Shape2D and is used to represent elliptical shapes.
// * 
// * @ingroup Domain
// * @ingroup Domain2D
// * @ingroup Boundary2D
// */
//class Ellipse : public Shape2D
//{
//    
////==============================================================================
//// Class ID
////==============================================================================    
//    
//public:       
//    /**
//     * @brief Get the class name.
//     *
//     * @return A string view representing the name of this class.
//     */
//    virtual std::string_view className() const noexcept override {
//        return "Ellipse";
//    }
//
//    /**
//     * @brief Get the class ID.
//     *
//     * @return A ClassID enum representing the unique ID of this class.
//     */
//    static ClassID getClassID() {
//        return ClassID::Ellipse; 
//    }      
//    
////==============================================================================
//// Constructors / Destructor
////==============================================================================
//
//public:
//    
//    /**
//     * @brief Default constructor.
//     */
//    Ellipse() noexcept = default;
//
//    /**
//     * @brief Copy constructor.
//     */
//    Ellipse(const Ellipse&) noexcept = default;
//
//    /**
//     * @brief Move constructor.
//     */
//    Ellipse(Ellipse&&) noexcept = default;
//
//    /**
//     * @brief Destructor.
//     */
//    virtual ~Ellipse() noexcept = default;
//
//    /**
//     * @brief Constructs an Ellipse object using a GeometricDataHolder.
//     * 
//     * @param data The GeometricDataHolder containing the shape parameters.
//     */
//    Ellipse(const GeometricDataHolder& data);
//
//    /**
//     * @brief Copy assignment operator.
//     */
//    Ellipse& operator=(const Ellipse& other) noexcept = default;
//
//    /**
//     * @brief Move assignment operator.
//     */
//    Ellipse& operator=(Ellipse&& other) noexcept = default;    
//};  
//
////==============================================================================
//// Typedefs
////==============================================================================
//
///**
// * @typedef PtrEllipseShared
// * @brief Defines a shared pointer to an Ellipse.
// *
// * This type is useful when multiple entities need to share ownership of an Ellipse instance.
// */
//using PtrEllipseShared = std::shared_ptr<Ellipse>; ///< Smart pointer type for Ellipse
//
///**
// * @typedef PtrConstEllipseShared
// * @brief Defines a shared pointer to a const Ellipse.
// *
// * This type is useful when multiple entities need to share ownership of a const Ellipse instance,
// * ensuring that none of them can modify the instance.
// */
//using PtrConstEllipseShared = std::shared_ptr<const Ellipse>; ///< Smart pointer type for constant Ellipse
//
//VORMAKER_NAMESPACE_CLOSE
//
//#endif /* __VORONOMESHVMAKER_ELLIPSE_H__ */
