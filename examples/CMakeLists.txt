# Define the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Examples)

# Function to add all subdirectories recursively
function(add_subdirectories_recursive dir)
    file(GLOB SUBDIRS RELATIVE ${dir} ${dir}/*)
    foreach(subdir ${SUBDIRS})
        if(IS_DIRECTORY ${dir}/${subdir})
            add_subdirectory(${dir}/${subdir})
        endif()
    endforeach()
endfunction()

# Recursively add all subdirectories
add_subdirectories_recursive(${CMAKE_CURRENT_SOURCE_DIR})
