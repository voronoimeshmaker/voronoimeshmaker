Extensibility Policy
====================

Purpose
-------

VMM is expected to evolve through GIS workflows, boundary-condition metadata,
mesh-audit severities, export formats and optional backends. These categories
should not be represented as closed enums when new capabilities would require
editing unrelated switch statements, format writers or registries.

Policy
------

Use traits, identifiers, tags, entity sets or factories for categories that can
grow with user workflows. Closed enums are allowed only when the set is
structural and deliberately small, such as the compile-time 2D/3D dimension
selector used by template specialisations.

Current decisions
-----------------

``LogLevel``
   Implemented as a lightweight value plus ``LogLevelTraits``.

``BoundaryPatchType``
   Implemented as a lightweight value plus ``BoundaryPatchTypeTraits``.

``BoundaryType``
   Implemented as a lightweight value plus ``BoundaryTypeTraits`` while legacy
   cell-centred topology is kept alive.

``MeshAuditSeverity``
   Implemented as a lightweight value plus ``MeshAuditSeverityTraits`` so audit
   modules can introduce new severity families without changing a central enum.

``Dimension``
   Kept as a closed enum because it selects compile-time specialisations and is
   not an extensible functional category.
