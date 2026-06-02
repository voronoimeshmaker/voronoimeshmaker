# cmake/ConfigCompiler.cmake

add_library(vmm_project_options INTERFACE)
add_library(VoronoiMeshMaker::project_options ALIAS vmm_project_options)

add_library(vmm_project_warnings INTERFACE)
add_library(VoronoiMeshMaker::project_warnings ALIAS vmm_project_warnings)

target_compile_features(vmm_project_options
    INTERFACE
        cxx_std_${VMM_CXX_STANDARD}
)

set(_vmm_optimised_config
    $<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>
)

set(_vmm_debug_config
    $<CONFIG:Debug>
)

set(_vmm_gnu_or_clang
    $<CXX_COMPILER_ID:GNU,Clang>
)

set(_vmm_msvc
    $<CXX_COMPILER_ID:MSVC>
)

set(_vmm_optimised_gnu_or_clang
    $<AND:${_vmm_optimised_config},${_vmm_gnu_or_clang}>
)

set(_vmm_debug_gnu_or_clang
    $<AND:${_vmm_debug_config},${_vmm_gnu_or_clang}>
)

set(_vmm_optimised_msvc
    $<AND:${_vmm_optimised_config},${_vmm_msvc}>
)

target_compile_options(vmm_project_options
    INTERFACE
        $<${_vmm_optimised_gnu_or_clang}:-O3>
        $<${_vmm_optimised_gnu_or_clang}:-DNDEBUG>
        $<${_vmm_debug_gnu_or_clang}:-O0>
        $<${_vmm_debug_gnu_or_clang}:-g3>
        $<${_vmm_optimised_msvc}:/O2>
        $<${_vmm_optimised_msvc}:/DNDEBUG>
)

if(VMM_ENABLE_NATIVE_ARCH)
    target_compile_options(vmm_project_options
        INTERFACE
            $<${_vmm_optimised_gnu_or_clang}:-march=native>
            $<${_vmm_optimised_gnu_or_clang}:-mtune=native>
    )
endif()

if(VMM_ENABLE_FAST_MATH)
    message(WARNING
        "VMM_ENABLE_FAST_MATH is ON. "
        "This may change floating-point behaviour and can be unsafe for robust geometric predicates."
    )

    target_compile_options(vmm_project_options
        INTERFACE
            $<${_vmm_optimised_gnu_or_clang}:-ffast-math>
    )
endif()

if(VMM_ENABLE_LTO)
    include(CheckIPOSupported)

    check_ipo_supported(
        RESULT _vmm_ipo_supported
        OUTPUT _vmm_ipo_error
    )

    if(_vmm_ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    else()
        message(WARNING
            "IPO/LTO requested but not supported: ${_vmm_ipo_error}"
        )
    endif()
endif()

if(UNIX AND NOT APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(vmm_project_options
        INTERFACE
            $<${_vmm_optimised_gnu_or_clang}:-Wl,-O2>
            $<${_vmm_optimised_gnu_or_clang}:-Wl,--as-needed>
    )
endif()

if(VMM_ENABLE_WARNINGS)
    target_compile_options(vmm_project_warnings
        INTERFACE
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wpedantic>
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wshadow>
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wconversion>
            $<$<CXX_COMPILER_ID:GNU,Clang>:-Wsign-conversion>
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    )
endif()

if(VMM_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(vmm_project_options
            INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_link_options(vmm_project_options
            INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
    else()
        message(WARNING
            "VMM_ENABLE_SANITIZERS is ON, but automatic sanitizer flags are only configured for GNU and Clang."
        )
    endif()
endif()

function(vmm_apply_common_target_settings target_name)
    target_link_libraries("${target_name}"
        PRIVATE
            vmm_project_options
            vmm_project_warnings
    )
endfunction()