# ------------------------------------------------------------
# Root CMakeLists.txt for VoronoiMeshMaker
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.30)

# Policies (silence common warnings on newer CMake)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(VoronoiMeshMaker VERSION 1.0 LANGUAGES CXX)

# ------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------
# Options
# ------------------------------------------------------------
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS    "Build tests"    ON)
option(BUILD_DOCS     "Build docs (Doxygen + Sphinx)" ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# ------------------------------------------------------------
# Project layout
# ------------------------------------------------------------
set(VMMLib_DIR ${CMAKE_CURRENT_SOURCE_DIR}/VMMLib)

# Output for binaries (.so/.exe) -> ./bin
set(VMM_OUTPUT_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(${VMMLib_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     "${VMMLib_DIR}/src/*.cpp")

add_library(VoronoiMeshMaker SHARED ${SOURCES})

# CGAL (required)
find_package(CGAL REQUIRED COMPONENTS Core)
target_link_libraries(VoronoiMeshMaker
    PUBLIC CGAL::CGAL CGAL::CGAL_Core
)

# Optimizations (tune as you like)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(VoronoiMeshMaker PRIVATE -O3 -march=native -flto)
elseif(MSVC)
  target_compile_options(VoronoiMeshMaker PRIVATE /O2 /GL)
endif()

set_target_properties(VoronoiMeshMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
)

# Install rules
install(TARGETS VoronoiMeshMaker
    EXPORT VoronoiMeshMakerTargets
    LIBRARY DESTINATION ${VMM_OUTPUT_BIN_DIR}
    ARCHIVE DESTINATION ${VMM_OUTPUT_BIN_DIR}
    RUNTIME DESTINATION ${VMM_OUTPUT_BIN_DIR})

install(DIRECTORY ${VMMLib_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp")

install(DIRECTORY ${VMMLib_DIR}/src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.tpp")

# ------------------------------------------------------------
# Examples
# ------------------------------------------------------------
macro(add_examples_in_directory dir)
  file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
       RELATIVE "${dir}" "${dir}/*.cpp")
  if(EXAMPLE_SOURCES)
    foreach(example_source ${EXAMPLE_SOURCES})
      get_filename_component(example_name ${example_source} NAME_WE)
      set(example_full_path "${dir}/${example_source}")
      get_filename_component(example_dir "${example_full_path}" DIRECTORY)

      add_executable(${example_name} "${example_full_path}")
      target_link_libraries(${example_name} PRIVATE VoronoiMeshMaker)
      target_include_directories(${example_name} PRIVATE
          ${VMMLib_DIR}/include)

      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${example_name} PRIVATE -O3 -march=native -flto)
      elseif(MSVC)
        target_compile_options(${example_name} PRIVATE /O2 /GL)
      endif()

      set_target_properties(${example_name} PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY "${example_dir}")
    endforeach()
  endif()
endmacro()

if(BUILD_EXAMPLES)
  message(STATUS "Building Examples...")
  add_subdirectory(examples)
endif()

# ------------------------------------------------------------
# Tests (GoogleTest via FetchContent)
# ------------------------------------------------------------
if(BUILD_TESTS)
  message(STATUS "Building Tests...")
  include(CTest)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  add_subdirectory(tests)
endif()

# ------------------------------------------------------------
# Documentation (Doxygen + Sphinx)
# ------------------------------------------------------------

# Sphinx source and build dirs
set(SPHINX_SRC   "${CMAKE_SOURCE_DIR}/docs_sphinx")
set(SPHINX_BUILD "${CMAKE_BINARY_DIR}/docs_sphinx")

# Where to publish final HTML docs (in source tree)
set(DOCS_PUBLISH_DIR "${CMAKE_SOURCE_DIR}/documentation" CACHE PATH
    "Directory in source tree to publish generated HTML docs")

# Doxygen
find_package(Doxygen QUIET)
if(BUILD_DOCS AND DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
  set(DOXYFILE_IN  "${SPHINX_SRC}/Doxyfile.in")
  set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

  if(NOT EXISTS "${DOXYFILE_IN}")
    message(WARNING "Doxyfile.in not found at ${DOXYFILE_IN}. Skipping Doxygen.")
  else()
    configure_file("${DOXYFILE_IN}" "${DOXYFILE_OUT}" @ONLY)

    add_custom_target(doc_doxygen
      COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
      BYPRODUCTS "${DOXYGEN_OUTPUT_DIR}/xml/index.xml"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      COMMENT "Generating Doxygen XML"
      VERBATIM)
  endif()
endif()

# Sphinx detection (prefer venv, then system)
set(SPHINX_EXECUTABLE "" CACHE FILEPATH "Path to sphinx-build executable")

if(BUILD_DOCS AND NOT SPHINX_EXECUTABLE)
  if(DEFINED ENV{VIRTUAL_ENV})
    if(WIN32)
      set(_cand "$ENV{VIRTUAL_ENV}/Scripts/sphinx-build.exe")
    else()
      set(_cand "$ENV{VIRTUAL_ENV}/bin/sphinx-build")
    endif()
    if(EXISTS "${_cand}")
      set(SPHINX_EXECUTABLE "${_cand}" CACHE FILEPATH "sphinx-build from active venv" FORCE)
    endif()
  endif()
endif()

if(BUILD_DOCS AND NOT SPHINX_EXECUTABLE)
  if(WIN32)
    set(_cand "${CMAKE_SOURCE_DIR}/.venv/Scripts/sphinx-build.exe")
  else()
    set(_cand "${CMAKE_SOURCE_DIR}/.venv/bin/sphinx-build")
  endif()
  if(EXISTS "${_cand}")
    set(SPHINX_EXECUTABLE "${_cand}" CACHE FILEPATH "sphinx-build from .venv" FORCE)
  endif()
endif()

if(BUILD_DOCS AND NOT SPHINX_EXECUTABLE)
  find_program(SPHINX_EXECUTABLE sphinx-build)
endif()

# Sphinx target (depends on Doxygen if available)
if(BUILD_DOCS)
  if(NOT SPHINX_EXECUTABLE)
    message(WARNING "sphinx-build not found. Sphinx docs will be skipped.")
  else()
    if(NOT TARGET doc_doxygen)
      add_custom_target(doc_doxygen
        COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not available: skipping XML generation.")
      set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
    endif()

    add_custom_target(doc_sphinx
      COMMAND ${CMAKE_COMMAND} -E make_directory "${SPHINX_BUILD}/html"
      # Pass Doxygen XML path via environment variable (read in conf.py)
      COMMAND ${CMAKE_COMMAND} -E env VMM_DOXY_XML=${DOXYGEN_OUTPUT_DIR}/xml
              "${SPHINX_EXECUTABLE}" -b html
              "${SPHINX_SRC}" "${SPHINX_BUILD}/html"
      # Publish to source-tree directory
      COMMAND ${CMAKE_COMMAND} -E rm -rf "${DOCS_PUBLISH_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${SPHINX_BUILD}/html" "${DOCS_PUBLISH_DIR}"
      DEPENDS doc_doxygen
      WORKING_DIRECTORY "${SPHINX_SRC}"
      COMMENT "Generating Sphinx HTML docs and publishing to ${DOCS_PUBLISH_DIR}"
      VERBATIM)

    add_custom_target(doc ALL
      DEPENDS doc_sphinx
      COMMENT "Build full documentation (Doxygen + Sphinx)")
  endif()
endif()

# Distclean (purge build-tree artefacts)
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf
      ${CMAKE_BINARY_DIR}/*
      ${CMAKE_BINARY_DIR}/CMakeFiles
      ${CMAKE_BINARY_DIR}/Makefile
      ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMENT "Cleaning build artifacts")
