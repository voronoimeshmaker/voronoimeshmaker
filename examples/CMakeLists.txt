# ------------------------------------------------------------
# examples/CMakeLists.txt
# - Um executável por pasta que contenha exatamente um ex_*.cpp
# - Suporta subpastas aninhadas (p.ex.: core/core1, core/core2, ...)
# - Alvos agregados:
#     examples          -> compila todos
#     run_all_examples  -> executa todos
#     run_<target>      -> executa um específico (ex.: run_ex_core1)
# - Cada binário é gerado ao lado do seu .cpp
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.30)


# Se estiver usando CMake 3.24 ou superior
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Evita warning do FetchContent (CMP0135) ao extrair arquivos
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# --- Modo standalone (se abrir esta pasta direto com cmake):
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(VoronoiMeshMakerExamples LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(PROJECT_ROOT    "${CMAKE_SOURCE_DIR}")
set(EXAMPLES_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}")
set(VMM_INCLUDE_DIR "${PROJECT_ROOT}/VMMLib/include")

# Alvos agregados
add_custom_target(examples         COMMENT "Compilando todos os exemplos")
add_custom_target(run_all_examples COMMENT "Executando todos os exemplos")

# 1) Varre recursivamente, pega todos os ex_*.cpp
file(GLOB_RECURSE EXAMPLE_CPP_ABS CONFIGURE_DEPENDS
     "${EXAMPLES_ROOT}/ex_*.cpp")

if(EXAMPLE_CPP_ABS STREQUAL "")
  message(STATUS "  [Examples] Nenhum ex_*.cpp encontrado em ${EXAMPLES_ROOT}")
  return()
endif()

# 2) Mapeia: diretório -> lista de ex_*.cpp dentro dele
#    (esperado: 1 arquivo por pasta; se houver mais, avisamos e usamos todos)
set(EXAMPLE_DIRS_ABS "")
foreach(f_abs IN LISTS EXAMPLE_CPP_ABS)
  get_filename_component(dir_abs "${f_abs}" DIRECTORY)
  list(APPEND EXAMPLE_DIRS_ABS "${dir_abs}")
endforeach()
list(REMOVE_DUPLICATES EXAMPLE_DIRS_ABS)

message(STATUS "  [Examples] Pastas de exemplos detectadas:")

foreach(dir_abs IN LISTS EXAMPLE_DIRS_ABS)
  # Arquivos desta pasta (apenas neste nível)
  file(GLOB EX_SOURCES_THIS_DIR "${dir_abs}/ex_*.cpp")
  if(EX_SOURCES_THIS_DIR STREQUAL "")
    # (não deve acontecer pois a pasta veio do passo 2)
    continue()
  endif()

  # Caminho relativo para mensagens e para compor nomes
  file(RELATIVE_PATH dir_rel "${EXAMPLES_ROOT}" "${dir_abs}")
  if(dir_rel STREQUAL "")
    set(dir_rel ".")
  endif()

  # Nome base do alvo:
  #   - se existir só um ex_*.cpp, usamos o stem desse arquivo (ex_core1)
  #   - se houver mais de um, usamos o nome da pasta "flattened"
  list(LENGTH EX_SOURCES_THIS_DIR _n_src)
  if(_n_src EQUAL 1)
    list(GET EX_SOURCES_THIS_DIR 0 _only_src)
    get_filename_component(stem "${_only_src}" NAME_WE) # ex_core1
    set(target_name "${stem}")
  else()
    # transforma 'core/core1' -> 'core_core1'
    string(REPLACE "/" "_" target_name "${dir_rel}")
    set(target_name "ex_${target_name}")
    message(WARNING "  [Examples] Pasta '${dir_rel}' contém ${_n_src} exemplos; "
                    "o alvo será '${target_name}' com todos eles.")
  endif()

  # Garante unicidade de nome (caso haja colisão com outro alvo)
  if(TARGET "${target_name}")
    string(MD5 _hash "${dir_rel}")
    string(SUBSTRING "${_hash}" 0 8 _short)
    set(target_name "${target_name}__${_short}")
  endif()

  message(STATUS "    - ${dir_rel}  -> alvo: ${target_name}")

  # 3) Cria o executável desta pasta
  add_executable(${target_name} ${EX_SOURCES_THIS_DIR})
  target_link_libraries(${target_name} PRIVATE VoronoiMeshMaker)
  target_include_directories(${target_name} PRIVATE "${VMM_INCLUDE_DIR}")

  # Binário é gerado dentro da própria pasta
  # Nome do binário:
  #   - 1 arquivo  -> stem do arquivo (ex_core1)
  #   - vários     -> nome do alvo (ex_core_core1)
  if(_n_src EQUAL 1)
    set(out_name "${stem}")
  else()
    set(out_name "${target_name}")
  endif()

  set_target_properties(${target_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${dir_abs}"
    OUTPUT_NAME "${out_name}"
  )

  # Otimizações leves em Release
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(${target_name} PRIVATE -O3 -march=native)
    elseif(MSVC)
      target_compile_options(${target_name} PRIVATE /O2)
    endif()
  endif()

  # Alvos auxiliares
  add_dependencies(examples ${target_name})
  add_custom_target(run_${target_name}
    DEPENDS ${target_name}
    COMMAND "${dir_abs}/${out_name}"
    WORKING_DIRECTORY "${dir_abs}"
    COMMENT "Executando exemplo ${out_name} (${dir_rel})"
  )
  add_dependencies(run_all_examples run_${target_name})
endforeach()
