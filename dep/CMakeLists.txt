add_library(otfcc-lib_dep STATIC)

set(SRC_otfcc-lib_dep json-builder/src/json-builder.c json-parser/src/json.c sds/src/sds.c sds/src/sdsalloc.h)
set_source_files_properties(
    ${SRC_otfcc-lib_dep}
    PROPERTIES LANGUAGE C
)
target_sources(otfcc-lib_dep PRIVATE ${SRC_otfcc-lib_dep})

target_sources(otfcc-lib_dep
    PUBLIC
    FILE_SET pub_headers
    TYPE HEADERS
    BASE_DIRS
    json-builder/include
    json-parser/include
    sds/include
    uthash/include
    intl/include
)

add_subdirectory(nowide)
add_subdirectory(intl)
target_link_libraries(otfcc-lib_dep PUBLIC nowide-lib intl-lib)
