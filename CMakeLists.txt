# ------------------------------------------------------------
# Root CMakeLists.txt for VoronoiMeshMaker
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.30)

# Project definition MUST be in the main CMakeLists.txt
project(VoronoiMeshMaker VERSION 1.0 LANGUAGES CXX)

# Include all modules
include(cmake/ConfigUtils.cmake)          # Utilities (without project())
include(cmake/ConfigOptions.cmake)        # Options and policies
include(cmake/ConfigCompiler.cmake)       # Compiler settings
include(cmake/ConfigDependencies.cmake)   # Dependencies
include(cmake/ConfigTargets.cmake)        # Main library target
include(cmake/ConfigExamples.cmake)       # Examples
include(cmake/ConfigTests.cmake)          # Tests
include(cmake/ConfigDocs.cmake)           # Documentation

# Final message
message(STATUS "VoronoiMeshMaker configuration complete!")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")