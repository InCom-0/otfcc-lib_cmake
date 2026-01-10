add_library(otfcc_lib_dep STATIC)

set(SRC_otfcc_lib_dep json-builder/src/json-builder.c json-parser/src/json.c sds/src/sds.c sds/src/sdsalloc.h)
set_source_files_properties(
    ${SRC_otfcc_lib_dep}
    PROPERTIES LANGUAGE C
)
target_sources(otfcc_lib_dep PRIVATE ${SRC_otfcc_lib_dep})

target_sources(otfcc_lib_dep
    PUBLIC
    FILE_SET pub_headers
    TYPE HEADERS
    BASE_DIRS
    json-builder/include
    json-parser/include
    sds/include
    uthash/include
)

#####################################################################
### Working around getopt.h absence on MSVC STL ###
#####################################################################
target_sources(otfcc_lib_dep
    PUBLIC
    FILE_SET pub_headers_getopt
    TYPE HEADERS
    BASE_DIRS
    $<$<BOOL:${USING_MSVC_STL}>:${CMAKE_CURRENT_SOURCE_DIR}/polyfill-msvc/include>
)

if(USING_MSVC_STL)
    target_sources(otfcc_lib_dep PRIVATE polyfill-msvc/getopt.c)
endif()



