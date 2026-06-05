Mesh Generator Survey
=====================

Purpose
-------

This survey records external mesh-generation systems that can inform VMM design
without expanding VMM's immediate feature scope. The goal is to identify more
efficient ways to organise extension points, audits, metadata and backend
boundaries.

Systems reviewed
----------------

Gmsh
   Reference for geometry kernels, mesh-size fields, callbacks, scripting APIs,
   format interoperability and background-mesh driven refinement.

CGAL Mesh packages
   Reference for robust predicates, Delaunay refinement and keeping geometric
   kernels isolated behind backend translation units.

Triangle
   Reference for constrained Delaunay triangulation, PSLG handling and robust
   2D predicates.

JIGSAW
   Reference for geophysical Delaunay and Voronoi workflows driven by spatially
   variable resolution functions.

MMG
   Reference for remeshing operations such as split, collapse, swap and vertex
   relocation. These ideas are useful for future site-edit/remeshing plans.

DistMesh
   Reference for clean separation between signed distance functions, size
   functions and mesh relaxation.

OpenFOAM snappyHexMesh
   Reference for staged mesh-generation pipelines with explicit quality gates
   and diagnostic outputs after each phase.

OceanMesh2D
   Reference for coastal/ocean domain preparation, DEM/bathymetry use, shoreline
   handling and unstructured workflows for environmental models.

MOAB, CGNS and meshio
   References for mesh metadata, tags, sets, boundary-condition families and
   conversion-oriented interoperability.

Design implications for VMM
---------------------------

Canonical mesh first
   VMM should preserve its own canonical finite-volume table model. Backends
   may generate geometry, but they must feed VMM-owned tables and audits.

Tags and traits over enums
   Boundary patches, audit severities, backend identifiers and export policies
   should be extensible values with stable names/codes.

Size-field abstraction
   Future GIS/raster/remeshing work should introduce a queryable size-field
   interface before adding algorithm-specific controls.

Audit stages
   Mesh construction should remain staged: index audit, connectivity audit,
   geometry audit, text diagnostics and only then package export.

Backend isolation
   CGAL, JIGSAW, Triangle or MMG concepts must not leak into public VMM headers.
   Optional backends should adapt into VMM's canonical mesh.

Immediate action
----------------

No new user-facing mesh-generation feature is required from this survey. The
near-term action is architectural: keep current blocks focused on the canonical
finite-volume mesh, audit reports, diagnostics and web documentation.
