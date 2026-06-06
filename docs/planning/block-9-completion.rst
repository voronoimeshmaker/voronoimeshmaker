Block 9 Completion: Projection and Orthogonality Audits
=======================================================

Purpose
-------

Block 9 adds report-only mesh-quality diagnostics for projection and
orthogonality anomalies. These checks help identify finite-volume geometry that
may require special treatment downstream, but they do not change operators,
modify sites, merge faces, or remesh.

Implemented Artifacts
---------------------

* ``vmm::audit::MeshProjectionAudit2D`` runs the established connectivity,
  geometry, and patch audits before applying projection checks.
* Internal face midpoint checks report faces whose centres are far from the
  owner-neighbour centre midpoint.
* Internal non-orthogonality checks report faces whose normals are strongly
  misaligned with the owner-neighbour vector.
* Boundary projection checks report boundary faces whose face centres are
  tangentially shifted from the owner-normal projection.
* ``MeshProjectionAudit2DOptions`` exposes tolerances and the maximum allowed
  non-orthogonality angle without introducing closed enums.
* Tests cover clean meshes, each warning family, invalid option rejection, and
  the base-audit stop behaviour.

Issue Codes
-----------

The audit adds warning-level issues:

* ``VMM-PRJ-001``: internal face centre offset from the owner-neighbour
  midpoint;
* ``VMM-ORT-001``: internal face normal strongly non-orthogonal to the
  owner-neighbour vector;
* ``VMM-PRJ-002``: boundary face centre tangentially offset from the owner
  normal projection.

Design Decisions
----------------

All projection findings are warnings. They are intended for reporting, ranking,
and future operator decisions, not for rejecting otherwise valid mesh packages.
Fatal topology and basic geometry errors are still handled by the earlier
audits.

The audit stores findings in ``MeshAuditReport`` rather than a separate quality
format so text diagnostics, future web reports, and export gates can share the
same issue transport.

Exit Criteria
-------------

Block 9 is complete when:

* projection and orthogonality anomalies are reported without remeshing;
* default settings pass the existing clean canonical meshes;
* configurable thresholds can trigger midpoint, orthogonality, and boundary
  projection warnings;
* invalid canonical meshes stop before projection-only checks;
* API/status/planning documentation records the new audit layer;
* build, tests, Sphinx, and whitespace checks pass.
