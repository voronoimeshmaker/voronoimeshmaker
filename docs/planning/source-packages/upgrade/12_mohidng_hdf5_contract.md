# MohidNG HDF5 mesh package

## Purpose

The VMM shall export a mesh package that MohidNG can read and validate.

This document records the intended VMM side of that contract.

## Required metadata

Suggested attributes:

```text
schema_version = "mohidng.voronoi_mesh_package/0.1"
producer = "VoronoiMeshMaker"
dimension = 2 or 3
coordinate_reference_system
working_coordinate_reference_system
vmm_version
generation_date
```

## Required groups for 2D

```text
/nodes/id
/nodes/xy

/cells/id
/cells/centre
/cells/area

/faces/id
/faces/owner
/faces/neighbour
/faces/centre
/faces/unit_normal
/faces/length
/faces/patch_id

/boundary_patches/id
/boundary_patches/name
/boundary_patches/type
```

## Required groups for future 3D

Likely extension:

```text
/nodes/id
/nodes/xyz

/cells/id
/cells/centre
/cells/volume

/faces/id
/faces/owner
/faces/neighbour
/faces/centre
/faces/unit_normal
/faces/area
/faces/patch_id
/faces/role

/boundary_patches/id
/boundary_patches/name
/boundary_patches/type
```

## Optional groups

Possible VMM-specific groups:

```text
/audit/index_summary
/audit/connectivity_summary
/audit/boundary_projection_anomalies
/audit/internal_face_anomalies

/stencil/internal/row_ptr
/stencil/internal/col_idx
/stencil/internal/face_id
/stencil/internal/centre_distance
/stencil/internal/normal_distance
/stencil/internal/face_measure

/stencil/boundary/owner
/stencil/boundary/face_id
/stencil/boundary/patch_id
/stencil/boundary/boundary_distance
/stencil/boundary/face_measure
```

The canonical mesh groups remain nodes, cells, faces and boundary_patches.

## Export rule

Before writing a MohidNG package, the VMM shall run:

- MeshIndexAudit,
- MeshConnectivityAudit,
- MeshGeometryAudit,
- PatchAudit.

Fatal errors shall block export.

## Integration test

The first integration test should:

1. VMM generates a simple 2D mesh.
2. VMM writes HDF5 package.
3. MohidNG reads it using its Voronoi mesh package reader.
4. MohidNG validation passes.
