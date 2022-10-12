find_package(QT NAMES Qt5 COMPONENTS Network Core Test Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Test Quick REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
                    book_controller_tests.hpp
                    authentication_controller_tests.hpp
                    user_controller_tests.hpp
                    user_storage_gateway_tests.hpp
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
                    Qt5::Quick
                    Qt5::Test
    )

target_include_directories(adapters_unit_tests
                PRIVATE
                    ${ADAPTERS_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
                    ${PROJECT_SOURCE_DIR}
)