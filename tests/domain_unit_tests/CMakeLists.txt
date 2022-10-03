# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)



# Build tests
add_executable(domain_unit_tests
                    main.cpp
                    login_model_tests.hpp
                    register_model_tests.hpp
                    book_tests.hpp
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