cmake_minimum_required(VERSION 3.8)

project(xsens_xme_sdk)

find_package(ament_cmake REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# --------------------------------------------------------------------------
# Link-stub generation
# --------------------------------------------------------------------------
# The vendor-shipped libxme.so has stripped ELF section headers (anti-tamper
# protection).  The static linker needs section headers to resolve symbols,
# but the dynamic linker (runtime) does not.
#
# Instead of patching the vendor binary we generate a thin stub library that
# exports the same symbols.  Downstream packages link against the stub at
# build time; the real vendor library is loaded at runtime via RPATH.
# --------------------------------------------------------------------------

set(_XME_REAL "${CMAKE_CURRENT_SOURCE_DIR}/lib/libxme.so.2026.2.0")
set(_XME_STUB_DIR "${CMAKE_CURRENT_BINARY_DIR}/stubs")
set(_XME_STUB_C "${_XME_STUB_DIR}/xme_link_stubs.c")
set(_XME_STUB_SO "${_XME_STUB_DIR}/libxme.so")

file(MAKE_DIRECTORY ${_XME_STUB_DIR})

# Generate the stub C source from the vendor library's dynamic symbol table.
execute_process(
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_link_stubs.py
    ${_XME_REAL} ${_XME_STUB_C}
  RESULT_VARIABLE _gen_result
)
if(_gen_result)
  message(FATAL_ERROR "Failed to generate link stubs from ${_XME_REAL}")
endif()

# Compile the stub into a shared library with the correct SONAME.
execute_process(
  COMMAND ${CMAKE_C_COMPILER}
    -shared -fPIC -o ${_XME_STUB_SO}
    ${_XME_STUB_C}
    -Wl,-soname,libxme.so.2026
  RESULT_VARIABLE _cc_result
)
if(_cc_result)
  message(FATAL_ERROR "Failed to compile link stub library")
endif()

# Create an interface library target for the vendor package
add_library(xsens_xme_sdk INTERFACE)

# Set include directories for build and install
target_include_directories(xsens_xme_sdk INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Link against the stub for libxme (searched first) plus the normal vendor libs.
# The stub dir contains a libxme.so with proper section headers for the linker.
# At runtime the real vendor libxme.so is found via RPATH in the lib/ directory.
target_link_directories(xsens_xme_sdk INTERFACE
  $<BUILD_INTERFACE:${_XME_STUB_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
  $<INSTALL_INTERFACE:lib/linkstubs>
  $<INSTALL_INTERFACE:lib>
)

target_link_libraries(xsens_xme_sdk INTERFACE
  $<BUILD_INTERFACE:xme;xsensdeviceapi;xstypes;xkf3hm_filter;xee_solver>
  $<INSTALL_INTERFACE:xme;xsensdeviceapi;xstypes;xkf3hm_filter;xee_solver>
)

# Install headers
install(
  DIRECTORY include/
  DESTINATION include
)

# Install shared libraries (the real vendor binaries)
install(
  DIRECTORY lib/
  DESTINATION lib
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN "*.so*"
)

# Install the link stub so downstream packages can resolve symbols at build time.
install(
  FILES ${_XME_STUB_SO}
  DESTINATION lib/linkstubs
)

# Install data files
install(
  FILES
    lib/xmedef.xsb
    lib/xmedef_male.xsb
    lib/xmedef_female.xsb
    lib/xmedef_syncstation.xsb
    lib/npose.xsa
    lib/npose-male.xsa
    lib/npose-female.xsa
    lib/tpose.xsa
    lib/tpose-male.xsa
    lib/tpose-female.xsa
  DESTINATION share/${PROJECT_NAME}
)

# Install the target and export it
install(
  TARGETS xsens_xme_sdk
  EXPORT export_xsens_xme_sdk
)

# Export the target for downstream packages
ament_export_targets(export_xsens_xme_sdk HAS_LIBRARY_TARGET)
ament_export_dependencies()

ament_package()
