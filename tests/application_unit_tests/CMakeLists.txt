# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick Xml PrintSupport Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Xml PrintSupport Test REQUIRED)


# Build tests
add_executable(application_unit_tests
                    main.cpp
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    services/book_service_tests.cpp
                    services/user_service_tests.cpp
                    services/settings_service_tests.cpp
                    services/authentication_service_tests.cpp
                    
                    utility/downloaded_books_tracker_tests.cpp
                    utility/book_storage_manager_tests.cpp
                    utility/test_enum.hpp
                    utility/enum_utils_tests.cpp
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
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/utility
)

target_compile_options(application_unit_tests PRIVATE "${LIBRUM_SANITIZERS};-O0;-g")
target_link_options(application_unit_tests PRIVATE "${LIBRUM_SANITIZERS}")