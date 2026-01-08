cmake_minimum_required(VERSION 3.30...4.0)

project(otfcc_lib
  HOMEPAGE_URL "https://github.com/InCom-0/otfcc_cmake"
  DESCRIPTION "Fork of otfcc, adding the underlying library as 'otfcc_lib' target. Modern CMake enabled. Made cross-platform."
  LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)


include(cmake/stdlibQuery.cmake)
if(USING_LIBSTDCXX)
  message(STATUS "Using libstdc++")
elseif(USING_LIBCXX)
  message(STATUS "Using libc++")
elseif(USING_MSVC_STL)
  message(STATUS "Using MSVC STL")
endif()


# If we are using MSVC_STL then the default is not to build the tools (because they can't be built)
# If the user explicitly requests them, then the configure step will error out with sensible message (see below) 
if((NOT OTFCC_LIB_BUILD_OTFCCDUMP) AND (NOT OTFCC_LIB_BUILD_TOOLS) AND USING_MSVC_STL)
  set(OTFCC_LIB_BUILD_OTFCCDUMP OFF)
endif()

if((NOT OTFCC_LIB_BUILD_OTFCCBUILD) AND (NOT OTFCC_LIB_BUILD_TOOLS) AND USING_MSVC_STL)
  set(OTFCC_LIB_BUILD_OTFCCBUILD OFF)
endif()

option(OTFCC_LIB_BUILD_TOOLS "Build CLI tools" ${PROJECT_IS_TOP_LEVEL})
option(OTFCC_LIB_BUILD_OTFCCDUMP "Build CLI tool: otfccdump" ${OTFCC_LIB_BUILD_TOOLS})
option(OTFCC_LIB_BUILD_OTFCCBUILD "Build CLI tool: otfccbuild" ${OTFCC_LIB_BUILD_TOOLS})


#####################################################################
### Platform specific hacks ###
#####################################################################
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
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

add_library(otfcc_lib)
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

target_compile_features(otfcc_lib PRIVATE c_std_11)

add_subdirectory(dep)
target_link_libraries(otfcc_lib PUBLIC otfcc_lib_dep)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(otfcc_lib PRIVATE -Wno-multichar)
endif()

########################################################
### Target specification - otfccdump ###
########################################################
if(OTFCC_LIB_BUILD_OTFCCDUMP)
  if(USING_MSVC_STL)
    message(FATAL_ERROR
      "It is impossible to build otfccdump using MSVC STL.\n"
      "This is expected behavior of otfcc_cmake.\n"
      "Building would require getopt.h which MSVC STL does not possess.\n"
      "Possible solutions: 1) Don't build otfccdump, 2) Use stdlibc++ or libc++ on Windows (through MSYS2) isntead of MSVC STL, 3) Provide some working getopt.h through some third party sources (which exist but are not included)."
    )
  endif()

  add_executable(otfccdump)
  target_sources(otfccdump PRIVATE src/tools/otfccdump.c src/tools/stopwatch.c)

  target_sources(otfccdump
    PRIVATE
    FILE_SET priv_headers
    TYPE HEADERS
    BASE_DIRS
    src/tools)

  target_link_libraries(otfccdump PRIVATE otfcc_lib)

  target_compile_features(otfccdump PRIVATE c_std_11)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(otfccdump PRIVATE -Wno-multichar)
  endif()

endif(OTFCC_LIB_BUILD_OTFCCDUMP)

########################################################
### Target specification - otfccbuild ###
########################################################
if(OTFCC_LIB_BUILD_OTFCCBUILD)
  if(USING_MSVC_STL)
    message(FATAL_ERROR
      "It is impossible to build otfccbuild using MSVC STL.\n"
      "This is expected behavior of otfcc_cmake.\n"
      "Building would require getopt.h which MSVC STL does not possess.\n"
      "Possible solutions: 1) Don't build otfccbuild, 2) Use stdlibc++ or libc++ on Windows (through MSYS2) isntead of MSVC STL, 3) Provide some working getopt.h through a some third party sources (which exist but are not included)."
    )
  endif()
  add_executable(otfccbuild)
  target_sources(otfccbuild PRIVATE src/tools/otfccbuild.c src/tools/stopwatch.c)

  target_sources(otfccbuild
    PRIVATE
    FILE_SET priv_headers
    TYPE HEADERS
    BASE_DIRS
    src/tools)

  target_link_libraries(otfccbuild PRIVATE otfcc_lib)

  target_compile_features(otfccbuild PRIVATE c_std_11)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(otfccbuild PRIVATE -Wno-multichar)
  endif()

endif(OTFCC_LIB_BUILD_OTFCCBUILD)



