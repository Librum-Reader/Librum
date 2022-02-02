cmake_minimum_required(VERSION 3.22)



# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)



# Dependencies
include(FetchContent)
FetchContent_Declare(googletest
                    GIT_REPOSITORY https://github.com/google/googletest.git
                    GIT_TAG        f2d71f0be2ff2230789cfaf37d4c4f38db5b1c7c
    )

FetchContent_MakeAvailable(googletest)



# Build tests
add_executable(all_tests
                    main.cpp
    )

target_link_libraries(all_tests
                PRIVATE
                    model
                    controller
                    gtest
    )


# Run tests at build-time
add_custom_target(run_tests ALL
            COMMAND all_tests
    )