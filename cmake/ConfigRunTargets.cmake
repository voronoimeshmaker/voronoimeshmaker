# cmake/ConfigRunTargets.cmake
#
# Defines helper targets:
#
#   make run_ex_Caso1
#   make run_tst_QualityAnalysisTest
#   make run_examples
#   make run_tests
#   make run_all
#   make list_run_targets

function(vmm_make_cmake_identifier input_name output_var)
    set(_name "${input_name}")

    string(REPLACE "{" "" _name "${_name}")
    string(REPLACE "}" "" _name "${_name}")

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" _name "${_name}")
    string(REGEX REPLACE "_+" "_" _name "${_name}")
    string(REGEX REPLACE "^_+" "" _name "${_name}")
    string(REGEX REPLACE "_+$" "" _name "${_name}")

    if(_name STREQUAL "")
        set(_name "unnamed")
    endif()

    if(_name MATCHES "^[0-9]")
        set(_name "n_${_name}")
    endif()

    set(${output_var} "${_name}" PARENT_SCOPE)
endfunction()

function(vmm_add_run_target run_target executable_target group_name)
    if(TARGET "${run_target}")
        message(FATAL_ERROR "Duplicated run target: ${run_target}")
    endif()

    add_custom_target("${run_target}"
        COMMAND "$<TARGET_FILE:${executable_target}>"
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${executable_target}>"
        DEPENDS "${executable_target}"
        USES_TERMINAL
        VERBATIM
    )

    set(_vmm_run_entry
        "$<TARGET_FILE:${executable_target}>|$<TARGET_FILE_DIR:${executable_target}>|${run_target}"
    )

    set_property(GLOBAL APPEND PROPERTY VMM_RUN_TARGETS
        "${run_target}"
    )

    set_property(GLOBAL APPEND PROPERTY VMM_RUN_EXECUTABLE_TARGETS
        "${executable_target}"
    )

    set_property(GLOBAL APPEND PROPERTY VMM_RUN_ALL_ENTRIES
        "${_vmm_run_entry}"
    )

    if(group_name STREQUAL "examples")
        set_property(GLOBAL APPEND PROPERTY VMM_EXAMPLE_ENTRIES
            "${_vmm_run_entry}"
        )

        set_property(GLOBAL APPEND PROPERTY VMM_EXAMPLE_EXECUTABLE_TARGETS
            "${executable_target}"
        )
    elseif(group_name STREQUAL "tests")
        set_property(GLOBAL APPEND PROPERTY VMM_TEST_ENTRIES
            "${_vmm_run_entry}"
        )

        set_property(GLOBAL APPEND PROPERTY VMM_TEST_EXECUTABLE_TARGETS
            "${executable_target}"
        )
    endif()
endfunction()

function(vmm_add_run_group_target target_name empty_message)
    set(_vmm_entries ${ARGN})

    if(NOT _vmm_entries)
        add_custom_target("${target_name}"
            COMMAND "${CMAKE_COMMAND}" -E echo "${empty_message}"
            VERBATIM
        )
        return()
    endif()

    set(_vmm_script "set(_vmm_run_entries\n")

    foreach(_vmm_entry IN LISTS _vmm_entries)
        string(APPEND _vmm_script
            "    [==[${_vmm_entry}]==]\n"
        )
    endforeach()

    string(APPEND _vmm_script ")\n")
    string(APPEND _vmm_script "\n")
    string(APPEND _vmm_script "foreach(_vmm_entry IN LISTS _vmm_run_entries)\n")
    string(APPEND _vmm_script "    string(REPLACE \"|\" \";\" _vmm_fields \"\${_vmm_entry}\")\n")
    string(APPEND _vmm_script "    list(LENGTH _vmm_fields _vmm_field_count)\n")
    string(APPEND _vmm_script "    if(NOT _vmm_field_count EQUAL 3)\n")
    string(APPEND _vmm_script "        message(FATAL_ERROR \"Invalid run entry: \${_vmm_entry}\")\n")
    string(APPEND _vmm_script "    endif()\n")
    string(APPEND _vmm_script "    list(GET _vmm_fields 0 _vmm_exe)\n")
    string(APPEND _vmm_script "    list(GET _vmm_fields 1 _vmm_workdir)\n")
    string(APPEND _vmm_script "    list(GET _vmm_fields 2 _vmm_name)\n")
    string(APPEND _vmm_script "    message(STATUS \"Running \${_vmm_name}: \${_vmm_exe}\")\n")
    string(APPEND _vmm_script "    execute_process(\n")
    string(APPEND _vmm_script "        COMMAND \"\${_vmm_exe}\"\n")
    string(APPEND _vmm_script "        WORKING_DIRECTORY \"\${_vmm_workdir}\"\n")
    string(APPEND _vmm_script "        RESULT_VARIABLE _vmm_result\n")
    string(APPEND _vmm_script "    )\n")
    string(APPEND _vmm_script "    if(NOT _vmm_result EQUAL 0)\n")
    string(APPEND _vmm_script "        message(FATAL_ERROR \"\${_vmm_name} failed with exit code \${_vmm_result}\")\n")
    string(APPEND _vmm_script "    endif()\n")
    string(APPEND _vmm_script "endforeach()\n")

    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_$<CONFIG>.cmake"
        CONTENT "${_vmm_script}"
    )

    add_custom_target("${target_name}"
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_$<CONFIG>.cmake"
        USES_TERMINAL
        VERBATIM
    )
endfunction()

function(vmm_create_run_group_targets)
    get_property(_vmm_run_targets GLOBAL PROPERTY VMM_RUN_TARGETS)
    get_property(_vmm_run_executable_targets GLOBAL PROPERTY VMM_RUN_EXECUTABLE_TARGETS)

    get_property(_vmm_example_entries GLOBAL PROPERTY VMM_EXAMPLE_ENTRIES)
    get_property(_vmm_example_executable_targets GLOBAL PROPERTY VMM_EXAMPLE_EXECUTABLE_TARGETS)

    get_property(_vmm_test_entries GLOBAL PROPERTY VMM_TEST_ENTRIES)
    get_property(_vmm_test_executable_targets GLOBAL PROPERTY VMM_TEST_EXECUTABLE_TARGETS)

    get_property(_vmm_run_all_entries GLOBAL PROPERTY VMM_RUN_ALL_ENTRIES)

    vmm_add_run_group_target(
        run_examples
        "No example programs were configured."
        ${_vmm_example_entries}
    )

    vmm_add_run_group_target(
        run_tests
        "No test programs were configured."
        ${_vmm_test_entries}
    )

    vmm_add_run_group_target(
        run_all
        "No runnable programs were configured."
        ${_vmm_run_all_entries}
    )

    if(_vmm_example_executable_targets)
        add_dependencies(run_examples
            ${_vmm_example_executable_targets}
        )
    endif()

    if(_vmm_test_executable_targets)
        add_dependencies(run_tests
            ${_vmm_test_executable_targets}
        )
    endif()

    if(_vmm_run_executable_targets)
        add_dependencies(run_all
            ${_vmm_run_executable_targets}
        )
    endif()

    if(_vmm_run_targets)
        string(REPLACE ";" " " _vmm_run_targets_line "${_vmm_run_targets}")

        add_custom_target(list_run_targets
            COMMAND "${CMAKE_COMMAND}" -E echo "Available run targets:"
            COMMAND "${CMAKE_COMMAND}" -E echo "${_vmm_run_targets_line}"
            VERBATIM
        )
    else()
        add_custom_target(list_run_targets
            COMMAND "${CMAKE_COMMAND}" -E echo "No run targets were configured."
            VERBATIM
        )
    endif()
endfunction()