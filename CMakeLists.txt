cmake_minimum_required(VERSION 3.30...4.0)

project(otfcc_cmake
  HOMEPAGE_URL "https://github.com/InCom-0/otfcc_cmake"
  DESCRIPTION "Fork of otfcc, adding the underlying library as 'otfcc_lib' target. Modern CMake enabled. Made cross-platform."
  LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(otfcc_BUILD_SHARED_LIB "Build a shared version of otfcc_lib" ${BUILD_SHARED_LIBS})
option(otfcc_STAGE_OUTPUTS "Stage build output artifacts into proper directory structure" ON)

option(otfcc_BUILD_TOOLS "Build CLI tools" ${PROJECT_IS_TOP_LEVEL})
option(otfcc_BUILD_OTFCCDUMP "Build CLI tool: otfccdump" ${otfcc_BUILD_TOOLS})
option(otfcc_BUILD_OTFCCBUILD "Build CLI tool: otfccbuild" ${otfcc_BUILD_TOOLS})

include(cmake/stdlibQuery.cmake)
if(USING_LIBSTDCXX)
  message(STATUS "Using libstdc++\n")
elseif(USING_LIBCXX)
  message(STATUS "Using libc++\n")
elseif(USING_MSVC_STL)
  message(STATUS "Using MSVC STL\n")
endif()


#####################################################################
### Staging ###
#####################################################################
include(cmake/StageOutputs.cmake)
if(incplot-lib_STAGE_OUTPUTS)
  stageOutputDirsFor(incplot-lib)
endif()


#####################################################################
### Platform specific hacks ###
#####################################################################
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
  set(otfcc_BUILD_SHARED_LIB OFF)
  message(STATUS "otfcc_lib requires to be build as STATIC when used with MSVC STL.")
  message(STATUS "Forcing otfcc_lib to be built as STATIC library.\n")

  add_compile_options(/utf-8)
  add_compile_definitions(NOMINMAX)

  set_source_files_properties(
    src/lib/vf/vq.c
    PROPERTIES
    COMPILE_OPTIONS "/wd4098;/wd4715"
  )
  set_source_files_properties(
    src/lib/table/_TSI.c
    PROPERTIES
    COMPILE_OPTIONS "/wd4715"
  )
endif()

########################################################
### Target specification - otfcc_lib ###
########################################################
if(otfcc_BUILD_SHARED_LIB)
  add_library(otfcc_lib SHARED)
else()
  add_library(otfcc_lib STATIC)
endif()
add_library(otfcc_lib::otfcc_lib ALIAS otfcc_lib)

file(GLOB_RECURSE SRC_otfcc_lib src/lib/**.c)
set_source_files_properties(
  ${SRC_otfcc_lib}
  PROPERTIES LANGUAGE C
)
target_sources(otfcc_lib PRIVATE ${SRC_otfcc_lib})
target_sources(otfcc_lib
  PRIVATE
  FILE_SET priv_headers_insource
  TYPE HEADERS
  BASE_DIRS
  src/lib)

target_sources(otfcc_lib
  PUBLIC
  FILE_SET pub_headers
  TYPE HEADERS
  BASE_DIRS
  incl)

add_subdirectory(dep)
target_link_libraries(otfcc_lib PUBLIC otfcc_lib_dep)

target_compile_features(otfcc_lib PRIVATE c_std_11)
set_target_properties(otfcc_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(otfcc_lib PRIVATE -Wno-multichar)
endif()


########################################################
### Target specification - otfccdump ###
########################################################
if(otfcc_BUILD_OTFCCDUMP)

  add_executable(otfccdump)
  target_sources(otfccdump PRIVATE src/tools/otfccdump.c src/tools/stopwatch.c)

  target_sources(otfccdump
    PRIVATE
    FILE_SET priv_headers
    TYPE HEADERS
    BASE_DIRS
    src/tools)

  target_link_libraries(otfccdump PRIVATE otfcc_lib)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(otfccdump PRIVATE -Wno-multichar)
  endif()

endif(otfcc_BUILD_OTFCCDUMP)

########################################################
### Target specification - otfccbuild ###
########################################################
if(otfcc_BUILD_OTFCCBUILD)
  add_executable(otfccbuild)
  target_sources(otfccbuild PRIVATE src/tools/otfccbuild.c src/tools/stopwatch.c)

  target_sources(otfccbuild
    PRIVATE
    FILE_SET priv_headers
    TYPE HEADERS
    BASE_DIRS
    src/tools)

  target_link_libraries(otfccbuild PRIVATE otfcc_lib)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(otfccbuild PRIVATE -Wno-multichar)
  endif()

endif(otfcc_BUILD_OTFCCBUILD)



