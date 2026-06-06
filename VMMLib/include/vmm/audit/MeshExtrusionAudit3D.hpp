//==============================================================================
// File        : MeshExtrusionAudit3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Basic audit for extruded 3D FV meshes.
//==============================================================================
#pragma once

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/mesh/FiniteVolumeMesh3D.hpp>

namespace vmm::audit {

class MeshExtrusionAudit3D final {
public:
    [[nodiscard]] MeshAuditReport run(const vmm::mesh::FiniteVolumeMesh3D& mesh) const;
};

[[nodiscard]] MeshAuditReport audit_extruded_mesh_3d(const vmm::mesh::FiniteVolumeMesh3D& mesh);

} // namespace vmm::audit
