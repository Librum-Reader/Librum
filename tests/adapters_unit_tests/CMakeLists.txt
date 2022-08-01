# Build tests
add_executable(adapters_unit_tests
                    main.cpp
    )

target_link_libraries(adapters_unit_tests
                PRIVATE
                    gtest
    )


# Run tests at build-time
add_custom_target(run_adapters_unit_tests ALL
            COMMAND adapters_unit_tests
    )