# Architecture overview

## Layered architecture

Recommended modules:

```text
vmm::geometry
    Own geometric types.
    No public CGAL, GEOS, GDAL or PROJ types.

vmm::gis
    GIS readers and CRS conversion.
    GDAL/OGR, PROJ and GEOS hidden here.

vmm::sites
    Site sets, site generation and site edit batches.

vmm::tessellation
    Tessellation algorithms.
    Strategy + Registry + Concepts + Traits.
    Initial backend: CGAL.
    Future backends: JIGSAW, Voro++.

vmm::mesh
    Canonical finite-volume mesh.
    NodeTable, CellTable, FaceTable, PatchTable.

vmm::audit
    Index audit, connectivity audit, geometry audit, projection audit.

vmm::remeshing
    Remeshing by site edit plans.

vmm::stencil
    Solver-neutral finite-volume stencil graph.

vmm::io
    MohidNG HDF5 writer.
    VTK/VTU writer.
    Text audit writer.
```

## Core pipeline

```text
GeoDomain2D or SyntheticDomain2D
    -> SiteGenerator2D
    -> TessellationAlgorithm2D
    -> Clipped Voronoi cells
    -> FaceReconstruction2D
    -> FiniteVolumeMesh2D
    -> MeshAudit2D
    -> Optional RemeshingPlan2D
    -> Optional rebuild
    -> StencilGraph2D
    -> HDF5 / VTK / text audit
```

## Algorithm selection

Use registry and traits at the boundary:

```cpp
auto tessellator =
    TessellationRegistry2D::instance().create("cgal_clipped_voronoi_2d");
```

Do not use virtual dispatch per cell or face.

## Backend isolation

Example:

```text
vmm::backend::cgal
    CGAL-specific implementation files

vmm::backend::gdal
    GDAL/OGR-specific implementation files

vmm::backend::proj
    CRS transformation implementation

vmm::backend::geos
    geometry cleaning and validation

vmm::backend::voro
    optional future Voro++ backend
```

The rest of VMM uses VMM-owned types only.

## Canonical mesh

The canonical mesh is face-based:

```text
nodes
faces
owner
neighbour
cells
boundary_patches
```

This mirrors the finite-volume semantics of OpenFOAM polyMesh, but with VMM-owned DOD tables and VMM-owned HDF5/text formats.

## Metadata and tags

Inspired by MOAB, the VMM should eventually support:

- entity sets,
- tags,
- sparse attributes,
- dense fields,
- GIS feature references.

Keep this DOD, using tables and offsets.
