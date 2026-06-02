# cmake/ConfigDependencies.cmake
#
# External dependency resolution for VoronoiMeshMaker.

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_POLICY_DEFAULT_CMP0167 NEW)

# Core geometric and parallel backends.
find_package(CGAL REQUIRED COMPONENTS Core)
find_package(TBB  REQUIRED)

# Optional YAML configuration parsing for examples.
if(VMM_BUILD_EXAMPLES)
    find_package(yaml-cpp QUIET)

    if(yaml-cpp_FOUND)
        message(STATUS "yaml-cpp found: YAML-based examples will be built.")
    else()
        message(WARNING "yaml-cpp not found: YAML examples will be skipped.")
    endif()
endif()

# Testing framework.
if(VMM_BUILD_TESTS)
    find_package(GTest   REQUIRED)
    find_package(Threads REQUIRED)
endif()