# ------------------------------------------------------------
# tests/CMakeLists.txt (robusto: funciona embutido ou standalone)
# - Um executável por diretório em tests/:   ut_<dir>
# - Alvo para rodar cada um:                 run_ut_<dir>
# - Alvo agregado para rodar todos:          run_all_tests
# - Imprime "Testes detectados:" com caminho → alvo.
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

# --- Modo standalone: se este CMakeLists for top-level, defina projeto e testing
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(VoronoiMeshMakerTests LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  include(CTest)
  enable_testing()
endif()

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set(TESTS_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}")
set(VMM_INCLUDE_DIR "${PROJECT_ROOT}/VMMLib/include")

# Garanta que os targets do GTest existam (se raiz não os criou)
if(NOT TARGET GTest::gtest OR NOT TARGET GTest::gtest_main)
  find_package(GTest REQUIRED)
endif()

include(GoogleTest)

# Alvos agregados
add_custom_target(tests          COMMENT "Compilando todos os testes")
add_custom_target(run_all_tests  COMMENT "Executando todos os testes")

# 1) Coleta todos os .cpp RELATIVOS a tests/
file(GLOB_RECURSE TEST_CPP_REL CONFIGURE_DEPENDS
     RELATIVE "${TESTS_ROOT}" "${TESTS_ROOT}/*.cpp")

# 2) Deriva as pastas (dentro de tests/) que têm .cpp
set(TEST_DIRS_REL "")
foreach(f IN LISTS TEST_CPP_REL)
  get_filename_component(d "${f}" DIRECTORY)
  if(d STREQUAL "")  # arquivo direto em tests/
    set(d ".")
  endif()
  list(APPEND TEST_DIRS_REL "${d}")
endforeach()
list(REMOVE_DUPLICATES TEST_DIRS_REL)

if(TEST_DIRS_REL STREQUAL "")
  message(STATUS "  [Tests] Nenhum teste encontrado em ${TESTS_ROOT}")
  return()
else()
  message(STATUS "  [Tests] Testes detectados:")
endif()

foreach(d_rel IN LISTS TEST_DIRS_REL)
  # Caminho absoluto + nome da pasta para o binário
  if(d_rel STREQUAL ".")
    set(dir_path "${TESTS_ROOT}")
    set(dir_name "tests")
  else()
    set(dir_path "${TESTS_ROOT}/${d_rel}")
    get_filename_component(dir_name "${d_rel}" NAME)
  endif()

  # Nome do alvo: ut_<dir> (com sufixo se houver colisão)
  set(base_target "ut_${dir_name}")
  set(target_name "${base_target}")
  if(TARGET "${target_name}")
    string(MD5 _hash "${d_rel}")
    string(SUBSTRING "${_hash}" 0 8 _short)
    set(target_name "${base_target}__${_short}")
  endif()

  # Fontes deste diretório (não-recursivo para evitar duplicatas)
  file(GLOB TEST_SOURCES "${dir_path}/*.cpp")

  # Mensagem estilo "Exemplos detectados"
  message(STATUS "    - ${dir_path}  -> alvo: ${target_name}")

  # 1) Executável por diretório
  add_executable(${target_name} ${TEST_SOURCES})
  target_include_directories(${target_name} PRIVATE "${VMM_INCLUDE_DIR}")
  target_link_libraries(${target_name} PRIVATE
    VoronoiMeshMaker
    GTest::gtest
    GTest::gtest_main
  )

  # Binário ao lado dos fontes; nome do binário = nome da pasta
  set_target_properties(${target_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${dir_path}"
    OUTPUT_NAME "${dir_name}"
  )

  # Otimizações leves em Release
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(${target_name} PRIVATE -O3 -march=native)
    elseif(MSVC)
      target_compile_options(${target_name} PRIVATE /O2)
    endif()
  endif()

  # 2) Registrar no CTest
  gtest_discover_tests(${target_name}
    WORKING_DIRECTORY "${dir_path}"
    TEST_PREFIX "${target_name}:"
    DISCOVERY_TIMEOUT 30
  )

  # 3) Alvos convenientes
  add_dependencies(tests ${target_name})
  add_custom_target(run_${target_name}
    DEPENDS ${target_name}
    COMMAND "${dir_path}/${dir_name}"
    WORKING_DIRECTORY "${dir_path}"
    COMMENT "Executando testes em ${dir_name} (${dir_path})"
  )
  add_dependencies(run_all_tests run_${target_name})
endforeach()
