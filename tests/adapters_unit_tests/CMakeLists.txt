find_package(QT NAMES Qt5 COMPONENTS Network Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
    )

target_link_libraries(adapters_unit_tests
                PRIVATE
                    gtest
                    gmock
                    adapters
                    application
                    infrastructure
                    domain
                    Qt5::Core
                    Qt5::Network
    )

target_include_directories(adapters_unit_tests
                PRIVATE
                    ${ADAPTERS_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
                    ${PROJECT_SOURCE_DIR}
)

target_compile_options(adapters_unit_tests PRIVATE -g3 -Og)