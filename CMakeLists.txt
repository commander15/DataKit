cmake_minimum_required(VERSION 3.20)

project(DataKit
    DESCRIPTION "Data handling toolkit for Qt"
    VERSION     1.0.0
    LANGUAGES   CXX C
)

option(DATAKIT_BUILD_DOCS     OFF "Build documentation")
option(DATAKIT_BUILD_EXAMPLES OFF "Build Examples")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Core dependencies
find_package(Qt REQUIRED COMPONENTS Core)

# Excel plugin dependencies
if (DATAKIT_SUPPORT_EXCEL)
    find_package(QXlsxQt6 REQUIRED)
endif()

include(cmake/Utils.cmake)

add_subdirectory(src)

if (DATAKIT_BUILD_DOCS)
    add_subdirectory(docs)
endif()

#add_subdirectory(tests)

if (DATAKIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
