qt_add_library(DataKit)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(DataKit
    PUBLIC
        global.h config.h
        data.h
        etl.h
    PRIVATE
        etl_p.h
)

target_sources(DataKit
    PRIVATE
        data.cpp
        etl.cpp
)

target_include_directories(DataKit
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(DataKit PUBLIC Qt::Core)

generate_target_headers(DataKit FOLDER DataKit)

install(TARGETS DataKit
    EXPORT DataKitTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/DataKit
)
