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

Extension policy
----------------

Mesh metadata that can grow with GIS, export formats, boundary-condition
families, or optional backends should use traits, tags, identifiers, or
registries rather than closed enums.

Next direction
--------------

The mesh layer now has canonical face tables, text diagnostics, cell-to-face
reconstruction, and explicit boundary patch assignment. The next layer is GIS
or format-specific import/export around those stable tables rather than another
table redesign.
