# Requirements Specification

## 📘 Functional Requirements (RF)

| ID | Requirement | Description | Priority | Verification |
|----|-------------|-------------|----------|--------------|
| **RF-01** | Seed Generation | Generate seed points from Poisson disk, structured grids, external files, or scalar metric fields | MVP | Count and spatial distribution validated against input parameters |
| **RF-02** | Sequential IDs | Each generator receives a unique, deterministic `GeneratorID` (`uint64_t`) for identical seed inputs | MVP | Identical configuration → identical ID sequence across builds |
| **RF-03** | Tessellation | Generate 2D/3D Voronoi meshes maintaining explicit `GeneratorID ↔ Cell` mapping via dualisation | MVP | Duality verified; exactly one generator per cell; zero overlap |
| **RF-04** | Topological Classification | Tag internal versus boundary cells (external perimeter and internal holes). Expose dedicated views | MVP | `boundary_cells()` returns only cells with ≥1 physical boundary face |
| **RF-05** | Geometric Validation | Verify: volume > 0, topological consistency, skewness, aspect ratio, and numerical robustness of local orthogonality | MVP | Analytical test cases; orthogonality tolerance ≥ `1 - 1e-12`; zero non-positive volumes |
| **RF-06** | Connectivity Matrix | Export explicit CSR layout (`cell_id → [neighbor_ids]`, `boundary_flags`) derived from sequential IDs | MVP | Sparse matrix structurally valid; neighbour degree matches dimensionality |
| **RF-07** | Metric-Driven Remeshing | Perform split/coarsen/smoothing operations guided by scalar fields, preserving hole geometry and assigning continuous new IDs | Phase 2 | Target metric achieved; topological invariants maintained post-remesh |
| **RF-08** | Multi-Format Export | Support VTK/VTU, MSH v4, and UGRID/NetCDF with embedded contour tags and quality metadata | MVP | Files open without error in ParaView, Gmsh, or Python (`xarray`) |
| **RF-09** | Layered Interfaces | Provide CLI + YAML/TOML configuration, modern C++ API, and prepare for future Python/C bindings | MVP (CLI) | `vmm generate --config mesh.yaml` executes without C++ compilation |
| **RF-10** | Bilingual Errors | Messages in `en_GB` (default) and `pt_BR`, static error codes, structured context, and runtime locale switching | MVP | Locale tests pass; CI validates fallback; `--locale` and `VMM_LOCALE` functional |
| **RF-11** | Phase Instrumentation | Automatic timing of: seed generation, tessellation, validation, and export. Structured reporting to logger | MVP | `PhaseTimer` records milliseconds per stage; accessible in JSON/text |
| **RF-12** | Flow Tracing | Structured logger recording execution path (entry/exit) in critical functions. Toggleable per build type | Phase 2 | `FlowTracer` RAII logs `[→] func` and `[←] func (X ms)`; overhead ≈ 0 in Release |
| **RF-13** | Configuration Validation | Validate YAML/TOML inputs against JSON schema prior to generation | MVP | CLI rejects malformed configs with clear bilingual messages |
| **RF-14** | Progress Reporting | Expose CLI progress indicator or callback during tessellation/remeshing | Phase 2 | `--progress` prints percentage; API exposes `set_progress_callback()` |
| **RF-15** | Degenerate Case Handling | Explicit management of duplicate seeds, collinear points, zero-area regions, and overlapping holes | MVP | Adversarial input tests; controlled failure with structured error codes |
| **RF-16** | Web Documentation Generation | Sphinx-based documentation with modern theme, full-text search, and responsive navigation | MVP | `make html` produces functional site; search and navigation operational |
| **RF-17** | Doxygen & Breathe Integration | Automatic API reference extracted from C++ source comments; class/struct tree generation | MVP | `make html` integrates C++ documentation; signatures complete |
| **RF-18** | Bilingual Documentation | Main content available in both `en_GB` and `pt_BR` via tabs or parallel sections | Phase 2 | Language switch visible; content parity verified |
| **RF-19** | Error Code Catalogue | Complete reference table `[Code][EN][PT][Causes][Solutions]` | MVP | `docs/error-codes/` documents all `VALIDATION_*`, `GENERATION_*`, etc. |
| **RF-20** | Step-by-Step Tutorials | Guides: Getting Started, First 2D Mesh, Domains with Holes, Remeshing, Mohid-NG Integration | MVP | ≥5 functional tutorials; code snippets validated in CI |
| **RF-21** | YAML Configuration Gallery | Commented examples for common cases (rectangle, estuary, island, variable bathymetry) | MVP | `examples/configs/` contains ≥10 validated cases |
| **RF-22** | Architecture Documentation | `architecture/` section with Mermaid diagrams, numbered ADRs, and tessellation/remeshing flowcharts | Phase 2 | `docs/architecture/` renders correctly; ADRs follow template |
| **RF-23** | Versioned Changelog | `CHANGELOG.md` following Keep-a-Changelog; integrated with Git tags and release automation | MVP | Script auto-extracts changes; structure validated by CI |
| **RF-24** | Post-Generation Analysis | Compute geometric metrics (internal angles, orthogonality, skewness, aspect ratio, relative volume) and emit configurable warnings when thresholds are violated | MVP | `MeshAnalyzer::analyse(mesh, criteria)` returns structured `ValidationReport` |

---

## 📗 Non-Functional Requirements (RNF)

| ID | Requirement | Target / Metric | Verification |
|----|-------------|-----------------|--------------|
| **RNF-01** | Generation Performance | Time and memory scale linearly with cell count | ≤ 10s / ≤ 2GB for 1M cells (`hyperfine`, `valgrind massif`) |
| **RNF-02** | Numerical Precision | Robustness via CGAL exact/inexact kernel; configurable tolerances | Relative error < 1e-6 for analytical domain volumes |
| **RNF-03** | Portability | Compile on Linux (WSL/Ubuntu 22.04+), macOS, Windows; native C++20 | GCC≥11, Clang≥12, MSVC≥2022; CI compiler matrix passes |
| **RNF-04** | Maintainability | Core/Backend separation; zero CGAL leakage in public API; coverage > 80% | Public headers contain zero `#include <CGAL/...>`; `gcovr` enforces threshold |
| **RNF-05** | Usability & Accessibility | Intuitive CLI, comprehensive documentation, reproducible examples, clear errors | Novice generates valid mesh in ≤ 15 minutes without reading source |
| **RNF-06** | Interoperability | UGRID/CF-Conventions compliance; Python bindings NumPy/xarray-ready; C API `iso_c_binding` compatible | Files pass `cfchecker`; `pytest` with `xarray` executes cleanly |
| **RNF-07** | Academic Reproducibility | Semantic versioning; versionable configurations; structured logs; explicit citation guidelines | Exact replication across ≥3 distinct machines; `CITATION.cff` + DOI-ready |
| **RNF-08** | Licensing & Compliance | CGAL backend isolated; explicit dependency attribution; correct academic citation | Zero GPL violations; `LICENSE` and `THIRD-PARTY.md` audited in CI |
| **RNF-09** | Scalability | OpenMP for parallel seed generation; MPI domain decomposition preparation | Speedup ≥ 1.8x on 4 cores; `strong/weak scaling` benchmarks pass |
| **RNF-10** | Zero Instrumentation Overhead | Logging and timing compile-time toggleable. When disabled, compiles to **zero instructions** and **zero allocations** | CPU overhead < 0.1% in Release; 0 MB/s memory leak; validated via `perf` |
| **RNF-11** | Deterministic & Compact IDs | `GeneratorID` sequential, `uint64_t`, SoA-aligned, reproducible mapping | Same input → identical connectivity matrix; `sizeof(ID) ≤ 8B` |
| **RNF-12** | Versioning & Deprecation | Strict SemVer; deprecation warnings ≥2 releases ahead; `API_STABILITY.md` maintained | Zero breaking changes in minor releases; CI ABI/API check passes |
| **RNF-13** | Multi-Level CI/CD | Automated validation of translations, schemas, performance regressions, coverage, and test parity | Pipeline fails if coverage < 80%, locale mismatch, or broken links detected |
| **RNF-14** | Safe Remeshing | Split/merge operations preserve local orthogonality, positive volumes, and neighbour consistency | Quality degradation < 5% post-remesh; automatic rollback on invariant violation |
| **RNF-15** | Zero Overhead for Simple Domains | Unconstrained Delaunay selected automatically when `holes.empty()` | Compile-time dispatch; benchmark confirms faster path activation |
| **RNF-16** | Modern SOLID Principles | Concepts replace virtual inheritance; composition > inheritance; compile-time policies for extension | All modules pass `clang-tidy-modernize-*` and `cppcoreguidelines-*` |
| **RNF-17** | Floating-Point Determinism | `fast-math` disabled by default; explicit opt-in for relaxed FP behaviour | CI validates `strict` vs `relaxed` builds; results documented |
| **RNF-18** | Debug Bounds Checking | `std::span::at()` and assertions enabled in Debug; disabled in Release without overhead | ASan/UBSan clean in CI; `NDEBUG` removes all runtime checks |
| **RNF-19** | Thread-Safety Contract | Immutable mesh reads guaranteed thread-safe; generation marked `not thread-safe` by default | `ThreadSanitizer` clean; explicit documentation of concurrency boundaries |
| **RNF-20** | Sanitiser Integration | Address, Undefined Behaviour, and Thread sanitisers toggleable via CMake | `VMM_ENABLE_SANITIZERS=ON` activates; CI fails on first report |
| **RNF-21** | ABI/SO Versioning | `VERSION` and `SOVERSION` properties set in CMake; binary compatibility policy documented | `abi-compliance-checker` runs in release pipeline |
| **RNF-22** | Provenance Metadata | Exported files embed `vmm_version`, `config_sha256`, `generator_seed`, and timestamp | Fields present and parsable in UGRID/VTU headers |
| **RNF-23** | Documentation CI/Deploy | Build ≤ 5 minutes; warnings treated as errors; automatic deployment to `gh-pages` | `sphinx-build -W` passes; `make linkcheck` reports zero broken URLs |