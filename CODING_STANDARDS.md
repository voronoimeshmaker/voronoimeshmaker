# Coding Standards & Style Guide

This document defines the mandatory conventions for all source code in the `VoronoiMeshMaker` repository. Adherence is enforced via automated CI gates, static analysis, and mandatory peer review.

## 🏷️ Naming Conventions

All identifiers must follow the LLVM / C++ Core Guidelines convention. Hungarian notation, scope prefixes (`m_`, `g_`, `p_`), and abbreviation-heavy names are prohibited.

| Entity | Convention | Example | Rule |
|--------|------------|---------|------|
| **Namespaces** | `lowercase` | `vmm`, `vmm::core`, `vmm::backend::cgal_2d` | Hierarchical, short, no underscores or camel case |
| **Classes / Structs / Concepts** | `PascalCase` | `MeshTopology`, `FlowTracer`, `MeshGenerator` | Includes `enum class`, `concept`, `union` |
| **Functions / Methods** | `snake_case` | `generate_mesh()`, `compute_volume()`, `is_boundary()` | Readable, aligns with CLI/YAML and scientific notation |
| **Variables / Parameters** | `snake_case` | `cell_id`, `min_distance`, `boundary_flag` | Type and context convey semantics; no redundant prefixes |
| **Private Members** | `snake_case` + trailing `_` | `centroids_`, `generator_ids_`, `next_id_` | Eliminates `this->` ambiguity and shadowing warnings |
| **Constants / Build Macros** | `UPPER_SNAKE_CASE` | `VMM_MAX_CELLS`, `VMM_HAS_LOGGER`, `M_PI` | Strictly for compile-time toggles and RAII wrappers |
| **`enum class` Values** | `PascalCase` | `enum class BoundaryType { Internal, ExternalBoundary, HoleBoundary }` | Type-safe, explicit scoping, no global namespace pollution |
| **Template Parameters** | `PascalCase` (descriptive) | `Dim`, `Strategy`, `StorageLayout`, `T` | `T`/`U` only for trivial generics; prefer semantic names |
| **Source Files** | `PascalCase.hpp` / `PascalCase.cpp` | `MeshTopology.hpp`, `CDTGenerator.cpp` | Lowercase extensions; one primary concept per file |

## Repository Exceptions and Transitional Placeholders

The following repository-level exceptions are intentional and should not be treated
as style violations:

- `VMMLib/include/vmm/vmm.hpp` is the lowercase umbrella header.
- Test source files use the `tst_*.cpp` prefix so generated CMake targets remain
  visually distinct from production targets.
- Example programs use the `ex_*.cpp` prefix for the same reason.
- Module namespaces may use underscores when they match existing directory names
  or backend identifiers, for example `vmm::site_generation`, `vmm::backend::cgal_2d`,
  `builtin_error_id`, and `export_format`.

Empty placeholder files are transitional only. A placeholder must either be
removed before release or replaced with a minimal documented stub explaining its
reserved purpose. Empty test files must not be used as permanent placeholders,
because GTest discovery expects a real executable.

## 🧱 Modern SOLID Principles (C++20 & DOD)

Classical SOLID is reinterpreted for compile-time composition and data-oriented memory layouts. Virtual inheritance is avoided in favour of concepts and policies.

| Principle | Modern C++20 Implementation |
|-----------|-----------------------------|
| **SRP** | Strict separation between data (SoA arrays), transformations (views/ranges), I/O (exporters), and validation. Each module owns a single data flow. |
| **OCP** | New backends, metrics, or export formats are added via `concept` specialisations or policy templates. Existing code is never modified. |
| **LSP** | Any type satisfying a `concept` is fully substitutable. Compile-time constraints guarantee invariant preservation without runtime checks. |
| **ISP** | Granular contracts (`ExporterConcept`, `MetricConcept`, `CellRangeConcept`) replace monolithic interfaces. Consumers depend only on required capabilities. |
| **DIP** | Depend on abstractions (concepts, POD types, views), not concrete implementations. Injection occurs at compile time via templates. Runtime factories are reserved for CLI/plugin boundaries only. |

## 💾 Data-Oriented Design (DOD)

The library prioritises cache locality, predictable memory access, and SIMD readiness over traditional object-oriented encapsulation.

- **Structure-of-Arrays (SoA)**: Hot fields (`volumes_`, `centroids_`, `flags_`) are stored in contiguous, aligned arrays. Cold metadata is separated.
- **Public API Abstraction**: Consumers interact exclusively via `std::span`, lazy iterators, and range adapters. Internal `std::vector` layouts are never exposed.
- **Zero Allocation in Hot Paths**: Validation, metric computation, and export routines must operate on pre-allocated buffers. `new`, `std::make_unique`, and heap growth are prohibited in tight loops.
- **SIMD Readiness**: Contiguous arrays are aligned to 32 bytes (`alignas(32)`). Loops must be written to allow compiler auto-vectorisation (`#pragma omp simd` or `std::execution::par_unseq` where supported).
- **Cold/Hot Data Splitting**: Frequently accessed data (coordinates, volumes, neighbour indices) is grouped. Infrequently accessed data (IDs, tags, metadata) is stored separately to prevent cache line pollution.

## 🧪 Test Parity & Instrumentation

- **Mandatory Test Coverage**: Every new public class or significant function requires a corresponding `tests/<module>/<ClassName>Test.cpp` in the same pull request. The CI pipeline rejects merges lacking test parity.
- **Framework**: Catch2 is preferred for unit testing due to its header-only distribution, BDD-style syntax, and seamless CMake integration. GTest remains acceptable if already adopted in downstream projects.
- **Toggleable Instrumentation**: RAII wrappers (`VMM_FLOW`, `VMM_PHASE`) compile to **zero instructions** when `VMM_ENABLE_FLOW_LOGGER` or `VMM_ENABLE_PROFILING` are disabled. They must never alter program state or memory layout.
- **Deterministic IDs**: `GeneratorID` assignment must be strictly sequential and reproducible across builds. Tests must verify identical inputs yield identical ID sequences and connectivity matrices.

## 🛠️ Formatting & Static Analysis

- **`clang-format`**: Enforces `LLVM` base style with 4-space indentation, 120-character column limit, and automatic include sorting. PRs failing `clang-format --dry-run --Werror` are rejected.
- **`clang-tidy`**: Activates `modernize-*`, `cppcoreguidelines-*`, `performance-*`, and `bugprone-*`. Warnings are treated as errors. Known safe exceptions are explicitly suppressed via `.clang-tidy` or inline comments.
- **Sanitiser Compliance**: Debug builds must pass AddressSanitizer (ASan) and UndefinedBehaviourSanitizer (UBSan) without leaks or invalid memory access. ThreadSanitizer (TSan) validates concurrency boundaries before merging parallel code.
- **Documentation Consistency**: All public headers must contain Doxygen-compatible comments. Missing documentation for `public` or `protected` interfaces triggers CI failure.

---

*These standards are not stylistic preferences; they are engineering constraints required to guarantee numerical reproducibility, cache-efficient execution, and long-term maintainability. Deviations require explicit architectural justification and maintainer approval.*