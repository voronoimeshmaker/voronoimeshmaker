# VoronoiMeshMaker

[![License: GPL-3.0+](https://img.shields.io/badge/License-GPLv3%2B-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)
[![Build & Test](https://github.com/voronoimeshmaker/voronoimeshmaker/actions/workflows/build-test.yml/badge.svg)](https://github.com/voronoimeshmaker/voronoimeshmaker/actions)
[![Docs](https://github.com/voronoimeshmaker/voronoimeshmaker/actions/workflows/docs.yml/badge.svg)](https://voronoimeshmaker.github.io/voronoimeshmaker)

Gerador de malhas não estruturadas Voronoi 2D/3D de alta performance para modelagem hidrodinâmica, geociências e simulações científicas. Projetado para integração direta com `Mohid-NG` e solvers HPC (PETSc, Trilinos), com arquitetura moderna, isolamento rigoroso de dependências e instrumentação zero-overhead.

---

## 🎯 Visão & Propósito

O `VoronoiMeshMaker` nasce para superar limitações de discretização de ferramentas legadas, oferecendo:
- **Geração robusta**: Tesselação via CGAL (CDT 2D com furos / Delaunay 3D) + remeshing guiado por métricas
- **Arquitetura científica**: C++20 Concepts, Ranges, Data-Oriented Design (SoA), zero herança virtual
- **Reprodutibilidade acadêmica**: Configs versionáveis, metadados de proveniência, erros bilíngues, documentação Sphinx/Doxygen
- **Integração limpa**: API pública livre de CGAL; comunicação com solvers via arquivos neutros ou bindings C/Fortran

---

## ✨ Características Técnicas

| Categoria | Recursos |
|-----------|----------|
| **Tesselação & Remeshing** | CDT 2D (contornos + furos), Delaunay 3D, split/coarsen/smooth por campo métrico, preservação topológica |
| **Layout & Performance** | Structure-of-Arrays (SoA) interno, `std::span`/views públicas, alinhamento SIMD, zero alocação em hot paths |
| **Abstração Moderna** | C++20 Concepts > herança, policies compile-time, Ranges composables, `std::expected` para falhas esperadas |
| **IDs & Conectividade** | `GeneratorID` sequencial determinístico, matriz CSR explícita, mapeamento direto para solvers esparsos |
| **Instrumentação** | `FlowTracer` (entry/exit) e `PhaseTimer` toggleáveis via CMake; overhead ≈ 0 em Release |
| **Erros & Warnings** | Sistema bilíngue (`pt_BR` default, `en_US` fallback), códigos estáticos, contexto estruturado, switching runtime |
| **Exportação** | VTK/VTU (Paraview), MSH v4 (Gmsh/OpenFOAM), UGRID/NetCDF (CF-Conventions) com metadados de qualidade |
| **Análise de Qualidade** | Ângulos internos, ortogonalidade, skewness, aspect ratio, volume relativo; warnings configuráveis ou falha fatal |

---

## 🏗️ Arquitetura & Filosofia de Design

### 🔹 Separação Core / Backend
- **API Pública (`include/vmm/`)**: PODs, views, conceitos. **Zero** `#include <CGAL/...>`
- **Backend Privado (`src/backend/`)**: CGAL encapsulado em `cgal_2d/` e `cgal_3d/`. Linkado como `PRIVATE`
- **Contrato de Integração**: `Mohid-NG` consome via arquivos neutros ou C API estável (`iso_c_binding`)

### 🔹 DOD & C++20
- Layout interno: `Structure of Arrays` para campos quentes (`volumes_`, `centroids_`, `flags_`)
- API pública: `std::span`, iteradores lazy, filtros composáveis (`mesh.cells() | filter(...)`)
- SOLID reinterpretado: Concepts garantem LSP/OCP; composition > inheritance; policies compile-time > factories runtime

### 🔹 Estratégia 2D / 3D
- **API unificada**: `template<Dimension Dim> class MeshTopology`
- **Backends especializados**: `CDTGenerator2D` vs `DTGenerator3D` (separação explícita em `src/`)
- **Zero overhead sem furos**: `if constexpr (holes.empty())` usa Delaunay padrão mais rápido

### 🔹 Licenciamento
- Biblioteca: **GPLv3+** (compatível com CGAL-GPL)
- Solver (`Mohid-NG`): Licença independente via intercâmbio de arquivos neutros (sem linkagem direta)

---

## 🚀 Início Rápido (WSL / Ubuntu)

### 1. Dependências
```bash
sudo apt update && sudo apt install -y \
  cmake g++ make \
  libcgal-dev libtbb-dev \
  doxygen graphviz \
  python3-pip python3-venv