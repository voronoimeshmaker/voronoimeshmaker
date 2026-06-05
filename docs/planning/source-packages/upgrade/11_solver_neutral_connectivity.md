# Solver-neutral finite-volume connectivity

## Principle

The VMM shall not create PETSc or Trilinos matrices.

The VMM shall export a solver-neutral finite-volume stencil graph.

MohidNG or another consumer adapts the graph to PETSc, Trilinos, Eigen, Ginkgo or another backend.

## Canonical mesh first

The canonical source of truth is still the face table:

```text
faces:
    owner
    neighbour
    centre
    normal
    length or area
    patch_id
```

The stencil graph is a derived view.

## FiniteVolumeStencilGraph2D

```cpp
struct FiniteVolumeStencilGraph2D {
    std::vector<CellId> row_cell_ids;
    std::vector<std::uint64_t> row_ptr;

    std::vector<CellId> col_cell_ids;
    std::vector<FaceId> face_ids;

    std::vector<Real> centre_distances;
    std::vector<Real> normal_distances;
    std::vector<Real> face_measures;
};
```

For 2D, `face_measures` are face lengths.

For 3D, `face_measures` are face areas.

## BoundaryStencilTable2D

Boundary faces do not have neighbours. Store them separately.

```cpp
struct BoundaryStencilTable2D {
    std::vector<CellId> owner;
    std::vector<FaceId> face_id;
    std::vector<BoundaryPatchId> patch_id;

    std::vector<Real> boundary_distance;
    std::vector<Real> face_measure;
};
```

## Diagonal policy

The stencil graph should initially exclude diagonal entries.

Physical solvers can add diagonal entries during operator assembly.

Optional future policy:

```cpp
enum class DiagonalPolicy {
    excluded,
    included_first,
    included_sorted
};
```

## How to build the graph

1. Initialise row counts to zero.
2. For each internal face f:
    - i = owner(f),
    - j = neighbour(f),
    - add j to row i,
    - add i to row j.
3. Sort neighbours per row for deterministic output.
4. Build row_ptr and col arrays.
5. Add face_id and metrics aligned with each connection.
6. Boundary faces go to `BoundaryStencilTable`.

## Audit requirements

Verify:

- every col_cell_id exists,
- every face_id exists,
- face_id connects row cell and neighbour,
- internal faces appear in both directions,
- boundary faces do not appear as neighbours,
- boundary faces appear in boundary stencil table,
- distances are positive,
- face measures are positive.

## Backend adaptation

For PETSc:

```text
FiniteVolumeStencilGraph
    -> AIJ preallocation
    -> MatSetValues
```

For Trilinos:

```text
FiniteVolumeStencilGraph
    -> Tpetra::CrsGraph
    -> Tpetra::CrsMatrix
```

This adaptation belongs to MohidNG, not VMM core.
