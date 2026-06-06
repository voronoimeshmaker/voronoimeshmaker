# cmake/ConfigTests.cmake

if(VMM_BUILD_TESTS)
    message(STATUS "Configuring test suite...")

    include(CTest)
    enable_testing()
    include(GoogleTest)

    file(GLOB_RECURSE VMM_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/*.cc"
        "${CMAKE_SOURCE_DIR}/tests/*.cxx"
    )

    if(NOT VMM_TEST_SOURCES)
        message(STATUS "  No test source files found in tests/")
    else()
        list(SORT VMM_TEST_SOURCES)

        foreach(_src IN LISTS VMM_TEST_SOURCES)
            get_filename_component(_raw_name "${_src}" NAME_WE)

            vmm_make_cmake_identifier("${_raw_name}" _safe_name)

            if(_safe_name MATCHES "^tst_")
                set(_target "${_safe_name}")
            else()
                set(_target "tst_${_safe_name}")
            endif()

            set(_run_target "run_${_target}")

            if(TARGET "${_target}")
                message(FATAL_ERROR
                    "Duplicated test target '${_target}' generated from '${_src}'"
                )
            endif()

            add_executable("${_target}"
                "${_src}"
            )

            target_include_directories("${_target}"
                PRIVATE ${HDF5_INCLUDE_DIRS}
            )

            target_link_libraries("${_target}"
                PRIVATE
                    VoronoiMeshMaker::VoronoiMeshMaker
                    GTest::gtest_main
                    Threads::Threads
                    ${HDF5_LIBRARIES}
                    vmm_project_options
                    vmm_project_warnings
            )

            set_target_properties("${_target}" PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
                OUTPUT_NAME "${_target}"
            )

            gtest_discover_tests("${_target}"
                WORKING_DIRECTORY "$<TARGET_FILE_DIR:${_target}>"
                DISCOVERY_TIMEOUT 10
            )

            vmm_add_run_target(
                "${_run_target}"
                "${_target}"
                "tests"
            )

            message(STATUS "  Test source    : ${_src}")
            message(STATUS "  Test target    : ${_target}")
            message(STATUS "  Test run target: ${_run_target}")
        endforeach()
    endif()
endif()