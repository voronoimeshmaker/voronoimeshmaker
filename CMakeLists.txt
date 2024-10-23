# Define the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(VoronoiMeshMaker VERSION 1.0 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force the build type to Release for performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Locate CGAL, VTK, and TBB libraries
find_package(CGAL REQUIRED)
find_package(VTK REQUIRED)
find_package(TBB REQUIRED)

# Include directories for CGAL, VTK, and TBB
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})

# Include directories for your project headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect all source files from src directory and subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the shared library and set the output directory to "lib"
add_library(VoronoiMeshMaker SHARED ${SOURCES})

# Link the library with CGAL, VTK, and TBB
target_link_libraries(VoronoiMeshMaker
    PUBLIC
    ${CGAL_LIBRARIES}
    ${VTK_LIBRARIES}
    ${TBB_LIBRARIES}
)

# Set output directories for the library and binaries to the "lib" directory
set_target_properties(VoronoiMeshMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set compiler flags for optimizations (adjust based on compiler)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(VoronoiMeshMaker PRIVATE 
        -O3                     # Optimization level 3
        -march=native           # Optimize for the current architecture
        -flto                   # Enable Link Time Optimization
        -fopenmp                # Enable OpenMP support for parallelism (if applicable)
    )
elseif(MSVC)
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
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
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

# Define a custom target to run all examples (define apenas uma vez na raiz)
if(NOT TARGET run_examples)
    add_custom_target(run_examples
        COMMAND ${CMAKE_COMMAND} -E echo "Running all examples..."
    )
endif()

# Function to add each example to the run_examples target
function(add_example_target target_name)
    if(TARGET run_examples)
        add_dependencies(run_examples ${target_name})
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${target_name}
            COMMENT "Running example ${target_name}..."
        )
    endif()
endfunction()

# Option to choose between building examples or tests
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

# Ensure only one option is enabled
if(BUILD_EXAMPLES AND BUILD_TESTS)
    message(FATAL_ERROR "You can only enable either BUILD_EXAMPLES or BUILD_TESTS, not both.")
endif()

# Add the examples directory if BUILD_EXAMPLES is ON
if(BUILD_EXAMPLES)
    message(STATUS "Building Examples...")
    add_subdirectory(examples)
endif()

# Locate Google Test for testing
if(BUILD_TESTS)
    message(STATUS "Building Tests...")
    find_package(GTest REQUIRED)
    enable_testing()

    # Add the tests directory
    add_subdirectory(tests)
endif()

# Option to build Doxygen and Sphinx documentation
option(BUILD_DOCUMENTATION "Build Doxygen and Sphinx documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
        # Correcting the path to the Doxyfile.in
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx/Doxyfile.in)
        if(EXISTS ${DOXYGEN_IN})
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

            add_custom_target(doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating Doxygen documentation"
                VERBATIM)
        else()
            message(FATAL_ERROR "Doxyfile.in not found at ${DOXYGEN_IN}. Please check the path.")
        endif()
    else()
        message(WARNING "Doxygen not found. Documentation will not be generated.")
    endif()

# Add custom target to build Sphinx documentation
find_program(SPHINX_EXECUTABLE sphinx-build)
if(SPHINX_EXECUTABLE)
    add_custom_target(doc_sphinx ALL
        COMMAND ${SPHINX_EXECUTABLE} -b html ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx ${CMAKE_CURRENT_SOURCE_DIR}/sphinx
        COMMENT "Generating Sphinx documentation with HTML output"
        VERBATIM)
else()
    message(WARNING "Sphinx not found. Sphinx documentation will not be generated.")
endif()
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
