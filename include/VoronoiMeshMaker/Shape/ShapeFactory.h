#ifndef __VORONOIMESHMAKER_SHAPEFACTORY_H__
#define __VORONOIMESHMAKER_SHAPEFACTORY_H__

#include <map>
#include <functional>
#include <memory>
#include <stdexcept>
#include <VoronoiMeshMaker/Shape/Shape.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class ShapeFactory
 * @brief Factory for creating shape objects dynamically based on type registration.
 * 
 * This factory uses a registration mechanism to associate shape creation functions with a unique ClassID,
 * enabling shapes to be created dynamically based on this ID.
 */
class ShapeFactory {
public:
    /**
     * @brief Registers a shape type in the factory using its ClassID.
     * 
     * This function associates a ClassID with a function that creates an instance of the shape
     * when provided with a GeometricDataHolder.
     * 
     * @tparam T The type of shape to be registered.
     */
    template <typename T>
    static void registerShape() {
        static_assert(std::is_base_of<Shape, T>::value, "T must derive from Shape");

        shapeRegistry[T::getClassID()] = [](const GeometricDataHolder& data) -> std::unique_ptr<Shape> {
            return std::make_unique<T>(data);
        };
    }

    /**
     * @brief Creates a shape based on its ClassID.
     * 
     * Finds the associated shape creation function in the registry using the provided ClassID and
     * returns a unique pointer to the newly created shape, initialized with the provided data.
     * 
     * @param classID The unique ID of the shape to create.
     * @param data The geometric data used to initialize the shape.
     * @return std::unique_ptr<Shape> A unique pointer to the created shape.
     * @throws std::runtime_error if the ClassID is not found in the registry.
     */
    static std::unique_ptr<Shape> createShape(ClassID classID, const GeometricDataHolder& data);

private:
    /// Registry mapping ClassIDs to shape creation functions.
    static std::map<ClassID, std::function<std::unique_ptr<Shape>(const GeometricDataHolder&)>> shapeRegistry;
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_SHAPEFACTORY_H__
