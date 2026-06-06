Mesh API
========

Canonical finite-volume mesh
----------------------------

``vmm::mesh::FiniteVolumeMesh2D`` stores the current canonical 2D finite-volume
mesh contract: node table, cell table, face table, boundary patch table, and
flat cell-face connectivity.

``vmm::mesh::FiniteVolumeMeshBuilder2D`` reconstructs the canonical mesh from
planar 2D cells by deduplicating nodes, detecting shared polygon edges, creating
global faces, assigning owners/neighbours, classifying boundary faces against
optional patch rings, and filling cell-face connectivity.

``vmm::core::MeshTopology<Dim>``
   Legacy cell-centred structure-of-arrays topology container with CSR
   neighbour indexing.

``vmm::mesh::FiniteVolumeStencilGraph2D``
   Solver-neutral compact graph derived from the canonical face table. It stores
   internal owner-neighbour entries and boundary cell-patch entries without
   encoding any solver-specific coefficient policy. The helper
   ``vmm::mesh::finite_volume_matrix_bandwidth_2d`` reports the matrix bandwidth
   implied by the internal stencil graph.

Current legacy topology data
----------------------------

The legacy topology stores:

* cell centroids;
* generator identifiers;
* neighbour offsets and neighbour ids;
* boundary flags.

Audit APIs
----------

``vmm::audit::MeshIndexAudit2D``
   Validates table storage, ID ranges, owners, neighbours, patches, cell-face
   offsets, face IDs, and node IDs.

``vmm::audit::MeshConnectivityGeometryAudit2D``
   Runs the index audit first, then validates face membership, internal face
   reciprocity, duplicate cell-face entries, face occurrence counts, minimum
   2D face counts, positive areas and lengths, finite coordinates, finite
   normals, non-zero normals, and basic owner-normal orientation.

``vmm::audit::MeshPatchAudit2D``
   Runs the index audit first, then validates patch row identity, non-empty
   patch metadata, and boundary faces that reference non-internal patch types.

``vmm::audit::MeshProjectionAudit2D``
   Runs the established mesh audits first, then reports projection and
   orthogonality warnings for internal midpoint offsets, strongly non-orthogonal
   internal faces, and tangentially shifted boundary projections.

MohidNG HDF5 export
-------------------

``vmm::io::write_mohidng_hdf5_mesh_2d``
   Runs the mandatory mesh audits, then writes the MohidNG Voronoi mesh package
   schema ``mohidng.voronoi_mesh_package/0.1`` with nodes, cells, faces,
   boundary patches, root metadata, and no public HDF5 types in the VMM API.

GIS vector input
----------------

``vmm::gis::read_gis_polygonal_domain_2d``
   Reads GIS vector polygon sources into VMM-owned ``PolygonalDomain2D``,
   boundary patch rings, and CRS metadata. GDAL/OGR and PROJ are optional
   implementation backends: their types do not appear in public headers, and
   unsupported builds fail explicitly at runtime.

Raster bathymetry/topography
----------------------------

``vmm::raster::RasterScalarField2D``
   Stores regular 2D scalar rasters with grid-centre geometry, CRS metadata,
   no-data handling, and nearest-neighbour or bilinear sampling.

``vmm::raster::sample_cell_centres``
   Samples a raster field at canonical finite-volume cell centres and returns
   compact value and validity arrays suitable for bathymetry/topography
   diagnostics or later export workflows.

``vmm::raster::read_gis_raster_scalar_field_2d``
   Reads GDAL-supported raster sources when the optional backend is available.
   Builds without GDAL keep the API available and report unsupported operation
   explicitly at runtime.

Integrated 2D workflow
----------------------

``vmm::workflow::generate_complete_finite_volume_voronoi_mesh_2d``
   Runs the official 2D generation sequence from sites and a polygonal domain
   to clipped Voronoi cells, canonical finite-volume tables, mandatory audit
   reports, optional stencil graph, and optional cell-centre raster samples.
   This is the preferred public entry point for other projects that need a
   complete 2D VMM mesh rather than individual construction steps.

Site-edit remeshing
-------------------

``vmm::remeshing::SiteEditBatch2D``
   Records sequential generator-site edits. Insert, remove, and move operations
   are expressed through open traits rather than a closed enum.

``vmm::remeshing::remesh_complete_finite_volume_voronoi_mesh_2d``
   Applies a site edit batch, validates the resulting site set, rebuilds the
   complete 2D finite-volume Voronoi mesh through the integrated workflow, and
   reruns the mandatory audit chain.

``vmm::remeshing::SiteRemeshingRegistry2D``
   Registers remeshing methods by identifier. The default registry currently
   exposes ``site-edit-rebuild`` and can accept future remeshing algorithms
   without replacing the public workflow.

Extruded 3D FV mesh
-------------------

``vmm::mesh::extrude_finite_volume_mesh_3d``
   Builds the first supported 3D mesh family by extruding a valid 2D
   finite-volume mesh across vertical interfaces. The resulting
   ``FiniteVolumeMesh3D`` contains 3D node, cell, face, boundary patch,
   cell-face, and face-node tables.

``vmm::audit::audit_extruded_mesh_3d``
   Runs basic storage, volume, area, reference, patch, and connectivity checks
   for extruded 3D meshes.

Extension policy
----------------

Mesh metadata that can grow with GIS, export formats, boundary-condition
families, or optional backends should use traits, tags, identifiers, or
registries rather than closed enums.

Next direction
--------------

The mesh layer now has canonical face tables, text diagnostics, cell-to-face
reconstruction, explicit boundary patch assignment, MohidNG HDF5 export, and a
solver-neutral stencil graph. Projection and orthogonality anomaly audits now
report mesh-quality risks without remeshing automatically. GIS vector input,
raster sampling, the integrated 2D workflow, site-edit remeshing, and vertical
3D extrusion now have isolated public APIs. The next layers should extend HDF5
3D export and richer import validation around those stable tables rather than
another table redesign.
