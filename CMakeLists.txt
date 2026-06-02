cmake_minimum_required(VERSION 3.25)

cmake_policy(SET CMP0135 NEW)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(VoronoiMeshMaker
    VERSION 1.0
    DESCRIPTION "High-performance Voronoi mesh generation library for scientific computing"
    LANGUAGES CXX
)

include(cmake/ConfigUtils.cmake)
include(cmake/ConfigOptions.cmake)
include(cmake/ConfigCompiler.cmake)
include(cmake/ConfigDependencies.cmake)
include(cmake/ConfigTargets.cmake)
include(cmake/ConfigRunTargets.cmake)
include(cmake/ConfigExamples.cmake)
include(cmake/ConfigTests.cmake)
include(cmake/ConfigDocs.cmake)

vmm_create_run_group_targets()

get_property(VMM_CONFIGURED_RUN_TARGETS GLOBAL PROPERTY VMM_RUN_TARGETS)

message(STATUS "VoronoiMeshMaker configuration complete")
message(STATUS "  Build type          : ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard        : ${CMAKE_CXX_STANDARD}")
message(STATUS "  Shared library      : ${VMM_BUILD_SHARED_LIBS}")
message(STATUS "  Examples            : ${VMM_BUILD_EXAMPLES}")
message(STATUS "  Tests               : ${VMM_BUILD_TESTS}")
message(STATUS "  Documentation       : ${VMM_BUILD_DOCS}")
message(STATUS "  Warnings            : ${VMM_ENABLE_WARNINGS}")
message(STATUS "  Native architecture : ${VMM_ENABLE_NATIVE_ARCH}")
message(STATUS "  LTO/IPO             : ${VMM_ENABLE_LTO}")
message(STATUS "  Fast math           : ${VMM_ENABLE_FAST_MATH}")
message(STATUS "  Sanitisers          : ${VMM_ENABLE_SANITIZERS}")
message(STATUS "  Install prefix      : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Run targets         : ${VMM_CONFIGURED_RUN_TARGETS}")