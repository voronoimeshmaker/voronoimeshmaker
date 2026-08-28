Experimental Mesh Protocol
==========================

This page records the mesh-generation requirements for controlled solver
experiments.

Implemented in the current code
-------------------------------

* Geometry and numbering can be separated: a canonical finite-volume mesh can
  be generated once and then compared through cell permutations.
* Cell permutations store both ``old_to_new`` and ``new_to_old`` mappings.
* Native and deterministic random orderings are available as comparison
  references.
* RCM, Hilbert, and Morton/Z-order cell renumbering are available.
* METIS nested dissection is represented as an optional-backend method and
  reports unsupported operation until that backend is enabled.
* Matrix half-bandwidth and full bandwidth can be measured from the
  solver-neutral stencil graph, with or without a candidate permutation.
* Random site generation already accepts an explicit deterministic seed.
* Lloyd/CVT relaxation can be controlled by exact iteration count, including
  ``L0`` through zero iterations, and records mean and maximum generator
  displacement for every executed iteration.
* The complete finite-volume workflow can run Lloyd/CVT before constructing
  the canonical mesh and still runs the mandatory mesh audit chain afterwards.

Still planned
-------------

* Mesh ID and mesh manifest output with seed, target count, actual count,
  version, commit, timings, numbering method, and geometry metrics.
* Built-in timing reports for generation, tessellation, Lloyd iteration,
  renumbering, and export.
* Geometry metrics for neighbours, area distribution, generator-centroid
  distance, neighbouring generator distances, and boundary-cell count.
* Direct adjacency graph export as neighbour lists or CSR arrays.
* Numbering profile metrics in addition to bandwidth.
* Optional coherent face ordering experiments after cell renumbering.
* Target cell-count helpers that make cartesian, hexagonal, and random families
  easier to compare at similar problem sizes.
