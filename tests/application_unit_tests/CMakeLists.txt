# Build tests
add_executable(application_unit_tests
                    main.cpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    gtest
    )


# Run tests at build-time
add_custom_target(run_application_unit_tests ALL
            COMMAND application_unit_tests
    )