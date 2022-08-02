# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)



# Build tests
add_executable(domain_unit_tests
                    main.cpp
                    login_model_tests.hpp
)

target_include_directories(domain_unit_tests
                PRIVATE
                    ${DOMAIN_INCLUDES}
)

target_link_libraries(domain_unit_tests
                PRIVATE
                    gtest
                    domain
                    Qt6::Core
)