# Build tests
add_executable(infrastructure_unit_tests
                    main.cpp
    )

target_link_libraries(infrastructure_unit_tests
                PRIVATE
                    gtest
    )


# Run tests at build-time
add_custom_target(run_infrastructure_unit_tests ALL
            COMMAND infrastructure_unit_tests
    )