//==============================================================================
// File        : ExtrudedFiniteVolumeMesh3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Vertical extrusion from 2D FV meshes to 3D FV meshes.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <span>
#include <string>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>
#include <vmm/mesh/FiniteVolumeMesh3D.hpp>

namespace vmm::mesh {

struct ExtrudedFiniteVolumeMesh3DOptions final {
    std::string bottom_patch_name{"bottom"};
    std::string surface_patch_name{"surface"};
    bool require_valid_2d_mesh{true};
};

[[nodiscard]] FiniteVolumeMesh3D extrude_finite_volume_mesh_3d(
    const FiniteVolumeMesh2D& mesh2d,
    std::span<const vmm::core::Real> z_interfaces,
    const ExtrudedFiniteVolumeMesh3DOptions& options = {});

} // namespace vmm::mesh
