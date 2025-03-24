qt_add_library(DataKit)

set_target_properties(DataKit PROPERTIES
    OUTPUT_NAME DataKit
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(DataKit
    PUBLIC
        datakit.h
        global.h config.h
        data.h
        etl.h
    PRIVATE
        debug_p.h
        data_p.h
        etl_p.h
)

target_sources(DataKit
    PRIVATE
        debug.cpp
        data.cpp
        etl.cpp
)

target_include_directories(DataKit 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/DataKit>
)

target_compile_definitions(DataKit
    PUBLIC
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>,DATAKIT_STATIC,DATAKIT_SHARED>
    INTERFACE
        DATAKIT_LIB
    PRIVATE
        DATAKIT_BUILD
)

target_link_libraries(DataKit PUBLIC Qt::Core)

install(TARGETS DataKit
    EXPORT DataKitTargets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/DataKit
    #PRIVATE_HEADER DESTINATION include/DataKit/private
)

generate_target_headers(DataKit FOLDER DataKit)
