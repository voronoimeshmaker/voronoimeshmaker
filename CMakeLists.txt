cmake_minimum_required(VERSION 3.24)
project(VoronoiMeshMaker VERSION 0.1.0 LANGUAGES CXX)

include(cmake/ConfigUtils.cmake)
include(cmake/ConfigOptions.cmake)

# Dependências externas
find_package(CGAL REQUIRED COMPONENTS Core)
find_package(TBB REQUIRED)

include(cmake/ConfigTargets.cmake)
include(cmake/ConfigTests.cmake)
include(cmake/ConfigPaper.cmake)
if(VMM_BUILD_EXAMPLES) add_subdirectory(examples) endif()
