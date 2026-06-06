//==============================================================================
// File        : SurfacePatch.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Named 3D boundary surface patch metadata.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <string>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/BoundarySurface3D.hpp>

namespace vmm::domain {

struct SurfacePatch final {
    std::string name;
    BoundarySurfaceType type{BoundarySurfaceTypeTraits::Wall};
};

} // namespace vmm::domain
