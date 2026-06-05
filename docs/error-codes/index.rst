Error Codes
===========

The current error layer uses open identifiers rather than a closed numeric
catalogue.

Examples include:

``vmm.invalid_argument``
   Invalid user input.

``vmm.file_not_found``
   File path could not be opened.

``vmm.parse_error``
   Input text could not be parsed.

``vmm.domain.invalid_polygon``
   Polygon has fewer than three valid vertices or invalid coordinates.

``vmm.domain.degenerate_polygon``
   Polygon has zero area or repeated consecutive vertices.

``vmm.domain.self_intersecting_polygon``
   Polygon edges intersect each other.

Messages should carry British English and Brazilian Portuguese text.

Mesh audit codes
----------------

Mesh audit reports use open string identifiers. They are not closed enums.

Index audit
~~~~~~~~~~~

``VMM-IDX-000``
   A mesh table has inconsistent column sizes.

``VMM-IDX-002``
   Cell-face connectivity references an invalid face.

``VMM-IDX-003``
   A face references an invalid node.

``VMM-IDX-005``
   The patch table contains an invalid patch id.

``VMM-IDX-006``
   A face owner is invalid.

``VMM-IDX-007``
   A face neighbour is out of range.

``VMM-IDX-008``
   A boundary face has no valid patch.

``VMM-IDX-009``
   An internal face has a boundary patch.

``VMM-IDX-010``
   A cell face offset/count exceeds connectivity storage.

Connectivity audit
~~~~~~~~~~~~~~~~~~

``VMM-CON-001``
   An owner cell does not reference one of its owned faces.

``VMM-CON-002``
   A neighbour cell does not reference an internal face.

``VMM-CON-003``
   A cell face list contains a duplicate face.

``VMM-CON-004``
   A 2D cell has fewer than three faces.

``VMM-CON-005``
   A face occurrence count does not match its boundary/internal state.

Geometry audit
~~~~~~~~~~~~~~

``VMM-GEO-001``
   A cell centre is not finite.

``VMM-GEO-002``
   A cell area is not positive or not finite.

``VMM-GEO-003``
   A face centre is not finite.

``VMM-GEO-004``
   A face length is not positive or not finite.

``VMM-GEO-005``
   A face normal is not finite.

``VMM-GEO-006``
   A face normal is zero.

``VMM-GEO-007``
   A face normal does not point outward from the owner.
