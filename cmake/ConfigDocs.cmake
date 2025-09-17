# cmake/ConfigDocs.cmake
# ------------------------------------------------------------
# Documentation configuration (Doxygen + Sphinx) - IMPROVED
# ------------------------------------------------------------

if(BUILD_DOCS)
    message(STATUS "Configuring documentation...")
    
    # Sphinx source and build dirs
    set(SPHINX_SRC   "${CMAKE_SOURCE_DIR}/docs_sphinx")
    set(SPHINX_BUILD "${CMAKE_BINARY_DIR}/docs_sphinx")
    set(DOCS_PUBLISH_DIR "${CMAKE_SOURCE_DIR}/documentation" CACHE PATH
        "Directory in source tree to publish generated HTML docs")

    # Try to install requirements
    if(Python3_Interpreter_FOUND)
        if(EXISTS "${CMAKE_SOURCE_DIR}/requirements-docs.txt")
            execute_process(
                COMMAND ${Python3_EXECUTABLE} -m pip install -r ${CMAKE_SOURCE_DIR}/requirements-docs.txt
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE pip_result
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if(NOT pip_result EQUAL 0)
                message(WARNING "Failed to install documentation dependencies")
            endif()
        endif()
    endif()

    # Doxygen configuration
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
        set(DOXYFILE_IN  "${SPHINX_SRC}/Doxyfile.in")
        set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

        if(EXISTS "${DOXYFILE_IN}")
            configure_file("${DOXYFILE_IN}" "${DOXYFILE_OUT}" @ONLY)
            add_custom_target(doc_doxygen
                COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
                BYPRODUCTS "${DOXYGEN_OUTPUT_DIR}/xml/index.xml"
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                COMMENT "Generating Doxygen XML"
                VERBATIM)
        else()
            message(WARNING "Doxyfile.in not found at ${DOXYFILE_IN}")
            add_custom_target(doc_doxygen
                COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not available: skipping XML generation.")
            set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
        endif()
    else()
        message(WARNING "Doxygen not found")
        add_custom_target(doc_doxygen
            COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not available: skipping XML generation.")
        set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
    endif()

    # Sphinx detection
    if(NOT DEFINED SPHINX_EXECUTABLE)
        set(SPHINX_EXECUTABLE "" CACHE FILEPATH "Path to sphinx-build executable")
    endif()
    
    if(NOT SPHINX_EXECUTABLE)
        if(DEFINED ENV{VIRTUAL_ENV})
            if(WIN32)
                set(_cand "$ENV{VIRTUAL_ENV}/Scripts/sphinx-build.exe")
            else()
                set(_cand "$ENV{VIRTUAL_ENV}/bin/sphinx-build")
            endif()
            if(EXISTS "${_cand}")
                set(SPHINX_EXECUTABLE "${_cand}" CACHE FILEPATH "sphinx-build from active venv" FORCE)
            endif()
        endif()
    endif()
    
    if(NOT SPHINX_EXECUTABLE)
        if(WIN32)
            set(_cand "${CMAKE_SOURCE_DIR}/.venv/Scripts/sphinx-build.exe")
        else()
            set(_cand "${CMAKE_SOURCE_DIR}/.venv/bin/sphinx-build")
        endif()
        if(EXISTS "${_cand}")
            set(SPHINX_EXECUTABLE "${_cand}" CACHE FILEPATH "sphinx-build from .venv" FORCE)
        endif()
    endif()
    
    if(NOT SPHINX_EXECUTABLE)
        find_program(SPHINX_EXECUTABLE sphinx-build)
    endif()

    # Prefer `python -m sphinx`
    find_package(Python3 COMPONENTS Interpreter QUIET)

    # Sphinx guard - check Python modules
    set(SPHINX_PY_MODULES_DEFAULT "breathe;myst_parser;sphinx_design")
    set(SPHINX_PY_MODULES "${SPHINX_PY_MODULES_DEFAULT}"
        CACHE STRING "Python modules required for the docs")

    set(_SPHINX_IMPORT_ERR 0)
    set(_SPHINX_IMPORT_MISSING "")

    if(Python3_Interpreter_FOUND)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c
                "import importlib.util,sys; \
                missing=[m for m in sys.argv[1:] if not importlib.util.find_spec(m)]; \
                print(';'.join(missing)); \
                sys.exit(1 if missing else 0)"
                ${SPHINX_PY_MODULES}
            RESULT_VARIABLE _SPHINX_IMPORT_ERR
            OUTPUT_VARIABLE _SPHINX_IMPORT_MISSING
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()

    if(NOT SPHINX_EXECUTABLE AND NOT Python3_Interpreter_FOUND)
        message(WARNING "sphinx-build/Python3 not found. Sphinx docs will be skipped.")
    endif()

    if(_SPHINX_IMPORT_ERR)
        message(WARNING
            "Sphinx disabled: missing Python modules: ${_SPHINX_IMPORT_MISSING}\n"
            "Tip: activate a venv and 'pip install sphinx breathe myst-parser sphinx-design'")

        add_custom_target(doc_sphinx
            COMMAND ${CMAKE_COMMAND} -E echo
                "Skipping Sphinx (missing: ${_SPHINX_IMPORT_MISSING})."
            COMMENT "Sphinx (DISABLED): missing Python modules."
            VERBATIM)

        add_custom_target(doc ALL
            DEPENDS doc_sphinx
            COMMENT "Build documentation (stub; Sphinx disabled)")

    else()
        # Sphinx target (depends on Doxygen if available)
        if(NOT TARGET doc_doxygen)
            add_custom_target(doc_doxygen
                COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not available: skipping XML generation.")
            set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs_doxygen")
        endif()

        if(Python3_Interpreter_FOUND)
            set(_SPHINX_CMD ${Python3_EXECUTABLE} -m sphinx)
        else()
            set(_SPHINX_CMD "${SPHINX_EXECUTABLE}")
        endif()

        # ✅ TARGET CORRIGIDO - COMANDOS SEPARADOS
        add_custom_target(doc_sphinx
            # 1. Criar diretório de build
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SPHINX_BUILD}/html"
            
            # 2. Executar Sphinx (gera HTML) - COM VARIÁVEL DE AMBIENTE
            COMMAND ${CMAKE_COMMAND} -E env VMM_DOXY_XML=${DOXYGEN_OUTPUT_DIR}/xml
                    ${_SPHINX_CMD} -b html
                    "${SPHINX_SRC}" "${SPHINX_BUILD}/html"
            
            # 3. Limpar diretório de publicação
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${DOCS_PUBLISH_DIR}"
            
            # 4. Criar diretório de publicação
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DOCS_PUBLISH_DIR}"
            
            # 5. Copiar arquivos gerados
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${SPHINX_BUILD}/html" "${DOCS_PUBLISH_DIR}"
            
            # 6. Mensagem de confirmação
            COMMAND ${CMAKE_COMMAND} -E echo "Documentation published to: ${DOCS_PUBLISH_DIR}"
            
            DEPENDS doc_doxygen
            WORKING_DIRECTORY "${SPHINX_SRC}"
            COMMENT "Generating Sphinx HTML docs and publishing to ${DOCS_PUBLISH_DIR}"
            VERBATIM)

        add_custom_target(doc ALL
            DEPENDS doc_sphinx
            COMMENT "Build full documentation (Doxygen + Sphinx)")
    endif()

    # Debug target para verificar configuração
    add_custom_target(docs_debug
        COMMAND ${CMAKE_COMMAND} -E echo "=== Documentation Debug ==="
        COMMAND ${CMAKE_COMMAND} -E echo "SPHINX_SRC: ${SPHINX_SRC}"
        COMMAND ${CMAKE_COMMAND} -E echo "SPHINX_BUILD: ${SPHINX_BUILD}"
        COMMAND ${CMAKE_COMMAND} -E echo "DOCS_PUBLISH_DIR: ${DOCS_PUBLISH_DIR}"
        COMMAND ${CMAKE_COMMAND} -E echo "DOXYGEN_OUTPUT_DIR: ${DOXYGEN_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E echo "VMM_DOXY_XML: ${DOXYGEN_OUTPUT_DIR}/xml"
        COMMENT "Print documentation configuration for debugging"
        VERBATIM)

    # Distclean target
    add_custom_target(distclean
        COMMAND ${CMAKE_COMMAND} -E rm -rf
            ${CMAKE_BINARY_DIR}/*
            ${CMAKE_BINARY_DIR}/CMakeFiles
            ${CMAKE_BINARY_DIR}/Makefile
            ${CMAKE_BINARY_DIR}/cmake_install.cmake
        COMMENT "Cleaning build artifacts")
endif()