# Audit and validation

## Audit philosophy

Auditing is not optional. Every mesh exported as valid must pass the required audits.

The VMM shall fail early when indices, connectivity or geometry are inconsistent.

## Audit types

```text
MeshIndexAudit
    validates ids and references.

MeshConnectivityAudit
    validates owner/neighbour and cell-face consistency.

MeshGeometryAudit
    validates positive areas, lengths, volumes, normals.

BoundaryProjectionAudit
    validates seed projections onto physical boundaries.

InternalFaceOrthogonalityAudit
    validates midpoint between seeds relative to internal face.

MeshRemapAudit
    validates old-to-new entity maps after remeshing or compaction.
```

## MeshAuditReport

```cpp
enum class MeshAuditSeverity {
    info,
    warning,
    error,
    fatal
};

struct MeshAuditIssue {
    MeshAuditSeverity severity;
    std::string code;
    std::string message;

    CellId cell_id;
    FaceId face_id;
    NodeId node_id;
    SiteId site_id;
    BoundaryPatchId patch_id;
};

struct MeshAuditReport {
    bool valid;
    std::vector<MeshAuditIssue> issues;
};
```

## Suggested error codes

```text
VMM-IDX-001  CellId out of range
VMM-IDX-002  FaceId out of range
VMM-IDX-003  NodeId out of range
VMM-IDX-004  SiteId out of range
VMM-IDX-005  PatchId out of range
VMM-IDX-006  Face owner is invalid
VMM-IDX-007  Face neighbour is invalid
VMM-IDX-008  Boundary face has no valid patch
VMM-IDX-009  Internal face has boundary patch
VMM-CON-001  Non-reciprocal neighbour relation
VMM-CON-002  Cell references missing face
VMM-CON-003  Face references missing node
VMM-CON-004  Duplicate internal face ownership
VMM-GEO-001  Non-positive cell area
VMM-GEO-002  Non-positive face length
VMM-GEO-003  Non-unit normal
VMM-GEO-004  Face centre outside expected segment
VMM-PROJ-001 Boundary projection outside boundary face
VMM-PROJ-002 Boundary projection ambiguous
VMM-ORTH-001 Seed midpoint outside internal face
VMM-REM-001  Lost cell during remap
VMM-REM-002  Patch disappeared after remeshing
```

## Boundary projection anomaly

For each boundary cell and physical patch:

1. Get the cell seed.
2. Find the closest point on the physical boundary patch.
3. Store projected point and distance.
4. Check whether projected point belongs to one of the cell boundary faces for that patch.
5. If not, mark as anomalous.

## Internal face anomaly

For each internal face between owner i and neighbour j:

1. Get seed_i and seed_j.
2. Compute midpoint.
3. Test whether midpoint belongs to the actual face.
4. If not, mark face as anomalous.

## Remap audit

Every compaction or remeshing operation should produce maps:

```cpp
struct MeshRemap2D {
    std::vector<CellId> old_to_new_cell;
    std::vector<FaceId> old_to_new_face;
    std::vector<NodeId> old_to_new_node;
    std::vector<SiteId> old_to_new_site;

    std::vector<CellId> removed_cells;
    std::vector<FaceId> removed_faces;
    std::vector<NodeId> removed_nodes;

    std::vector<CellId> created_cells;
    std::vector<FaceId> created_faces;
    std::vector<NodeId> created_nodes;
};
```

Audit must verify that no entity was lost unless explicitly removed.
