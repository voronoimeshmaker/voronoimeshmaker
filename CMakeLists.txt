# Define the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set policy to suppress warnings for Boost
cmake_policy(SET CMP0167 NEW)

# Set the project name and version
project(VoronoiMeshMaker VERSION 1.0 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force the build type to Release for performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Locate CGAL and VTK libraries
find_package(CGAL REQUIRED)
find_package(VTK REQUIRED)

# Include directories for CGAL and VTK
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})

# Include directories for your project headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect all source files from src directory and subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the shared library and set the output directory to "lib"
add_library(VoronoiMeshMaker SHARED ${SOURCES})

# Link the library with CGAL and VTK
target_link_libraries(VoronoiMeshMaker
    PUBLIC  # Allow other projects to use the CGAL and VTK dependencies transitively
    ${CGAL_LIBRARIES}
    ${VTK_LIBRARIES}
)

# Set output directories for the library and binaries to the "lib" directory
set_target_properties(VoronoiMeshMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set compiler flags for optimizations (adjust based on compiler)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang specific optimizations
    target_compile_options(VoronoiMeshMaker PRIVATE 
        -O3                     # Optimization level 3
        -march=native           # Optimize for the current architecture
        -flto                   # Enable Link Time Optimization
        -fopenmp                # Enable OpenMP support for parallelism (if applicable)
    )
elseif(MSVC)
    # MSVC specific optimizations
    target_compile_options(VoronoiMeshMaker PRIVATE 
        /O2                     # Optimization level 2
        /GL                     # Enable Whole Program Optimization
        /DNDEBUG                # Define NDEBUG for release builds
    )
endif()

# Installation rules: Install the library and headers
install(TARGETS VoronoiMeshMaker
    EXPORT VoronoiMeshMakerTargets
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib  # For compatibility, though not used for shared libraries
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Install header files located in the "include" directory (.h, .hpp, and .tpp files)
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp"
)

# Install template files (.tpp) located in the "src" directory
install(DIRECTORY src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.tpp"
)

# Add the examples directory if there are example programs
add_subdirectory(examples)

# Export the package configuration to the build directory
export(EXPORT VoronoiMeshMakerTargets
    FILE "${CMAKE_BINARY_DIR}/VoronoiMeshMakerTargets.cmake"
)

# Doxygen documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Set Doxygen options and output directory
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx/doxygen)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx/Doxyfile)

    # Configure Doxygen with a template Doxyfile.in
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # Add custom target for generating Doxygen documentation
    add_custom_target(doxygen_doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(WARNING "Doxygen not found, API documentation will not be generated.")
endif()

# Sphinx documentation
find_program(SPHINX_EXECUTABLE sphinx-build)

if(SPHINX_EXECUTABLE)
    # Add custom target for generating Sphinx documentation
    add_custom_target(sphinx_doc ALL
        COMMAND ${SPHINX_EXECUTABLE} -b html ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx ${CMAKE_CURRENT_BINARY_DIR}/docs_sphinx
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx
        COMMENT "Generating documentation with Sphinx"
        VERBATIM
    )

    # Make sphinx_doc depend on doxygen_doc
    add_dependencies(sphinx_doc doxygen_doc)
else()
    message(WARNING "Sphinx not found, Sphinx documentation will not be generated.")
endif()

# Optional: Add a custom target to clean up generated files
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf
    ${CMAKE_BINARY_DIR}/*
    ${CMAKE_BINARY_DIR}/CMakeFiles
    ${CMAKE_BINARY_DIR}/Makefile
    ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMENT "Cleaning up all generated files"
)
