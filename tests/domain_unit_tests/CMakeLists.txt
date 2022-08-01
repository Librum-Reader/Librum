# Build tests
add_executable(domain_unit_tests
                    main.cpp
    )

target_link_libraries(domain_unit_tests
                PRIVATE
                    gtest
    )