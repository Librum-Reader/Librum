# Build tests
add_executable(application_unit_tests
                    main.cpp
    )

target_link_libraries(application_unit_tests
                PRIVATE
                    gtest
    )