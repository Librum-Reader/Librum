# Build tests
add_executable(domain_unit_tests
                    main.cpp
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
                    Qt6::Core
                    Qt6::Quick
)

target_compile_options(domain_unit_tests PRIVATE "${LIBRUM_SANITIZERS};-O0;-g")
target_link_options(domain_unit_tests PRIVATE "${LIBRUM_SANITIZERS}")

add_test(NAME DomainUnitTests 
         COMMAND ${PROJECT_BINARY_DIR}/domain_unit_tests)