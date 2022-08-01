# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_subdirectory(domain_unit_tests/)
add_subdirectory(application_unit_tests/)
add_subdirectory(adapters_unit_tests/)
add_subdirectory(infrastructure_unit_tests/)