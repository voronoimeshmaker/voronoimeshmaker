# ------------------------------------------------------------
# Dependencies configuration
# ------------------------------------------------------------

# CGAL
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(CGAL REQUIRED COMPONENTS Core)

# yaml-cpp (optional)
find_package(yaml-cpp QUIET)
if(yaml-cpp_FOUND)
  message(STATUS "yaml-cpp found: enabling YAML-based examples")
else()
  message(STATUS "yaml-cpp NOT found: YAML-based examples will be skipped")
endif()