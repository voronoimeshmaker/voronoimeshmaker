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
find_package(HDF5 REQUIRED COMPONENTS C)

if(VMM_ENABLE_GIS)
    find_package(GDAL QUIET)
    find_package(PROJ QUIET)

    if(GDAL_FOUND)
        message(STATUS "GDAL/OGR found: GIS vector readers will be enabled.")
    else()
        message(STATUS "GDAL/OGR not found: GIS vector readers will report unsupported operation.")
    endif()

    if(PROJ_FOUND)
        message(STATUS "PROJ found: CRS transformation backend is available.")
    else()
        message(STATUS "PROJ not found: CRS transformation backend is unavailable.")
    endif()
endif()

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