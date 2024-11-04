# examples/CMakeLists.txt

message(STATUS "Processing examples in ${CMAKE_CURRENT_SOURCE_DIR}")

# Add examples in the current directory and subdirectories
add_examples_in_directory("${CMAKE_CURRENT_SOURCE_DIR}")
