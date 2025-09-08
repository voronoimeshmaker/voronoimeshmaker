# SolidLab

Biblioteca didática em **C++20** para treinar **SOLID**, **TDD** e boas práticas.
A API é mínima (classe `solidlab::SolidLab`) e o projeto já vem com:

- **auto‑descoberta de exemplos** (`examples/` → alvos `exe_<pasta>`, `run_exe_<pasta>`, `run_all`)
- **auto‑descoberta de testes** (`tests/` → alvos `ut_<pasta>`, `run_ut_<pasta>`, `run_all_tests`)
- geração opcional de **documentação Doxygen**

Durante a configuração você verá mensagens como:
```
-- Building Examples...
--   [Examples] Exemplos detectados:
--     - .../examples/hello  -> alvo: exe_hello
-- Building Tests...
--   [Tests] Testes detectados:
--     - .../tests/SolidLab  -> alvo: ut_SolidLab
```

---

## Estrutura do repositório

```
SolidLab/
├─ CMakeLists.txt                  # raiz (lib, examples, tests, docs)
├─ SolidLabLib/
│  ├─ include/
│  │  └─ SolidLab.hpp             # API pública (Doxygen completo)
│  └─ src/
│     └─ SolidLab.cpp
├─ examples/                       # cada pasta com .cpp vira um executável
│  └─ hello/
│     └─ main.cpp
├─ tests/                          # cada pasta com .cpp vira um executável de testes
│  └─ SolidLab/
│     └─ test_solidlab.cpp
├─ docs/
│  └─ mainpage.md                  # página principal do Doxygen (opcional)
└─ bin/                            # saída da biblioteca (libSolidLab.so/.dll/.dylib/.a/.lib)
```

---

## Requisitos

- **CMake ≥ 3.16** (usa `FetchContent` para baixar o GoogleTest)
- **Compilador C++20**
  - GCC ≥ 10 / Clang ≥ 10 / MSVC (VS 2019 16.10+)
- **Make** ou **Ninja** (qualquer gerador suportado pelo CMake)

> **OpenMP**: o `CMakeLists.txt` ativa `-fopenmp` **apenas** para GCC/Clang.  
> - **Linux (GCC)**: normalmente já funciona (libgomp).  
> - **macOS**: `brew install llvm libomp` e use o Clang do Homebrew (veja seção macOS).  
> - **Windows (MSVC)**: OpenMP **não** é ativado por padrão.

---

## Instalação / Build

> **Importante:** por projeto, **Examples** e **Tests** são **mutuamente exclusivos** (evita compilar tudo ao mesmo tempo). Habilite **um** por vez.

### Compilar **apenas exemplos**
```bash
cmake -S . -B build -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build -j
```

### Compilar **apenas testes**
```bash
cmake -S . -B build -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build -j
```

### Saída dos artefatos
- A biblioteca **SolidLab** é emitida em `./bin/`.
- Executáveis de **exemplos** são emitidos **ao lado** dos `.cpp` dentro de `examples/...`.
- Executáveis de **testes** são emitidos **ao lado** dos `.cpp` dentro de `tests/...`.

---

## Executando **exemplos**

O `examples/CMakeLists.txt` cria:
- **um executável por diretório** com `.cpp` → **alvo** `exe_<nome_da_pasta>`
- **um alvo para executar** cada um → `run_exe_<nome_da_pasta>`
- **um alvo agregado** → `run_all` (executa todos em série)

Comandos úteis:
```bash
# compilar todos os exemplos
cmake --build build --target examples

# executar todos os exemplos
cmake --build build --target run_all

# executar um exemplo específico (pasta 'hello')
cmake --build build --target run_exe_hello

# binário gerado ao lado do fonte:
./examples/hello/hello        # Linux/macOS
# .\examples\hello\hello.exe  # Windows
```

### Adicionar um novo exemplo
Crie uma pasta com um `.cpp` dentro de `examples/`:
```
examples/minha_demo/main.cpp
```
Na próxima configuração/compilação, surgirão os alvos `exe_minha_demo` e `run_exe_minha_demo` automaticamente.

---

## Executando **testes**

Quando `-DBUILD_TESTS=ON`, o CMake raiz **baixa e configura** o **GoogleTest** via `FetchContent` (não precisa instalar no sistema).  
O `tests/CMakeLists.txt` cria:
- **um executável por diretório** em `tests/` → **alvo** `ut_<nome_da_pasta>`
- **um alvo para rodar** cada um → `run_ut_<nome_da_pasta>`
- **um alvo agregado** → `run_all_tests`
- Integração com **CTest** via `gtest_discover_tests`

Comandos úteis:
```bash
# compilar todos os testes
cmake --build build --target tests -j

# rodar todos os testes (alvo de conveniência)
cmake --build build --target run_all_tests

# alternativa via CTest (coleta tudo que foi descoberto)
ctest --test-dir build --output-on-failure

# rodar apenas os testes da pasta tests/SolidLab
cmake --build build --target run_ut_SolidLab

# rodar o binário diretamente (útil para filtros do GTest)
./tests/SolidLab/SolidLab --gtest_filter=SolidLab_Join.*
```

### Adicionar um novo conjunto de testes
Crie uma nova pasta dentro de `tests/` e adicione `.cpp`:
```
tests/Vector3/test_ops.cpp
```
Na próxima configuração/compilação, surgirão os alvos `ut_Vector3` e `run_ut_Vector3`.  
Os testes também aparecem no CTest com prefixo `ut_Vector3:`.

---

## Documentação (Doxygen) — opcional

- A API (`SolidLab.hpp`) possui comentários Doxygen detalhados.
- A **página principal** fica em `docs/mainpage.md`.

Gerar documentação:
```bash
cmake -S . -B build -DBUILD_DOCUMENTATION=ON
cmake --build build --target docs
# abra: build/docs/html/index.html
```

---

## Uso mínimo da API

```cpp
#include "SolidLab.hpp"
#include <iostream>
using solidlab::SolidLab;

int main() {
  SolidLab lab;
  lab.add_note("alpha");
  lab.add_note("beta");
  std::cout << "SolidLab v" << SolidLab::version() << "\n";
  std::cout << "Joined: " << lab.join('-') << "\n"; // "alpha-beta"
}
```

---

## Plataformas / Notas específicas

### macOS (Homebrew)
```bash
brew install cmake llvm libomp
export CC=$(brew --prefix)/opt/llvm/bin/clang
export CXX=$(brew --prefix)/opt/llvm/bin/clang++
```

### Windows (Visual Studio)
Use **Developer PowerShell** ou **x64 Native Tools Prompt**:
```powershell
cmake -S . -B build -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build --config Release --target run_all
# ctest:
ctest --test-dir build -C Release --output-on-failure
```

---

## Limpeza / Alvos úteis

```bash
# limpar artefatos de build (equivalente a "make clean")
cmake --build build --target clean

# limpeza pesada (alvo customizado)
cmake --build build --target distclean

# listar alvos disponíveis
cmake --build build --target help
```

---

## Solução de problemas (FAQ)

- **“You can only enable either BUILD_EXAMPLES or BUILD_TESTS”**  
  Habilite apenas um dos dois flags ao configurar.

- **“No rule to make target 'clear'”**  
  Você executou `make clear`. Para limpar o terminal use `clear`.  
  Para limpar o build: `cmake --build build --target clean`.

- **Erros de OpenMP no macOS (GOMP/omp)**  
  Instale `libomp` e use o Clang do Homebrew (veja a seção macOS).  
  Em último caso, remova `-fopenmp` do `CMakeLists.txt`.

- **Problemas de rede ao baixar GoogleTest (FetchContent)**  
  Configure proxy/variáveis de ambiente ou baixe o zip e aponte `URL` para um caminho local.

---

## Licença
Uso educativo. Se existir arquivo de licença no repositório, ele prevalece.
