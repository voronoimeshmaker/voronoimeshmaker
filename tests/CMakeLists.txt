# tests/CMakeLists.txt
# ---------------------------------------------------------------------------
# Test suite configuration.
# Automatically discovers .cpp files, creates prefixed executables (tst_*),
# and registers all GTest cases with CTest.
# ---------------------------------------------------------------------------

include(GoogleTest)

# Discover all test source files in this directory and subdirectories
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

if(NOT TEST_SOURCES)
    message(WARNING "No test source files found in tests/")
    return()
endif()

foreach(_src IN LISTS TEST_SOURCES)
    # Extract filename without path or extension
    get_filename_component(_name "${_src}" NAME_WE)
    set(_target "tst_${_name}")

    # Create executable for this test file
    add_executable(${_target} "${_src}")
    target_link_libraries(${_target} PRIVATE
        VoronoiMeshMaker::VoronoiMeshMaker
        GTest::gtest_main
        Threads::Threads
        vmm_project_options
        vmm_project_warnings
    )

    # Automatically discover all TEST/TEST_F macros and register with CTest
    gtest_discover_tests(${_target}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests"
        DISCOVERY_TIMEOUT 10
    )

    # Keep test binaries isolated in bin/tests/
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
    )
endforeach()