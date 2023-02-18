# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)



# Build tests
add_executable(domain_unit_tests
                    main.cpp
                    login_model_tests.cpp
                    register_model_tests.cpp
                    book_tests.cpp
                    user_tests.cpp
                    tag_tests.cpp
)

target_include_directories(domain_unit_tests
                PRIVATE
                    ${DOMAIN_INCLUDES}
)

target_link_libraries(domain_unit_tests
                PRIVATE
                    gtest
                    domain
                    Qt5::Core
                    Qt5::Quick
)

target_compile_options(domain_unit_tests PRIVATE "${LIBRUM_SANITIZERS};-g3;-Og")
target_link_options(domain_unit_tests PRIVATE "${LIBRUM_SANITIZERS}")