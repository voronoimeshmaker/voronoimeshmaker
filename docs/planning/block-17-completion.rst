Block 17 Completion: Placeholder Cleanup and Source Audit
==========================================================

Goal
----

Block 17 audits all ``.cpp``, ``.h``, and ``.hpp`` files after the late-stage
restructuring work. The intent is to remove files that were useful as an early
roadmap skeleton but no longer represent real API, implementation, or tests.

Findings
--------

The empty files were introduced as planning placeholders in the initial
repository skeleton and upgrade packages. They reserved names for generic mesh
APIs, backend layers, configuration parsing, shortcut APIs, site patterns, and
future tests before the canonical finite-volume tables and workflow APIs were
stable.

After the implemented blocks, many of those names became misleading:

* generic mesh placeholders were superseded by canonical FV tables and audits;
* shortcut and config placeholders had no callers or public contract;
* empty backend files were not contributing symbols;
* empty tests created targets without coverage;
* unsupported site-pattern placeholders advertised features that were not
  available.

Actions
-------

* Removed empty public headers with no active references.
* Removed placeholder-only headers and sources.
* Removed empty test files and the corresponding no-op test targets.
* Removed the unsupported BCC 3D example because no BCC 3D pattern is exposed.
* Replaced placeholder examples for box, cartesian, hexagonal, and radial
  generation with examples that exercise real public APIs.
* Replaced the placeholder 3D transform test with real point and surface
  transform tests.
* Confirmed that no empty ``.cpp``, ``.h``, or ``.hpp`` files remain under
  ``VMMLib``, ``tests``, or ``examples``.

Site pattern consolidation
--------------------------

The supported built-in 2D site patterns are now:

* ``hexagonal_2d``;
* ``cartesian_2d``;
* ``radial_2d``;
* ``random_2d`` with deterministic seeding.

The default site factory exposes those names through identifiers rather than a
closed enum.

Validation
----------

The cleanup was validated by reconfiguring CMake from the updated file graph,
building the library, examples, and tests, and running the full test suite.
