# Build tests
add_executable(infrastructure_unit_tests
                    main.cpp
    )

target_link_libraries(infrastructure_unit_tests
                PRIVATE
                    gtest
    )