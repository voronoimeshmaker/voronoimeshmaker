# cmake/ConfigUtils.cmake
#
# Utility functions and guard rails.

macro(vgt_block_in_source_builds)
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
        set(_vgt_has_build_files FALSE)

        if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeCache.txt" OR
           EXISTS "${CMAKE_SOURCE_DIR}/cmake_install.cmake" OR
           EXISTS "${CMAKE_SOURCE_DIR}/Makefile" OR
           EXISTS "${CMAKE_SOURCE_DIR}/CMakeFiles")
            set(_vgt_has_build_files TRUE)
        endif()

        if(_vgt_has_build_files)
            message(FATAL_ERROR
                "\nIn-source build detected with existing build artefacts.\n"
                "Remove the generated files manually:\n"
                "  rm -f CMakeCache.txt cmake_install.cmake Makefile\n"
                "  rm -rf CMakeFiles/\n\n"
                "Then configure in a separate build directory:\n"
                "  mkdir -p build && cd build && cmake ..\n"
            )
        else()
            message(FATAL_ERROR
                "\nIn-source builds are not permitted.\n"
                "Configure in a separate build directory:\n"
                "  mkdir -p build && cd build && cmake ..\n"
            )
        endif()
    endif()
endmacro()

vgt_block_in_source_builds()