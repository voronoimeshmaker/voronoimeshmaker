# Codex implementation tasks

## Immediate task set, start here

### Task 1, Add strong ID types

Create:

```text
include/vmm/core/StrongIds.hpp
```

Types:

```cpp
CellId
FaceId
NodeId
SiteId
BoundaryPatchId
```

Provide:

- invalid value,
- comparison,
- explicit construction,
- conversion only through `.value`,
- helper `is_valid`.

Do not use implicit conversion to integer.

### Task 2, Add DOD mesh tables

Create:

```text
include/vmm/mesh/NodeTable2D.hpp
include/vmm/mesh/CellTable2D.hpp
include/vmm/mesh/FaceTable2D.hpp
include/vmm/mesh/BoundaryPatchTable.hpp
include/vmm/mesh/CellFaceConnectivity.hpp
include/vmm/mesh/FiniteVolumeMesh2D.hpp
```

Keep tables simple and contiguous.

### Task 3, Add basic mesh invariants

Add member functions or free functions for:

- number of nodes,
- number of cells,
- number of faces,
- valid id range checks,
- spans/views for cell faces.

### Task 4, Implement MeshIndexAudit2D

Create:

```text
include/vmm/audit/MeshAuditReport.hpp
include/vmm/audit/MeshIndexAudit2D.hpp
src/audit/MeshIndexAudit2D.cpp
```

Audit:

- all face owners valid,
- neighbours valid or invalid as boundary,
- patch ids valid when boundary,
- cell face offsets inside range,
- face ids in cell-face table valid,
- face node ids valid.

### Task 5, Implement MeshConnectivityAudit2D

Audit:

- internal face appears in both owner and neighbour cell face lists,
- boundary face appears only in owner cell face list,
- neighbour reciprocity,
- no duplicated face id inside one cell,
- each active cell has at least three faces.

### Task 6, Implement simple text audit writer

Create:

```text
include/vmm/io/TextMeshAuditWriter2D.hpp
src/io/TextMeshAuditWriter2D.cpp
```

Output column-aligned format from `13_text_audit_format.md`.

### Task 7, Add a manual mesh unit test

Create a small square mesh manually in DOD tables.

Test:

- index audit passes,
- connectivity audit passes,
- text audit file is generated,
- intentional invalid owner fails,
- intentional invalid patch fails.

### Task 8, Face reconstruction from polygonal cells

After tables and audits exist, implement reconstruction from clipped polygonal cells.

Keep this separate:

```text
include/vmm/mesh/FaceReconstruction2D.hpp
src/mesh/FaceReconstruction2D.cpp
```

Input:

- cell polygons,
- cell ids,
- site ids,
- boundary patch information.

Output:

- FiniteVolumeMesh2D.

### Task 9, HDF5 writer for MohidNG

Only after the mesh audits pass.

Create:

```text
include/vmm/io/MohidNgMeshPackageWriter2D.hpp
src/io/MohidNgMeshPackageWriter2D.cpp
```

Write the groups listed in `12_mohidng_hdf5_contract.md`.

### Task 10, Integration test with MohidNG

Generate HDF5 from VMM and read with MohidNG reader.

If direct MohidNG test is not yet practical, create an internal reader test that verifies the same datasets and invariants.

## Coding rules

- Use C++20.
- Keep implementation simple first.
- Do not introduce PETSc or Trilinos.
- Do not expose CGAL in public mesh headers.
- Do not build per-cell object graphs.
- Prefer free functions or stateless builders for algorithms.
- Use clear namespaces.
- Add tests with every block.
