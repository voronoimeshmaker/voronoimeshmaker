# VoronoiMeshMaker

[![License: GPL-3.0+](https://img.shields.io/badge/License-GPLv3%2B-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)
[![Build & Test](https://github.com/voronoimeshmaker/voronoimeshmaker/actions/workflows/build-test.yml/badge.svg)](https://github.com/voronoimeshmaker/voronoimeshmaker/actions)
[![Docs Source](https://img.shields.io/badge/Docs-Source-yellow.svg)](docs/)

A high-performance, non-structured Voronoi mesh generator for hydrodynamic modelling and scientific computing. Designed for direct integration with `Mohid-NG` and HPC solvers, the library employs modern C++20, Data-Oriented Design (DOD), strict backend isolation, and toggleable instrumentation.

---

## Overview

`VoronoiMeshMaker` addresses limitations in legacy discretisation tools by providing:
- **Robust tessellation**: Constrained Delaunay (2D) and Delaunay/Regular (3D) via CGAL, with metric-driven remeshing
- **Scientific architecture**: C++20 concepts, ranges, SoA memory layout, and compile-time policy composition
- **Reproducibility**: Versionable configurations, provenance metadata, bilingual error reporting, and automated documentation
- **Clean integration**: A public API free of CGAL dependencies; solver communication via neutral file formats or a stable C API

---

## Technical Features

| Category | Capabilities |
|----------|--------------|
| **Tessellation** | Constrained Delaunay 2D (external boundaries + internal holes), Delaunay/Regular 3D, deterministic dualisation, sequential generator ID assignment |
| **Remeshing** | Metric-driven split/coarsen/smoothing, adaptive refinement guided by scalar fields, topological preservation, continuous ID reassignment |
| **Memory Layout & Performance** | Structure-of-Arrays (SoA) internally, `std::span`/views publicly, SIMD-aligned allocations, zero dynamic allocation in hot paths |
| **Modern Abstraction** | C++20 concepts replacing virtual inheritance, compile-time policies, composable ranges, `std::expected` for recoverable failures |
| **IDs & Connectivity** | Deterministic sequential `GeneratorID`, explicit CSR connectivity matrix, direct mapping to sparse solver structures |
| **Instrumentation** | `FlowTracer` (entry/exit) and `PhaseTimer` toggleable at compile time; negligible overhead in release builds |
| **Errors & Warnings** | Bilingual system (`en_GB` default, `pt_BR` supported), static error codes, structured context, runtime locale switching |
| **Export Formats** | VTK/VTU (ParaView), MSH v4 (Gmsh/OpenFOAM), UGRID/NetCDF (CF-Conventions) with quality metadata |
| **Quality Analysis** | Internal angles, orthogonality, skewness, aspect ratio, relative volume; configurable warnings or fatal validation |

---

## Architecture & Design Principles

### Core / Backend Separation
- **Public API (`include/vmm/`)**: Plain data structures, views, and concepts. Contains zero `#include <CGAL/...>` directives.
- **Private Backend (`src/backend/`)**: CGAL encapsulated within `cgal_2d/` and `cgal_3d/`. Linked strictly as `PRIVATE`.
- **Integration Contract**: External solvers consume exported neutral files or interact via a stable C API (`iso_c_binding`).

### DOD & C++20
- Internal layout uses Structure-of-Arrays for hot fields (`volumes_`, `centroids_`, `flags_`).
- The public API exposes `std::span`, lazy iterators, and composable filters.
- SOLID principles are reinterpreted for modern C++: concepts guarantee LSP/OCP; composition replaces inheritance; compile-time policies replace runtime factories.

### 2D / 3D Strategy
- **Unified API**: `template<Dimension Dim> class MeshTopology` provides a dimension-agnostic interface.
- **Specialised Backends**: `CDTGenerator2D` and `DTGenerator3D` are maintained in separate source directories.
- **Optimised Path**: When no holes are present, the pipeline automatically selects the faster unconstrained Delaunay routine via compile-time dispatch.

### Licensing
- Library: **GPLv3+** (required for CGAL compatibility).
- Solver (`Mohid-NG`): May adopt independent licences provided communication occurs via neutral mesh files or the C API (no direct linkage to CGAL headers).

---

## Quick Start (WSL / Ubuntu)

### 1. Dependencies
```bash
sudo apt update && sudo apt install -y \
  cmake g++ make \
  libcgal-dev libtbb-dev \
  doxygen graphviz \
  python3-pip python3-venv