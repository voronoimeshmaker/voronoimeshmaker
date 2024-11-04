# examples/Parameters/CMakeLists.txt

message(STATUS "Processing examples in ${CMAKE_CURRENT_SOURCE_DIR}")

# Add examples in the current directory
add_examples_in_directory("${CMAKE_CURRENT_SOURCE_DIR}")

# Recursively add subdirectories
file(GLOB SUBDIRS CONFIGURE_DEPENDS LIST_DIRECTORIES true RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        # Add the subdirectory
        add_subdirectory("${subdir}")
    endif()
endforeach()
