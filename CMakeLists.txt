# Define the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(VoronoiMeshMaker VERSION 1.0 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options to build documentation, examples, or tests
option(BUILD_DOCUMENTATION "Build Doxygen and Sphinx documentation" OFF)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Locate CGAL, VTK, TBB, and Boost libraries
find_package(CGAL REQUIRED)
find_package(VTK REQUIRED)
find_package(TBB REQUIRED)
find_package(Boost REQUIRED COMPONENTS log log_setup system thread)

# Include directories for CGAL, VTK, TBB, and Boost
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})

# Include directories for your project headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect all source files from src directory and subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the shared library and set the output directory to "lib"
add_library(VoronoiMeshMaker SHARED ${SOURCES})

# Link the library with CGAL, VTK, TBB, and Boost
target_link_libraries(VoronoiMeshMaker
    PUBLIC
    ${CGAL_LIBRARIES}
    ${VTK_LIBRARIES}
    TBB::tbb
    Boost::log Boost::log_setup Boost::system Boost::thread
)

# Set output directories for the library to the "lib" directory
set_target_properties(VoronoiMeshMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set compiler flags for optimizations
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(VoronoiMeshMaker PRIVATE
        -O3                     # Optimization level 3
        -march=native           # Optimize for the current architecture
        -flto                   # Enable Link Time Optimization
        -fopenmp                # Enable OpenMP support if applicable
    )
elseif(MSVC)
    target_compile_options(VoronoiMeshMaker PRIVATE
        /O2                     # Optimization level 2
        /GL                     # Enable Whole Program Optimization
    )
endif()

# Installation rules: Install the library and headers
install(TARGETS VoronoiMeshMaker
    EXPORT VoronoiMeshMakerTargets
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Install header files located in the "include" directory
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp"
)

# Install template files (.tpp) located in the "src" directory
install(DIRECTORY src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.tpp"
)

# Define a custom target to run all examples
if(NOT TARGET run_examples)
    add_custom_target(run_examples
        COMMAND ${CMAKE_COMMAND} -E echo "Running all examples..."
    )
endif()

# Define the macro to add examples in a directory
macro(add_examples_in_directory dir)
    # Collect all .cpp files in the specified directory and subdirectories
    file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS RELATIVE "${dir}" "${dir}/*.cpp")
    if(EXAMPLE_SOURCES)
        foreach(example_source ${EXAMPLE_SOURCES})
            message(STATUS "Found example source: ${example_source}")
            # Construct the full path to the source file
            set(example_full_path "${dir}/${example_source}")
            # Get the name of the executable without extension
            get_filename_component(example_name ${example_source} NAME_WE)
            # Add the executable target
            add_executable(${example_name} "${example_full_path}")
            # Link the executable with the VoronoiMeshMaker library
            target_link_libraries(${example_name} PRIVATE VoronoiMeshMaker)
            # Include directories for headers
            target_include_directories(${example_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
            # Set compiler flags for optimizations
            if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_compile_options(${example_name} PRIVATE -O3 -march=native -flto)
            elseif(MSVC)
                target_compile_options(${example_name} PRIVATE /O2 /GL)
            endif()
            # Set the output directory for the executable
            get_filename_component(example_dir "${example_full_path}" DIRECTORY)
            set_target_properties(${example_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${example_dir}"
            )
            # Add the example to the run_examples target if it exists
            if(TARGET run_examples)
                add_dependencies(run_examples ${example_name})
                add_custom_command(
                    TARGET ${example_name}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "Executable ${example_name} successfully built!"
                    COMMAND ${CMAKE_COMMAND} -E echo "Running example ${example_name}..."
                    COMMAND "${example_dir}/${example_name}"
                    WORKING_DIRECTORY "${example_dir}"
                    COMMENT "Running example ${example_name}..."
                )
            endif()
        endforeach()
    else()
        message(STATUS "No examples found in ${dir}.")
    endif()
endmacro()

# Ensure only one option is enabled
if(BUILD_EXAMPLES AND BUILD_TESTS)
    message(FATAL_ERROR "You can only enable either BUILD_EXAMPLES or BUILD_TESTS, not both.")
endif()

# Add the examples directory if BUILD_EXAMPLES is ON
if(BUILD_EXAMPLES)
    message(STATUS "Building Examples...")
    add_subdirectory(examples)
endif()

# Add the tests directory if BUILD_TESTS is ON
if(BUILD_TESTS)
    message(STATUS "Building Tests...")
    find_package(GTest REQUIRED)
    enable_testing()
    add_subdirectory(tests)
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
