cmake_minimum_required(VERSION 3.30...4.0)

project(otfcc-lib
  HOMEPAGE_URL "https://github.com/InCom-0/otfcc-lib_cmake"
  DESCRIPTION
  "Fork of otfcc, keeping only the otfcc-lib library portion of the project (and its dependencies). Modern CMake enabled. Made cross-platform."
  LANGUAGES C CXX)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

#####################################################################
### Platform specific hacks ###
#####################################################################
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
  add_compile_options(/utf-8)
  add_compile_definitions(NOMINMAX)
endif()


########################################################
### Target specification ###
########################################################

add_library(otfcc-lib)
add_library(otfcc-lib::otfcc-lib ALIAS otfcc-lib)

file(GLOB_RECURSE SRC_otfcc-lib src/**.c)
set_source_files_properties(
  ${SRC_otfcc-lib}
  PROPERTIES LANGUAGE C
)
target_sources(otfcc-lib PRIVATE ${SRC_otfcc-lib})

target_sources(otfcc-lib
  PRIVATE
  FILE_SET priv_headers_insource
  TYPE HEADERS
  BASE_DIRS
  src)

target_sources(otfcc-lib
  PUBLIC
  FILE_SET pub_headers
  TYPE HEADERS
  BASE_DIRS
  incl)

target_compile_features(otfcc-lib PUBLIC c_std_11)

add_subdirectory(dep)
target_link_libraries(otfcc-lib PUBLIC dep-lib)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(otfcc-lib PRIVATE -Wno-multichar)
endif()
