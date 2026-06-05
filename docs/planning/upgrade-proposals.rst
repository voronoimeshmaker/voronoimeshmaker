Upgrade Proposals
=================

Source packages
---------------

The original proposal documents from the Codex packages are stored in the
repository for traceability:

* ``docs/planning/source-packages/upgrade``
* ``docs/planning/source-packages/visual-identity``

This page consolidates the proposals that affect architecture, implementation,
documentation, and visual identity.

Project role
------------

VoronoiMeshMaker is a mesh generation and validation tool, not a solver. Its
responsibility is to produce geometry, finite-volume topology, boundary patches,
audit data, and solver-neutral connectivity. MohidNG or another consumer is
responsible for physical operators and solver backend adaptation.

The VMM core must not depend on PETSc, Trilinos, Eigen, Ginkgo, or any other
linear algebra backend. Solver-facing data should be expressed through neutral
mesh tables and compact stencil arrays.

Core architecture
-----------------

The proposed architecture is layered around VMM-owned types:

* ``vmm::geometry`` for points, vectors, segments, polygons, bounding boxes, and CRS descriptors.
* ``vmm::gis`` for GIS reading and CRS conversion, with GDAL/OGR/PROJ hidden from public APIs.
* ``vmm::sites`` for site tables, site generation, and site edit batches.
* ``vmm::tessellation`` for Voronoi/Delaunay algorithms selected through concepts, traits, and registries.
* ``vmm::mesh`` for the canonical finite-volume mesh tables.
* ``vmm::audit`` for mandatory index, connectivity, geometry, patch, projection, and remap audits.
* ``vmm::remeshing`` for remeshing through site edits and full mesh rebuilds.
* ``vmm::stencil`` for solver-neutral finite-volume stencil graphs.
* ``vmm::io`` for text audit, VTK/VTU diagnostics, and MohidNG HDF5 export.
* ``vmm::backend`` for isolated CGAL, GDAL, PROJ, GEOS, Voro++, or other backend code.

External backend types must not appear in public core headers. CGAL, GDAL/OGR,
PROJ, GEOS, HDF5, JIGSAW, and Voro++ are implementation details at module
boundaries.

Data-oriented design policy
---------------------------

The finite-volume core should use contiguous arrays, Structure of Arrays,
CSR-style offsets, ``std::span`` views, preallocation, and batch operations.

Avoid per-cell object graphs, pointer networks, ``std::vector`` inside each cell,
runtime polymorphism per mesh entity, and dynamic allocation per face or cell in
hot paths.

Canonical finite-volume mesh
----------------------------

The canonical 2D finite-volume representation is face-based and should include:

* strong IDs: ``CellId``, ``FaceId``, ``NodeId``, ``SiteId``, ``BoundaryPatchId``;
* ``NodeTable2D`` with contiguous coordinate arrays;
* ``CellTable2D`` with centres, areas, site IDs, face offsets, and face counts;
* ``FaceTable2D`` with owner, neighbour, centre, unit normal, length, patch ID, and endpoint node IDs;
* ``BoundaryPatchTable`` with patch IDs, names, and patch types;
* ``CellFaceConnectivity`` with a flat face-ID array addressed by cell offsets;
* ``FiniteVolumeMesh2D`` as the aggregate of those tables.

Invalid neighbour IDs represent boundary faces. Invalid patch IDs represent
internal faces. Boundary faces must have a valid patch.

Boundary and GIS proposals
--------------------------

The 2D domain model must support outer boundaries, holes, islands, obstacles,
excluded regions, open boundaries, walls, and patch attributes.

The fluid domain is the outer boundary minus holes. Islands and no-flow regions
should be represented as holes in the active fluid domain, not as active cells
with artificial zero velocity.

GIS support should be progressive. GeoPackage is the preferred first vector
format if GDAL is available, with Shapefile as fallback. GeoJSON, GeoTIFF,
bathymetry rasters, topography rasters, refinement regions, constraint lines,
and patch attributes come later.

All lengths, areas, distances, normals, and volumes should be computed in a
metric working CRS unless an explicit geodetic mode is introduced.

Tessellation and remeshing proposals
------------------------------------

The first robust 2D tessellation path should be:

``sites -> Delaunay triangulation -> Voronoi dual -> clipping -> clipped cells -> face reconstruction``.

The architecture should allow multiple tessellation algorithms through strategy,
registry/factory, concepts, and traits. Runtime selection belongs at algorithm
boundaries, not inside per-cell or per-face loops.

Remeshing should happen by editing sites, not by mutating final cells directly.
Initial operations include insert, remove, move, and batch site edits. After site
edits, VMM rebuilds the finite-volume mesh and reruns mandatory audits.

Audit and validation proposals
------------------------------

Auditing is mandatory before exporting any mesh as valid. The proposed audit
families are:

* ``MeshIndexAudit`` for ID ranges and references;
* ``MeshConnectivityAudit`` for owner/neighbour/cell-face consistency;
* ``MeshGeometryAudit`` for positive measures and normals;
* ``PatchAudit`` for boundary patch consistency;
* ``BoundaryProjectionAudit`` for physical-boundary projection anomalies;
* ``InternalFaceOrthogonalityAudit`` for seed-midpoint/internal-face anomalies;
* ``MeshRemapAudit`` for compaction or remeshing maps.

Fatal audit errors must block HDF5 export.

Text audit report proposal
--------------------------

VMM should emit a stable, column-aligned text audit report suitable for human
inspection and diff-based regression tests.

For 2D, each cell block should include area, number of faces, local face order,
face ID, neighbour, centre distance, normal distance, face length, patch ID,
patch type, and status. Faces should be printed clockwise by face-centre angle
around the cell centre.

Solver-neutral stencil proposal
-------------------------------

VMM should produce solver-neutral finite-volume connectivity, with compact arrays
such as ``row_ptr``, ``col_idx``, ``face_id``, ``centre_distance``,
``normal_distance``, and ``face_measure``. In 2D, ``face_measure`` is length; in
3D, it is area.

Boundary stencil data should be represented separately from internal sparse
connectivity. PETSc and Trilinos adaptation belongs in consuming solvers, not in
VMM core.

MohidNG HDF5 proposal
---------------------

The MohidNG-oriented HDF5 package should include metadata plus canonical groups:

* ``/nodes/id`` and ``/nodes/xy``;
* ``/cells/id``, ``/cells/centre``, and ``/cells/area``;
* ``/faces/id``, ``/faces/owner``, ``/faces/neighbour``, ``/faces/centre``, ``/faces/unit_normal``, ``/faces/length``, and ``/faces/patch_id``;
* ``/boundary_patches/id``, ``/boundary_patches/name``, and ``/boundary_patches/type``.

Optional VMM-specific groups may store audit summaries, anomalies, and stencil
connectivity.

3D proposal
-----------

The first 3D target should be vertical extrusion of stable 2D finite-volume
meshes. General 3D Voronoi/polyhedral support is explicitly later work.

Extruded 3D should introduce 3D node, cell, face, volume, area, face role, and
vertical connectivity tables. Face ordering in text audit should use lateral
faces clockwise, then bottom, then top.

Visual identity proposal
------------------------

The approved visual direction is minimalist, technical, and geometric, using a
cracked-earth/Voronoi circular motif and a stacked mark:

``Voronoi``
``Mesh Maker``

Approved colours:

* Deep Slate: ``#263640``;
* Off White: ``#F5F7F8``;
* Steel Blue: ``#4C6A7F``;
* Cool Graphite: ``#3A434B``;
* Teal Accent: ``#3FA7A0``.

Use the visual identity for documentation, README assets, Sphinx styling,
landing-page material, GitHub presentation material, and slides. It is not part
of the numerical core.
