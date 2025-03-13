add_subdirectory(lib)

include(CMakePackageConfigHelpers)

set(ROOT .)

configure_package_config_file(DataKitConfig.cmake.in DataKitConfig.cmake
    PATH_VARS ROOT
    INSTALL_DESTINATION lib/cmake/DataKit
)

write_basic_package_version_file(DataKitConfigVersion.cmake
    VERSION ${DataKit_VERSION}
    COMPATIBILITY AnyNewerVersion
    ARCH_INDEPENDENT
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DataKitConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DataKitConfigVersion.cmake
    DESTINATION lib/cmake/DataKit
)

install(EXPORT DataKitTargets
    NAMESPACE DataKit::
    DESTINATION lib/cmake/DataKit
)
