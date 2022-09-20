find_package(QT NAMES Qt5 COMPONENTS Network Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
                    book_controller_tests.hpp
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
                    Qt5::Network
    )

target_include_directories(adapters_unit_tests
                PRIVATE
                    ${ADAPTERS_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
                    ${PROJECT_SOURCE_DIR}
)