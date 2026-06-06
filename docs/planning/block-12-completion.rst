Block 12 Completion
===================

Scope
-----

Block 12 introduces remeshing through site edits. It follows the project policy
that remeshing should edit generator sites, rebuild the canonical mesh, and
rerun mandatory audits instead of mutating final finite-volume cells directly.

Implemented
-----------

* ``vmm::remeshing::SiteEditBatch2D`` records sequential insert, remove, and
  move operations on generator sites.
* ``vmm::remeshing::SiteEditKind`` uses traits rather than a closed enum.
* ``vmm::remeshing::apply_site_edit_batch_2d`` applies edits to an existing site
  vector and validates finite coordinates, domain containment, minimum spacing,
  and minimum site count.
* ``vmm::remeshing::remesh_complete_finite_volume_voronoi_mesh_2d`` applies a
  site edit batch, calls the integrated 2D workflow, rebuilds the canonical FV
  mesh, and reruns the workflow audits.
* ``vmm::remeshing::SiteRemeshingRegistry2D`` keeps remeshing method selection
  open. The implemented method is registered as ``site-edit-rebuild``; later
  methods can be registered without changing call sites.
* ``vmm::mesh::finite_volume_matrix_half_bandwidth_2d`` and
  ``vmm::mesh::finite_volume_matrix_bandwidth_2d`` report the sparse matrix
  bandwidth implied by the internal FV stencil graph.
* Unit tests cover insert/move/remove batches, invalid site references,
  out-of-domain edits, spacing violations, method registry dispatch, matrix
  bandwidth, and full mesh reconstruction after edits.

Deliberate limits
-----------------

* Edits are applied sequentially to the current site vector, so remove and move
  IDs refer to the site state at that point in the batch.
* This block does not add automatic quality-driven site placement. It provides
  the explicit edit-and-rebuild machinery that later quality policies can use.
