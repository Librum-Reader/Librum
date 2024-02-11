# Build tests
add_executable(application_unit_tests
                    main.cpp
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    services/library_service_tests.cpp
                    services/user_service_tests.cpp
                    services/settings_service_tests.cpp
                    services/authentication_service_tests.cpp
                    services/folder_service_tests.cpp
                    
                    utility/local_library_tracker_tests.cpp
                    utility/library_storage_manager_tests.cpp
                    utility/test_enum.hpp
                    utility/enum_utils_tests.cpp
                    utility/book_merger_tests.cpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    domain
                    application
                    gtest
                    gmock
                    Qt6::Core
                    Qt6::Quick
                    Qt6::Test
    )

target_include_directories(application_unit_tests
                PRIVATE
                    ${APPLICATION_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/utility
)

target_compile_options(application_unit_tests PRIVATE "${LIBRUM_SANITIZERS};-O0;-g")
target_link_options(application_unit_tests PRIVATE "${LIBRUM_SANITIZERS}")

add_test(NAME ApplicationUnitTests 
         COMMAND ${PROJECT_BINARY_DIR}/application_unit_tests)