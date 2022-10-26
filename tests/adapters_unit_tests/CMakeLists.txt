find_package(QT NAMES Qt5 COMPONENTS Network Core Test Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Test Quick REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
                    controllers/book_controller_tests.hpp
                    controllers/authentication_controller_tests.hpp
                    controllers/user_controller_tests.hpp
                    gateways/user_storage_gateway_tests.hpp
                    gateways/book_storage_gateway_tests.hpp
                    gateways/authentication_gateway_tests.hpp
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
                    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/gateways
)