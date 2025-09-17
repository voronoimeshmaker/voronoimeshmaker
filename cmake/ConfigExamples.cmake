# ------------------------------------------------------------
# Examples configuration
# ------------------------------------------------------------

macro(add_examples_in_directory dir)
  file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
       RELATIVE "${dir}" "${dir}/*.cpp")
  if(EXAMPLE_SOURCES)
    foreach(example_source ${EXAMPLE_SOURCES})
      get_filename_component(example_name ${example_source} NAME_WE)
      set(example_full_path "${dir}/${example_source}")
      get_filename_component(example_dir "${example_full_path}" DIRECTORY)

      add_executable(${example_name} "${example_full_path}")
      target_link_libraries(${example_name} PRIVATE VoronoiMeshMaker)
      target_include_directories(${example_name} PRIVATE
          ${VMMLib_DIR}/include)

      set_target_optimizations(${example_name})

      set_target_properties(${example_name} PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY "${example_dir}")
    endforeach()
  endif()
endmacro()

if(BUILD_EXAMPLES)
  message(STATUS "Building Examples...")
  add_subdirectory(examples)
endif()