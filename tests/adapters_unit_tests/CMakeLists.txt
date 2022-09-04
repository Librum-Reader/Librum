find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
                    authentication_controller_tests.hpp
    )

target_link_libraries(adapters_unit_tests
                PRIVATE
                    gtest
                    gmock
                    adapters
                    application
                    domain
                    Qt5::Core
    )

target_include_directories(adapters_unit_tests
                PRIVATE
                    ${ADAPTERS_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
)