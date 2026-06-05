# Block roadmap

This roadmap is organised by technical blocks, not calendar dates. Each block should produce something testable before the next block starts.

## Block 0, Requirements and documentation base

Objective:

Formalise scope and prepare documentation.

Deliverables:

- functional requirements,
- non-functional requirements,
- architecture overview,
- DOD policy,
- backend policy,
- relation to MohidNG page,
- block roadmap.

Exit criteria:

- documentation builds,
- requirements are versioned,
- immediate 2D scope is clear,
- future 3D scope is explicitly separate.

## Block 1, DOD finite-volume mesh 2D core

Objective:

Create the central 2D finite-volume mesh data model.

Deliverables:

- strong ids,
- NodeTable2D,
- CellTable2D,
- FaceTable2D,
- BoundaryPatchTable,
- CellFaceConnectivity,
- FiniteVolumeMesh2D.

Exit criteria:

- a simple mesh can be represented in memory,
- no per-cell object graph,
- all hot data are in contiguous arrays.

## Block 2, Face reconstruction from clipped Voronoi cells

Objective:

Convert clipped polygonal cells into explicit finite-volume faces.

Deliverables:

- shared edge detection,
- global face creation,
- owner assignment,
- neighbour assignment,
- boundary face detection,
- face centres,
- face normals,
- face lengths,
- cell-face offsets.

Exit criteria:

- simple 2D mesh has correct internal and boundary faces,
- neighbour reciprocity can be checked,
- boundary faces have invalid neighbour.

## Block 3, Boundary patches

Objective:

Represent physical boundary types explicitly.

Deliverables:

- BoundaryPatchTable implementation,
- patch ids,
- patch names,
- patch types,
- internal and boundary patch policy,
- patch assignment for simple domains.

Exit criteria:

- boundary faces have valid patches,
- internal faces have invalid boundary patch,
- island/wall/open patches can be represented.

## Block 4, Mandatory index and connectivity audit

Objective:

Prevent lost or invalid indices.

Deliverables:

- MeshAuditReport,
- MeshIndexAudit2D,
- MeshConnectivityAudit2D,
- basic MeshGeometryAudit2D,
- audit error codes,
- test cases that intentionally fail.

Exit criteria:

- valid meshes pass,
- manipulated invalid meshes fail,
- export is blocked when fatal audit errors exist.

## Block 5, Text audit writer

Objective:

Produce human-readable column-aligned audit files.

Deliverables:

- TextMeshAuditWriter2D,
- header summary,
- one cell per block,
- one face per row,
- clockwise face order,
- status/anomaly column.

Exit criteria:

- output is readable in terminal,
- output is stable under repeated runs,
- diff-based regression tests are possible.

## Block 6, MohidNG HDF5 writer 2D

Objective:

Write the mesh package expected by MohidNG.

Deliverables:

- MohidNgMeshPackageWriter2D,
- metadata,
- nodes,
- cells,
- faces,
- boundary_patches,
- audit-before-export rule,
- integration test with MohidNG reader.

Exit criteria:

- VMM writes HDF5,
- MohidNG reads it,
- MohidNG validation passes.

## Block 7, Domains with holes and islands

Objective:

Support islands and internal no-flow regions.

Deliverables:

- real clipping by holes,
- no active cells in holes,
- boundary faces around holes,
- island/wall/no_flux patches,
- optional boundary site sampling near islands.

Exit criteria:

- domain with island produces cells only in fluid region,
- island boundary appears as internal boundary patch,
- boundary faces around island have invalid neighbour and valid patch.

## Block 8, Boundary projections and internal face anomalies

Objective:

Detect geometric anomalies relevant to finite-volume operators.

Deliverables:

- BoundaryProjectionTable2D,
- BoundaryProjectionAudit2D,
- InternalFaceOrthogonalityAudit2D,
- boundary anomalous cell list,
- internal anomalous face list,
- text audit integration.

Exit criteria:

- VMM detects boundary projections outside cell boundary face,
- VMM detects seed midpoint outside internal face,
- anomalies are visible in audit report.

## Block 9, Solver-neutral stencil graph

Objective:

Provide compact connectivity for solver backends without depending on them.

Deliverables:

- FiniteVolumeStencilGraph2D,
- BoundaryStencilTable2D,
- StencilGraphBuilder2D,
- stencil audit.

Exit criteria:

- graph is consistent with FaceTable,
- internal faces appear in both directions,
- boundary faces appear in boundary table,
- MohidNG can adapt graph to PETSc or Trilinos.

## Block 10, Algorithm architecture

Objective:

Allow multiple algorithms without closed switches in core.

Deliverables:

- TessellationAlgorithmConcepts,
- TessellationAlgorithmTraits,
- TessellationRegistry,
- RemeshingAlgorithmConcepts,
- RemeshingAlgorithmTraits,
- RemeshingRegistry,
- initial registration for CGAL clipped Voronoi 2D.

Exit criteria:

- algorithm can be selected by name,
- traits expose capabilities,
- incompatible options can be rejected,
- no polymorphism in hot loops.

## Block 11, GIS input layer

Objective:

Read real geospatial domains.

Deliverables:

- vmm::gis module,
- vector reader, GeoPackage or Shapefile first,
- CRS representation,
- PROJ transformation,
- conversion to GeoDomain2D,
- patch attributes from GIS.

Exit criteria:

- GIS domain converts to metric GeoDomain2D,
- patches are preserved,
- invalid geometries are reported.

## Block 12, Raster bathymetry/topography

Objective:

Attach raster fields to the mesh.

Deliverables:

- GeoTIFF reader,
- raster interpolation to cells,
- bathymetry field,
- topography field,
- missing-value diagnostics.

Exit criteria:

- each cell can receive a depth/elevation value,
- missing data are reported,
- field can be exported diagnostically.

## Block 13, Remeshing by site editing

Objective:

Improve mesh quality and reduce anomalies.

Deliverables:

- SiteEditBatch2D,
- RemeshingPlan2D,
- BoundarySamplingRemesher2D,
- BoundaryProjectionAnomalyRemesher2D,
- InternalFaceAnomalyRemesher2D,
- SizeFieldRemesher2D initial version,
- rebuild and audit after edit.

Exit criteria:

- remeshing changes site set,
- mesh is rebuilt globally,
- audits pass or report failures,
- anomalies decrease in controlled cases.

## Block 14, Extruded 3D mesh

Objective:

Create 3D meshes from stable 2D meshes.

Deliverables:

- NodeTable3D,
- CellTable3D,
- FaceTable3D,
- FiniteVolumeMesh3D,
- vertical layer specification,
- lateral faces,
- bottom faces,
- top faces,
- 3D audit,
- 3D text audit writer.

Exit criteria:

- simple 2D mesh extrudes to valid 3D mesh,
- volumes are positive,
- lateral connectivity is preserved,
- bottom/top faces are classified correctly.

## Block 15, MohidNG HDF5 writer 3D

Objective:

Export extruded 3D mesh packages.

Deliverables:

- MohidNgMeshPackageWriter3D,
- 3D nodes,
- 3D cells,
- 3D faces,
- 3D patches,
- 3D stencil graph optional,
- MohidNG reader test if available.

Exit criteria:

- MohidNG can read 3D package or a test reader validates it,
- audits pass.

## Block 16, Optional advanced backends

Objective:

Evaluate specialised backends without disturbing the core.

Candidates:

- Voro++ for 3D Voronoi cell-by-cell,
- JIGSAW for geophysical meshing and metric fields,
- CGAL regular triangulation for power diagrams.

Exit criteria:

- backend hidden behind existing algorithm interface,
- output converted to FiniteVolumeMesh,
- audit passes,
- no external types leak into public core.
