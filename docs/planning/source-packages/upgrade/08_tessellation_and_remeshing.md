# Tessellation and remeshing architecture

## Distinction

Tessellation creates a mesh from domain and sites.

Remeshing modifies site distribution or mesh resolution and rebuilds the mesh.

Do not treat them as the same operation.

## Tessellation pipeline 2D

```text
GeoDomain2D
    -> SiteTable2D
    -> Delaunay triangulation
    -> Voronoi dual
    -> clipping by outer boundary and holes
    -> clipped cells
    -> face reconstruction
    -> FiniteVolumeMesh2D
```

## Tessellation algorithms

Initial algorithm:

```text
cgal_clipped_voronoi_2d
```

Future algorithms:

```text
cgal_incremental_voronoi_2d
constraint_aware_clipped_voronoi_2d
regular_power_diagram_2d
jigsaw_voronoi_2d
extruded_voronoi_3d
voro_plus_plus_3d
```

## Concepts

```cpp
template<class Algorithm>
concept TessellationAlgorithm2D =
    requires(
        Algorithm algorithm,
        GeoDomain2D const& domain,
        SiteTable2D const& sites,
        TessellationOptions const& options
    ) {
        {
            algorithm.build(domain, sites, options)
        } -> std::same_as<FiniteVolumeMesh2D>;
    };
```

## Traits

```cpp
template<class Algorithm>
struct TessellationAlgorithmTraits;

template<>
struct TessellationAlgorithmTraits<CgalClippedVoronoi2D> {
    static constexpr std::string_view name =
        "cgal_clipped_voronoi_2d";

    static constexpr bool supports_holes = true;
    static constexpr bool supports_internal_constraints = false;
    static constexpr bool supports_incremental_update = false;
    static constexpr bool supports_boundary_projections = true;
    static constexpr bool produces_finite_volume_mesh = true;
};
```

## Registry

The registry is used for runtime selection only.

```cpp
auto tessellator =
    TessellationRegistry2D::instance().create(
        "cgal_clipped_voronoi_2d"
    );
```

The selected algorithm may use type erasure or runtime polymorphism at the boundary.

Do not use runtime polymorphism inside the per-cell or per-face loops.

## Remeshing principle

Remeshing shall edit sites, not final cells.

Pipeline:

```text
FiniteVolumeMesh2D
    -> MeshAudit2D
    -> RemeshingAlgorithm2D
    -> RemeshingPlan2D
    -> SiteEditBatch2D
    -> rebuild tessellation
    -> audit again
```

## Site edit batch

```cpp
struct SiteEditBatch2D {
    std::vector<SiteId> sites_to_remove;
    std::vector<Point2D> sites_to_insert;
    std::vector<std::pair<SiteId, Point2D>> sites_to_move;
};
```

## Remeshing plan

```cpp
struct RemeshingPlan2D {
    SiteEditBatch2D site_edits;
    std::vector<CellId> affected_cells;
    std::vector<FaceId> affected_faces;
    std::string reason;
};
```

## First remeshing strategy

For the first robust version:

- apply site edits,
- rebuild the full finite-volume mesh,
- audit the result.

Do not implement local face reconstruction first.

## Remeshing algorithms

Possible algorithms:

```text
boundary_sampling_remesher_2d
boundary_projection_anomaly_remesher_2d
internal_face_anomaly_remesher_2d
size_field_remesher_2d
bathymetry_gradient_remesher_2d
coarsening_remesher_2d
```

## Constrained-like behaviour

For islands and no-flow regions, use holes and patches.

Do not use constrained Delaunay as direct Voronoi core.

Constrained triangulations may be auxiliary tools for:

- validating GIS geometry,
- preparing constraints,
- sampling boundaries,
- inserting sites near features.

The final finite-volume mesh should be built from clipped Voronoi cells and explicit face reconstruction.
