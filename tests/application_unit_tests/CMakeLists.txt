# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)


# Build tests
add_executable(application_unit_tests
                    main.cpp
                    book_service_tests.hpp
                    authentication_service_tests.hpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    domain
                    application
                    gtest
                    gmock
                    Qt5::Core
    )

target_include_directories(application_unit_tests
                PRIVATE
                    ${APPLICATION_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${PROJECT_SOURCE_DIR}
)