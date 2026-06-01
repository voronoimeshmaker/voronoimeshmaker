------------------------------------------------------------
Utility functions and macros
------------------------------------------------------------
macro(block_in_source_builds)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
set(_has_build_files FALSE)
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeCache.txt" OR
EXISTS "${CMAKE_SOURCE_DIR}/cmake_install.cmake" OR
EXISTS "${CMAKE_SOURCE_DIR}/Makefile" OR
EXISTS "${CMAKE_SOURCE_DIR}/CMakeFiles")
set(_has_build_files TRUE)
endif()
    if(_has_build_files)
        message(FATAL_ERROR
             "\n❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌\n "
             "🚫 [PT] ERRO CRÍTICO: Build no diretório fonte detectado!\n "
             "💥 Arquivos de build existentes precisam ser removidos MANUALMENTE:\n\n "
             "   rm -f CMakeCache.txt cmake_install.cmake Makefile\n "
             "   rm -rf CMakeFiles/\n\n "
             "✅ DEPOIS execute corretamente:\n "
             "   mkdir -p build\n "
             "   cd build\n "
             "   cmake ..\n\n "
             "🚫 [EN] CRITICAL ERROR: In-source build detected!\n "
             "💥 Existing build files must be removed MANUALLY:\n\n "
             "   rm -f CMakeCache.txt cmake_install.cmake Makefile\n "
             "   rm -rf CMakeFiles/\n\n "
             "✅ THEN run correctly:\n "
             "   mkdir -p build\n "
             "   cd build\n "
             "   cmake ..\n "
             "❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌\n "
        )
    else()
        message(FATAL_ERROR
             "\n❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌\n "
             "🚫 [PT] ERRO: Build no diretório fonte não é permitido!\n "
             "✅ Execute corretamente:\n "
             "   mkdir -p build\n "
             "   cd build\n "
             "   cmake ..\n\n "
             "🚫 [EN] ERROR: In-source builds are not allowed!\n "
             "✅ Run correctly:\n "
             "   mkdir -p build\n "
             "   cd build\n "
             "   cmake ..\n "
             "❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌\n "
        )
    endif()
endif()
endmacro()
Call it immediately
block_in_source_builds()
