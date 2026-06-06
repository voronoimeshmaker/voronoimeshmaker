Block 11.5 Completion
=====================

Scope
-----

Block 11.5 makes the existing 2D pieces usable as one official generation
workflow before remeshing work starts. It does not replace the lower-level
domain, site, tessellation, mesh, audit, stencil, raster, or export APIs.

Implemented
-----------

* ``vmm::workflow::generate_complete_finite_volume_voronoi_mesh_2d`` runs the
  official 2D sequence: sites, clipped Voronoi cells, canonical finite-volume
  mesh reconstruction, mandatory audits, optional stencil graph, and optional
  raster sampling.
* ``vmm::workflow::CompleteFiniteVolumeVoronoiMesh2D`` stores the generated
  clipped cells, canonical FV mesh, audit reports, optional stencil graph, and
  optional cell-centre raster samples.
* The workflow fails before returning invalid production meshes when
  ``require_valid_mesh`` is enabled.
* A complete example writes domain/cell VTK diagnostics, text audit output, and
  a HDF5 package from one workflow result.
* Unit tests cover the default workflow, raster sampling integration, and the
  optional stencil graph switch.

Use in later blocks
-------------------

Block 12 remeshing can now rebuild the complete 2D mesh through this workflow
after applying site edit batches. Solver and mesh-side experiments can call
the same public workflow instead of stitching internal layers manually.
