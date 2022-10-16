# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick Xml PrintSupport Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Xml PrintSupport Test REQUIRED)


# Build tests
add_executable(application_unit_tests
                    main.cpp
                    services/book_service_tests.hpp
                    services/user_service_tests.hpp
                    services/authentication_service_tests.hpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    domain
                    application
                    okularcore
                    gtest
                    gmock
                    Qt5::Core
                    Qt5::Quick
                    Qt5::Xml
                    Qt5::PrintSupport
                    Qt5::Test
    )

target_include_directories(application_unit_tests
                PRIVATE
                    ${APPLICATION_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
)