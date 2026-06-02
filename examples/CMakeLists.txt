# examples/CMakeLists.txt
# ---------------------------------------------------------------------------
# Automatically discovers .cpp files, creates executables (ex_<Name>),
# and registers companion run targets (run_ex_<Name>) for one-step execution.
# Preserves original casing and directory structure.
# ---------------------------------------------------------------------------
if(VMM_BUILD_EXAMPLES)
    file(GLOB_RECURSE VMM_EXAMPLE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/examples/*.cpp")

    foreach(_src IN LISTS VMM_EXAMPLE_SOURCES)
        get_filename_component(_name "${_src}" NAME_WE)
        set(_target "ex_${_name}")

        # 1. Build target
        add_executable(${_target} "${_src}")
        target_link_libraries(${_target} PRIVATE
            VoronoiMeshMaker::VoronoiMeshMaker
            vmm_project_options
            vmm_project_warnings
        )
        if(yaml-cpp_FOUND)
            target_link_libraries(${_target} PRIVATE yaml-cpp::yaml-cpp)
        endif()

        set_target_properties(${_target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples"
        )

        # 2. Run target (builds first, then executes)
        add_custom_target(run_${_target}
            COMMAND $<TARGET_FILE:${_target}>
            DEPENDS ${_target}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT "▶ Executing ${_target}..."
            VERBATIM
        )
        message(STATUS "  → Registered: ${_target} | run_${_target}")
    endforeach()
endif()