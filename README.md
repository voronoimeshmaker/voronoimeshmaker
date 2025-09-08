# VoronoiMeshMaker (VMM)

Biblioteca em **C++20** baseada em **CGAL** para geração de **malhas de Voronoi**, com foco futuro em **modelagem de EDPs**.  
O projeto é guiado por **SOLID** e **TDD**, com estrutura de **exemplos** e **testes** auto-descobertos via CMake.

## Principais pontos

- **C++20** e headers centrais em `VMMLib/include/VoronoiMeshMaker/Core/`  
  (`namespace.h`, `type.h`, `constants.h`)
- **Kernel**: `CGAL::Exact_predicates_inexact_constructions_kernel` (EPIK)
- **Descoberta automática de exemplos** (em `examples/`)  
  – cada exemplo fica **em sua própria pasta** e o fonte chama-se `ex_<nome>.cpp`
- **Descoberta automática de testes** (em `tests/`) com GoogleTest + CTest
- Build da lib **compila mesmo vazia** (símbolo âncora no TU mínimo)

---

## Estrutura do repositório

```
VoronoiMeshMaker/
├─ CMakeLists.txt
├─ VMMLib/
│  ├─ include/
│  │  └─ VoronoiMeshMaker/
│  │     └─ Core/
│  │        ├─ namespace.h
│  │        ├─ type.h
│  │        └─ constants.h
│  └─ src/
│     └─ VoronoiMeshMaker/
│        └─ vmm_minimal.cpp        # TU mínimo (símbolo de link)
├─ examples/                        # EXEMPLOS: um por pasta, fonte ex_<nome>.cpp
│  └─ core/
│     ├─ core1/ ex_core1.cpp
│     └─ core2/ ex_core2.cpp
└─ tests/                           # TESTES com GoogleTest (um binário por pasta)
   └─ Core/
      └─ ut_core.cpp
```

---

## Dependências

- **CMake ≥ 3.16**
- **Compilador C++20** (GCC, Clang ou MSVC)
- **CGAL** (inclui componentes e *targets* `CGAL::CGAL` e `CGAL::CGAL_Core`)
- **GMP** e **MPFR** (normalmente trazidos como dependências do CGAL)
- **Boost** (o CGAL pode requerer — já funciona com `BoostConfig.cmake`)

> Caso veja o aviso de *policy* **CMP0167** ao localizar Boost, você pode
> silenciar no `CMakeLists.txt` raiz (antes de `find_package(CGAL ...)`):
>
> ```cmake
> if(POLICY CMP0167)
>   cmake_policy(SET CMP0167 NEW)  # ou OLD, conforme preferir
> endif()
> ```

---

## Como compilar

### 1) Só a biblioteca
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### 2) Biblioteca + **exemplos**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON
cmake --build build -j
```

### 3) Biblioteca + **testes**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

> Dica: se preferir, deixe ambos ON; só considere o tempo de build.

---

## Executando os **exemplos**

A lógica de `examples/CMakeLists.txt` cria **um executável por pasta** que contenha um arquivo `ex_*.cpp`.  
O binário é gerado **na própria pasta** do exemplo.

Targets úteis (Make/Ninja):

```bash
# compila todos os exemplos
cmake --build build --target examples

# executa todos os exemplos em série
cmake --build build --target run_all_examples

# executa um exemplo específico (o alvo tem o nome do arquivo sem .cpp)
cmake --build build --target run_ex_core1

# execução direta (Linux/macOS)
./examples/core/core1/ex_core1
```

### Adicionando um novo exemplo

Crie uma pasta e adicione **um** `ex_<nome>.cpp`:

```
examples/voronoi/2d/ex_seeded_diagram.cpp
```

Na próxima configuração/compilação, surgirá o alvo `ex_seeded_diagram` e o
binário `examples/voronoi/2d/ex_seeded_diagram`.

---

## Executando os **testes**

- Cada pasta em `tests/` que contenha `.cpp` gera um executável `ut_<pasta>`.
- Os testes são integrados ao **CTest** via `gtest_discover_tests`.

Comandos:

```bash
# compilar todos os testes
cmake --build build --target tests -j

# rodar via alvo agregado
cmake --build build --target run_all_tests

# rodar via CTest
ctest --test-dir build --output-on-failure

# rodar apenas os testes de uma pasta
cmake --build build --target run_ut_Core

# rodar o binário diretamente (útil para filtros do GTest)
./tests/Core/Core --gtest_filter=CoreConstants.*
```

---

## Convenções (SOLID / Core)

- **Namespaces**  
  Use os macros de abertura/fechamento:
  ```cpp
  VORMAKER_NAMESPACE_OPEN
  // ... vmm:: ...
  VORMAKER_NAMESPACE_CLOSE
  ```
  Subespaço de geometria: `vmm::gtp`.

- **Tipos** (`type.h`)  
  Kernel CGAL **EPIK**, aliases para `Point2D/3D`, `Segment2D/3D`, `Polygon2D`,
  `Polyhedron3D`, `Real = gtp::Kernel::FT`, containers (`VecPoint2D`, etc.).

- **Constantes** (`constants.h`)  
  `kZeroTol` (~1e-12 para `double`), `kEpsilon` (1e-6), `kPi`, com aliases
  legados: `ZERO`, `EPSILON`, `PI`, `LSIZE`, `Pi()`.

- **TU mínimo**  
  `vmm_minimal.cpp` exporta um símbolo “âncora” e toca `PI/Real` para evitar
  otimizações agressivas quando a lib ainda está vazia.

---

## Exemplo mínimo (Core)

```cpp
// examples/core/core1/ex_core1.cpp
#include <iostream>
#include <VoronoiMeshMaker/Core/type.h>
#include <VoronoiMeshMaker/Core/constants.h>

int main() {
    using namespace vmm;
    gtp::Polygon2D tri;
    tri.push_back({0.0, 0.0});
    tri.push_back({1.0, 0.0});
    tri.push_back({0.0, 1.0});

    std::cout << "PI = " << static_cast<double>(PI) << "\n";
    std::cout << "area(tri) = " << CGAL::to_double(tri.area()) << "\n";
}
```

---

## Notas de CMake (resumo)

- **Alvo da lib**: `VoronoiMeshMaker`
- **Includes públicos**: `VMMLib/include`
- **Linkagem**: `CGAL::CGAL` e `CGAL::CGAL_Core` (trazem GMP/MPFR transitivamente)
- **Exemplos**: `add_subdirectory(examples)` quando `BUILD_EXAMPLES=ON`
- **Testes**: `add_subdirectory(tests)` quando `BUILD_TESTS=ON`

---

## Roadmap (curto)

- [ ] Geração de diagrama de Voronoi 2D (sementes pontuais)
- [ ] Construtores/validadores de malha (restrições de qualidade)
- [ ] *Backends* de exportação (VTK/PLY/OBJ)
- [ ] Integração com discretização para EDPs

---

## Licença

**GNU GPL v3** (veja o cabeçalho dos arquivos ou `LICENSE`, se presente).
