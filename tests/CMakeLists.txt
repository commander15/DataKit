enable_testing()

find_package(GTest REQUIRED)

add_executable(DataKitTest
    main.cpp
)

target_compile_definitions(DataKitTest PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/testdata")

target_link_libraries(DataKitTest PRIVATE GTest::gtest Qt::Test DataKit)
