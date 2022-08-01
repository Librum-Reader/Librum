# Build tests
add_executable(domain_unit_tests
                    main.cpp
    )

target_link_libraries(domain_unit_tests
                PRIVATE
                    gtest
    )


# Run tests at build-time
add_custom_target(run_domain_unit_tests ALL
            COMMAND domain_unit_tests
    )