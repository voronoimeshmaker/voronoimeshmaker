# cmake/ConfigExamples.cmake

if(VMM_BUILD_EXAMPLES)
    message(STATUS "Configuring examples...")

    file(GLOB_RECURSE VMM_EXAMPLE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/examples/*.cpp"
        "${CMAKE_SOURCE_DIR}/examples/*.cc"
        "${CMAKE_SOURCE_DIR}/examples/*.cxx"
    )

    if(NOT VMM_EXAMPLE_SOURCES)
        message(STATUS "  No example source files found in examples/")
    else()
        list(SORT VMM_EXAMPLE_SOURCES)

        foreach(_src IN LISTS VMM_EXAMPLE_SOURCES)
            get_filename_component(_raw_name "${_src}" NAME_WE)

            vmm_make_cmake_identifier("${_raw_name}" _safe_name)

            if(_safe_name MATCHES "^ex_")
                set(_target "${_safe_name}")
            else()
                set(_target "ex_${_safe_name}")
            endif()

            set(_run_target "run_${_target}")

            if(TARGET "${_target}")
                message(FATAL_ERROR
                    "Duplicated example target '${_target}' generated from '${_src}'"
                )
            endif()

            add_executable("${_target}"
                "${_src}"
            )

            target_link_libraries("${_target}"
                PRIVATE
                    VoronoiMeshMaker::VoronoiMeshMaker
                    vmm_project_options
                    vmm_project_warnings
            )

            if(yaml-cpp_FOUND)
                target_link_libraries("${_target}"
                    PRIVATE
                        yaml-cpp::yaml-cpp
                )
            endif()

            set_target_properties("${_target}" PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples"
                OUTPUT_NAME "${_target}"
            )

            vmm_add_run_target(
                "${_run_target}"
                "${_target}"
                "examples"
            )

            message(STATUS "  Example source    : ${_src}")
            message(STATUS "  Example target    : ${_target}")
            message(STATUS "  Example run target: ${_run_target}")
        endforeach()
    endif()
endif()