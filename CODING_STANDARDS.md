# Padrões de Código e Estilo

## 🏷️ Nomenclatura (LLVM + C++ Core Guidelines)
| Entidade | Padrão | Exemplo |
|----------|--------|---------|
| Namespaces | `lowercase` | `vmm`, `vmm::core`, `vmm::backend::cgal_2d` |
| Classes/Structs/Concepts | `PascalCase` | `MeshTopology`, `FlowTracer`, `MeshGenerator` |
| Funções/Métodos | `snake_case` | `generate_mesh()`, `compute_volume()` |
| Variáveis/Parâmetros | `snake_case` | `cell_id`, `min_distance` |
| Membros Privados | `snake_case_` | `centroids_`, `generator_ids_`, `next_id_` |
| Constantes/Macros Build | `UPPER_SNAKE_CASE` | `VMM_MAX_CELLS`, `VMM_HAS_LOGGER` |
| `enum class` values | `PascalCase` | `BoundaryType::HoleBoundary` |
| Templates | `PascalCase` descritivo | `Dim`, `Strategy`, `StorageLayout` |

## 🧱 SOLID Reinterpretado para C++20 + DOD
- **SRP**: Separação estrita entre dados (SoA), transformações (views), I/O (exporters), validação
- **OCP**: Novos backends/métricas via `concepts` + policies compile-time. Zero modificação em código existente
- **LSP**: Tipos que satisfazem um `concept` são substituíveis sem quebrar invariantes
- **ISP**: Contratos granulares (`ExporterConcept`, `MetricConcept`) em vez de interfaces monolíticas
- **DIP**: Dependência em abstrações de dados e `concepts`. Injeção via templates. Herança virtual evitada.

## 💾 Data-Oriented Design (DOD)
- Layout interno: `Structure of Arrays` (SoA) para campos quentes (`volumes_`, `centroids_`)
- API pública: `std::span`, iteradores, views. Zero exposição de `std::vector` interno
- Zero alocação em loops críticos. Pré-alocação + `std::span` obrigatório em validação/exportação
- Alinhamento SIMD: `alignas(32)` para arrays contíguos; loops vetorizáveis por compilador

## 🧪 Paridade de Testes
- Toda classe nova **deve** ter arquivo `tests/<modulo>/<NomeClasse>Test.cpp` no mesmo PR
- CI rejeita se `src/**/*.hpp` não tiver correspondente em `tests/**/*Test.cpp`
- Usar Catch2: `TEST_CASE("...", "[tag]") { REQUIRE(...); }`
- Macros RAII de instrumentação (`VMM_FLOW`, `VMM_PHASE`) só ativas em `Debug` ou flags explícitas

## 🛠️ Formatação & CI
- `.clang-format`: `BasedOnStyle: LLVM`, `IndentWidth: 4`, `SortIncludes: true`
- `.clang-tidy`: `modernize-*, cppcoreguidelines-*, performance-*`
- PRs falham se `clang-format --dry-run --Werror` ou `clang-tidy` reportarem erros