# ------------------------------------------------------------
# Compiler settings and optimizations
# ------------------------------------------------------------

# Global include directories
include_directories(${VMMLib_DIR}/include)

# Function to set compiler optimizations
function(set_target_optimizations target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -O3 -march=native -flto)
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /O2 /GL)
    endif()
endfunction()