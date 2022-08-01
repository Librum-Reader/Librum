# Build tests
add_executable(adapters_unit_tests
                    main.cpp
    )

target_link_libraries(adapters_unit_tests
                PRIVATE
                    gtest
    )