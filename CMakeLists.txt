cmake_minimum_required(VERSION 3.30)

project(DataKit
    DESCRIPTION "Data handling toolkit for Qt"
    VERSION     "1.0.0"
    LANGUAGES   CXX C
)

# Project settings

option(DATAKIT_BUILD_TESTS        OFF "Build DataKit tests")
option(DATAKIT_BUILD_DOCS         OFF "Build DataKit documentation")
option(DATAKIT_BUILD_TRANSLATIONS OFF "Build DataKit translations")
option(DATAKIT_BUILD_EXAMPLES     OFF "Build DataKit Examples")

set(DATAKIT_LOCALES en fr)

# CMake settings
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# CXX Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Registering project module path
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Including utility scripts
include(cmake/Utils.cmake)

# Qt setup

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt_MODULES Core)

if (DATAKIT_BUILD_TESTS)
    list(APPEND Qt_MODULES Test)
endif()

if (DATAKIT_BUILD_TRANSLATIONS)
    list(APPEND Qt_MODULES LinguistTools)
endif()

find_package(Qt6 REQUIRED COMPONENTS ${Qt_MODULES})

qt_standard_project_setup(
    REQUIRES 6.8.0
    I18N_SOURCE_LANGUAGE      en
    I18N_TRANSLATED_LANGUAGES ${DATAKIT_LOCALES}
)

qt_policy(SET QTP0002 NEW)
qt_policy(SET QTP0003 NEW)

# Other dependencies

# Project directories

add_subdirectory(src)

if (DATAKIT_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (DATAKIT_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if (DATAKIT_BUILD_TRANSLATIONS)
    qt_collect_translation_source_targets(SOURCE_TARGETS)

    qt_add_translations(DataKit SOURCE_TARGETS ${SOURCE_TARGETS}
        TS_FILE_BASE datakit
        TS_FILE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/translations
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )
    
    # Ensure the build directory has a translations folder
    set(TRANSLATIONS_BUILD_DIR "${CMAKE_BINARY_DIR}/translations")
    file(MAKE_DIRECTORY ${TRANSLATIONS_BUILD_DIR})

    # Copy QM files to build directory after they are generated
    add_custom_command(
        TARGET DataKit_lrelease POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} ${TRANSLATIONS_BUILD_DIR}
        COMMENT "Copying generated QM files to ${TRANSLATIONS_BUILD_DIR}"
    )

    install(FILES ${QM_FILES} DESTINATION translations)
endif()

if (DATAKIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
