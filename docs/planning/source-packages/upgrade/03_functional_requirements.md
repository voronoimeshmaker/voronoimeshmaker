# Functional requirements

## FR-001, Domain representation 2D

The VMM shall represent 2D domains with:

- outer boundary,
- internal holes,
- islands,
- obstacles,
- excluded regions,
- open boundaries,
- wall boundaries,
- patch attributes.

Islands and zero-flow regions shall be represented as holes in the fluid domain, not as active cells with artificial zero velocity.

## FR-002, Domains with holes

The computational domain shall be:

fluid domain = outer boundary minus holes

The VMM shall generate active cells only in the fluid domain.

The boundaries of holes shall be exported as internal boundary patches, for example `island`, `wall` or `no_flux`.

## FR-003, GIS input

The VMM shall provide a GIS layer able to progressively read:

- GeoPackage,
- Shapefile,
- GeoJSON,
- GeoTIFF,
- bathymetry rasters,
- topography rasters,
- boundary attributes,
- refinement regions,
- constraint lines.

The public core API shall not expose GDAL, OGR, GEOS or PROJ types.

## FR-004, CRS transformation

The VMM shall support a source CRS and a metric working CRS.

Lengths, areas, distances, normals and volumes shall be computed in a metric working CRS, unless an explicit geodetic mode is implemented.

## FR-005, Site generation

The VMM shall support multiple site-generation strategies:

- uniform sites,
- hexagonal sites,
- boundary-sampled sites,
- island boundary sites,
- user-provided sites,
- size-field-driven sites,
- bathymetry/topography-driven sites.

Sites inside holes or excluded regions shall not be active.

## FR-006, Tessellation 2D

The VMM shall generate 2D Voronoi tessellations from valid sites.

The first robust algorithm shall use:

- Delaunay triangulation of sites,
- Voronoi dual,
- clipping by the physical domain,
- reconstruction of clipped cells.

The architecture shall allow multiple tessellation algorithms.

## FR-007, Explicit finite-volume mesh 2D

The VMM shall convert clipped Voronoi cells into an explicit finite-volume mesh containing:

- nodes,
- cells,
- faces,
- owner,
- neighbour,
- cell centre,
- cell area,
- face centre,
- face unit normal,
- face length,
- patch id,
- patch type.

## FR-008, Boundary patches

The VMM shall store boundary patches in a dedicated table.

Minimum patch data:

- patch_id,
- name,
- type,
- optional description,
- optional GIS source layer or feature id.

Minimum patch types:

- internal,
- wall,
- island,
- inlet,
- outlet,
- open_boundary,
- land_boundary,
- no_flux,
- bottom,
- surface.

## FR-009, Index audit

The VMM shall audit indices after every operation that can change mesh topology or compact storage.

The audit shall verify:

- valid CellId,
- valid FaceId,
- valid NodeId,
- valid SiteId,
- valid BoundaryPatchId,
- valid owner for all faces,
- valid neighbour for internal faces,
- invalid neighbour for boundary faces,
- valid patch for boundary faces,
- invalid patch for internal faces,
- valid cell-face references,
- valid face-node references,
- contiguous ids before HDF5 export,
- no lost entities after remeshing or compaction unless explicitly removed.

A mesh with index errors shall not be exported as valid.

## FR-010, Connectivity audit

The VMM shall audit:

- reciprocal neighbour relations,
- correct owner/neighbour use,
- internal faces shared correctly,
- boundary faces without neighbour,
- minimum number of faces per cell,
- positive cell areas in 2D,
- positive face lengths in 2D,
- positive cell volumes in 3D,
- positive face areas in 3D.

## FR-011, Internal face orthogonality anomaly

For each internal face between cells i and j, the VMM shall compute:

m_ij = 0.5 * (seed_i + seed_j)

The VMM shall test whether this midpoint belongs to the actual shared face.

If the midpoint does not belong to the face, the face shall be marked as anomalous.

## FR-012, Boundary orthogonal projection anomaly

For each boundary cell, the VMM shall compute the orthogonal projection of the cell seed onto the physical boundary patch.

The VMM shall store:

- cell_id,
- site_id,
- patch_id,
- boundary segment or surface element id,
- projected point,
- distance,
- projection kind,
- projection status,
- whether the projected point belongs to the boundary face of that cell.

If the projected point does not belong to the boundary face, the cell shall be marked as a boundary projection anomaly.

## FR-013, Text audit report

The VMM shall export a human-readable column-aligned text audit report.

For 2D, per cell:

- CELL,
- AREA,
- NFACES,
- local face index,
- face_id,
- neighbour,
- distance,
- length,
- patch_id,
- patch_type,
- anomaly/status.

Faces shall be printed clockwise.

For extruded 3D, per cell:

- CELL,
- VOLUME,
- NFACES,
- local face index,
- role,
- face_id,
- neighbour,
- distance,
- area,
- patch_id,
- patch_type,
- anomaly/status.

Face order in extruded 3D should be lateral faces clockwise, then bottom, then top.

## FR-014, Solver-neutral stencil graph

The VMM shall produce solver-neutral finite-volume stencil connectivity.

Minimum arrays:

- row_ptr,
- col_idx,
- face_id,
- centre_distance,
- normal_distance,
- face_measure.

In 2D, face_measure is face length.

In 3D, face_measure is face area.

The VMM shall not produce PETSc or Trilinos matrices in its core.

## FR-015, MohidNG HDF5 export

The VMM shall export a mesh package compatible with the MohidNG mesh package reader.

Minimum groups:

- nodes,
- cells,
- faces,
- boundary_patches,
- metadata.

The VMM shall run mandatory audits before export.

## FR-016, Visualisation export

The VMM shall export diagnostic meshes to VTK/VTU.

These exports are for visualisation and debugging. They do not replace the MohidNG HDF5 package.

## FR-017, Remeshing by site editing

The VMM shall support remeshing by editing sites, not by direct mutation of final cells.

Minimum operations:

- insert_site,
- remove_site,
- move_site,
- insert_site_batch,
- remove_site_batch,
- apply_site_edit_batch.

After remeshing, the VMM shall rebuild the finite-volume mesh and run mandatory audits.

## FR-018, Multiple algorithms

The VMM shall support multiple algorithms for:

- site generation,
- tessellation,
- remeshing,
- audit,
- export.

The architecture shall use:

- Strategy for algorithm behaviour,
- Registry/Factory for runtime selection,
- Concepts for compile-time contracts,
- Traits for algorithm capabilities.

## FR-019, Extruded 3D mesh

The first 3D support shall be vertical extrusion of 2D finite-volume meshes.

The VMM shall generate:

- 3D cells,
- lateral faces,
- bottom faces,
- top or surface faces,
- owner,
- neighbour,
- face areas,
- cell volumes,
- 3D patches,
- vertical connectivity.

General polyhedral 3D Voronoi shall be considered later.

## FR-020, Metadata and traceability

The VMM shall store metadata such as:

- VMM version,
- tessellation algorithm,
- remeshing algorithm,
- source CRS,
- working CRS,
- GIS source,
- generation date,
- generation options,
- number of cells,
- number of faces,
- number of nodes,
- number of patches,
- number of anomalies.
