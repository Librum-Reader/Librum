# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)


# Build tests
add_executable(application_unit_tests
                    main.cpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    application
                    domain
                    Qt6::Core
                    gtest
                    gmock
    )

target_include_directories(application_unit_tests
                PRIVATE
                    ${APPLICATION_INCLUDES}
                    ${DOMAIN_INCLUDES}
)