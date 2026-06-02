# cmake/ConfigDocs.cmake
# ---------------------------------------------------------------------------
# Documentation pipeline: Doxygen (C++ API) + Sphinx (User Guides).
# Handles Python environment detection, module validation, and HTML generation.
# ---------------------------------------------------------------------------

if(VMM_BUILD_DOCS)
    message(STATUS "Configuring documentation pipeline...")

    set(SPHINX_SRC       "${CMAKE_SOURCE_DIR}/docs_sphinx")
    set(SPHINX_BUILD     "${CMAKE_BINARY_DIR}/docs_sphinx")
    set(DOCS_PUBLISH_DIR "${CMAKE_SOURCE_DIR}/documentation" CACHE PATH
        "Destination directory for published HTML documentation")

    # Python & Sphinx detection
    find_package(Python3 COMPONENTS Interpreter QUIET)

    if(NOT DEFINED SPHINX_EXECUTABLE)
        set(SPHINX_EXECUTABLE "" CACHE FILEPATH "Path to sphinx-build")
    endif()

    # Attempt to locate sphinx-build in virtual environments
    if(NOT SPHINX_EXECUTABLE AND Python3_Interpreter_FOUND)
        if(DEFINED ENV{VIRTUAL_ENV})
            set(_vmm_sphinx_cand "$ENV{VIRTUAL_ENV}/Scripts/sphinx-build.exe"
                                 "$ENV{VIRTUAL_ENV}/bin/sphinx-build")
        else()
            set(_vmm_sphinx_cand "${CMAKE_SOURCE_DIR}/.venv/Scripts/sphinx-build.exe"
                                 "${CMAKE_SOURCE_DIR}/.venv/bin/sphinx-build")
        endif()
        foreach(_vmm_path IN LISTS _vmm_sphinx_cand)
            if(EXISTS "${_vmm_path}")
                set(SPHINX_EXECUTABLE "${_vmm_path}" CACHE FILEPATH "sphinx-build from venv" FORCE)
                break()
            endif()
        endforeach()
    endif()

    if(NOT SPHINX_EXECUTABLE)
        find_program(SPHINX_EXECUTABLE sphinx-build)
    endif()

    # Validate required Python modules
    set(_vmm_sphinx_req_modules "breathe;myst_parser;sphinx_design")
    set(_vmm_missing_modules "")
    if(Python3_Interpreter_FOUND)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c
                "import importlib.util, sys; "
                "missing=[m for m in sys.argv[1:] if not importlib.util.find_spec(m)]; "
                "print(';'.join(missing)); "
                "sys.exit(1 if missing else 0)"
            ${_vmm_sphinx_req_modules}
            RESULT_VARIABLE _vmm_module_check
            OUTPUT_VARIABLE _vmm_missing_modules
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()

    # Doxygen configuration
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
    find_package(Doxygen QUIET)

    if(DOXYGEN_FOUND)
        set(DOXYFILE_IN  "${SPHINX_SRC}/Doxyfile.in")
        set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")
        if(EXISTS "${DOXYFILE_IN}")
            configure_file("${DOXYFILE_IN}" "${DOXYFILE_OUT}" @ONLY)
            add_custom_target(doc_doxygen
                COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
                BYPRODUCTS "${DOXYGEN_OUTPUT_DIR}/xml/index.xml"
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                COMMENT "Generating Doxygen XML for Breathe integration"
                VERBATIM)
        else()
            message(WARNING "Doxyfile.in not found at ${DOXYFILE_IN}. Skipping Doxygen.")
            add_custom_target(doc_doxygen COMMAND ${CMAKE_COMMAND} -E echo "Doxygen skipped.")
        endif()
    else()
        message(WARNING "Doxygen not found. Skipping C++ API extraction.")
        add_custom_target(doc_doxygen COMMAND ${CMAKE_COMMAND} -E echo "Doxygen skipped.")
    endif()

    # Fail gracefully if Sphinx or modules are missing
    if(NOT SPHINX_EXECUTABLE OR NOT Python3_Interpreter_FOUND OR _vmm_module_check)
        message(WARNING
            "Sphinx documentation disabled.\n"
            "Missing: ${_vmm_missing_modules}\n"
            "Tip: Run 'pip install sphinx breathe myst-parser sphinx-design' in your venv.")
        add_custom_target(doc_sphinx COMMAND ${CMAKE_COMMAND} -E echo "Sphinx disabled.")
        add_custom_target(doc ALL DEPENDS doc_sphinx)
    else()
        # Use python -m sphinx for consistency across environments
        set(_vmm_sphinx_cmd ${Python3_EXECUTABLE} -m sphinx)

        add_custom_target(doc_sphinx
            # 1. Ensure build directories exist
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPHINX_BUILD}/html"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DOCS_PUBLISH_DIR}"

            # 2. Generate HTML with Doxygen XML available to Breathe
            COMMAND ${CMAKE_COMMAND} -E env
                    VMM_DOXY_XML="${DOXYGEN_OUTPUT_DIR}/xml"
                    ${_vmm_sphinx_cmd} -b html "${SPHINX_SRC}" "${SPHINX_BUILD}/html"

            # 3. Publish to source tree for GitHub Pages / local preview
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${DOCS_PUBLISH_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SPHINX_BUILD}/html" "${DOCS_PUBLISH_DIR}"

            DEPENDS doc_doxygen
            WORKING_DIRECTORY "${SPHINX_SRC}"
            COMMENT "Generating and publishing Sphinx documentation to ${DOCS_PUBLISH_DIR}"
            VERBATIM)

        add_custom_target(doc ALL
            DEPENDS doc_sphinx
            COMMENT "Building full documentation (Doxygen + Sphinx)")
    endif()

    # Debug helper
    add_custom_target(docs_debug
        COMMAND ${CMAKE_COMMAND} -E echo "=== Documentation Configuration ==="
        COMMAND ${CMAKE_COMMAND} -E echo "SPHINX_SRC        : ${SPHINX_SRC}"
        COMMAND ${CMAKE_COMMAND} -E echo "SPHINX_BUILD      : ${SPHINX_BUILD}"
        COMMAND ${CMAKE_COMMAND} -E echo "SPHINX_EXECUTABLE : ${SPHINX_EXECUTABLE}"
        COMMAND ${CMAKE_COMMAND} -E echo "DOCS_PUBLISH_DIR  : ${DOCS_PUBLISH_DIR}"
        COMMAND ${CMAKE_COMMAND} -E echo "DOXYGEN_OUTPUT_DIR: ${DOXYGEN_OUTPUT_DIR}"
        COMMENT "Print documentation environment variables"
        VERBATIM)

    # Clean build artifacts
    add_custom_target(distclean
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}"
        COMMENT "Removing all build artifacts"
        VERBATIM)
endif()