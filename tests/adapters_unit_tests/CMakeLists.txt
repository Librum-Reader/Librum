find_package(QT NAMES Qt5 COMPONENTS Network Core Test Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Test Quick REQUIRED)



# Build tests
add_executable(adapters_unit_tests
                    main.cpp
                    controllers/book_controller_tests.cpp
                    controllers/authentication_controller_tests.cpp
                    controllers/user_controller_tests.cpp
                    controllers/settings_controller_tests.cpp
                    
                    gateways/user_storage_gateway_tests.cpp
                    gateways/book_storage_gateway_tests.cpp
                    gateways/authentication_gateway_tests.cpp
                    
                    data_models/library_proxy_model_tests.cpp
                    data_models/test_data/sort_by_title_test_data.hpp
                    data_models/test_data/sort_by_recently_added_test_data.hpp
                    data_models/test_data/sort_by_fuzzing_test_data.hpp
                    data_models/test_data/sort_by_last_opened_test_data.hpp
                    data_models/test_data/sort_by_authors_test_data.hpp
                    data_models/test_data/filter_by_tags_test_data.hpp
                    data_models/test_data/filter_by_request_test_data.hpp
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
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models
                    ${CMAKE_CURRENT_SOURCE_DIR}/gateways
)

target_compile_options(adapters_unit_tests PRIVATE "${LIBRUM_SANITIZERS};-g3;-Og")
target_link_options(adapters_unit_tests PRIVATE "${LIBRUM_SANITIZERS}")