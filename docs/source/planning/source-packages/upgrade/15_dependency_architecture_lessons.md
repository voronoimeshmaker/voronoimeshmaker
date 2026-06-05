# Architecture lessons from other mesh projects

The goal is not to copy all functionality from these projects. The goal is to extract architectural patterns useful for VMM.

## OpenFOAM polyMesh

Use as the main conceptual reference for finite-volume topology.

Adopt:

- points/nodes,
- faces,
- owner,
- neighbour,
- boundary patches,
- mesh validity checks.

Do not copy:

- OpenFOAM file format as primary format,
- solver coupling,
- object model.

## MeshKernel

Useful for hydraulic and environmental workflows.

Adopt:

- separation between API and backend,
- mesh editing operations,
- refinement by polygons or samples,
- environmental meshing workflow,
- stable interface for higher-level tools.

## Gmsh

Useful architectural lessons:

- separate geometry, mesh, solver and post-processing,
- physical groups as inspiration for patches,
- size fields,
- scriptable configuration,
- multiple APIs.

Do not turn VMM into a general CAD plus FEM mesher.

## CGAL

Initial geometric backend.

Use for:

- Delaunay triangulation,
- constrained triangulation as auxiliary tool,
- clipping,
- AABB queries,
- robust geometric predicates.

Do not expose CGAL types in public VMM API.

## JIGSAW

Possible future backend for geophysical Delaunay/Voronoi and metric fields.

Use as inspiration for:

- size fields,
- metric-driven meshing,
- geophysical workflows,
- Delaunay/Voronoi/Power diagrams.

Do not make it mandatory in the first version.

## Voro++

Potential future backend for 3D Voronoi cell computation.

Use for:

- cell-by-cell 3D Voronoi,
- volumes,
- centroids,
- neighbour-by-face information.

Still required in VMM:

- global face reconciliation,
- owner/neighbour audit,
- patches,
- GIS clipping,
- HDF5 export.

## MMG and Omega_h

Use as remeshing references.

Adopt:

- metric-driven remeshing,
- remeshing plans,
- batch operations,
- deterministic adaptation,
- HPC/DOD thinking.

Do not copy simplicial remeshing as final VMM mesh model.

## MOAB

Use as inspiration for:

- handles,
- ranges,
- sets,
- tags,
- metadata,
- mesh as a database.

Keep implementation DOD and VMM-specific.

## DUNE

Use as inspiration for:

- thin interfaces,
- backend independence,
- static polymorphism where useful,
- grid implementation separation.

## p4est

Use as long-term inspiration for:

- adaptive refinement,
- global/local ids,
- partitioning,
- ghost cells,
- remap tables.

## MFEM

Use as inspiration for separating:

- mesh,
- discretisation,
- linear algebra.

This reinforces that VMM should not assemble solver matrices.

## Formats

Primary:

- MohidNG HDF5 package.

Diagnostic:

- VTK/VTU,
- text audit.

Future:

- XDMF/HDF5,
- UGRID/CF-NetCDF,
- Exodus.
