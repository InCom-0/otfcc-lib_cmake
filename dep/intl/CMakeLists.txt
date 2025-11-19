add_library(intl-lib INTERFACE)

target_sources(intl-lib
    PUBLIC
    FILE_SET pub_headers
    TYPE HEADERS
    BASE_DIRS
    include
)
