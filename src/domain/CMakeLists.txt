set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Build
add_library(domain
                SHARED
                    entities/book.cpp
                    entities/book.hpp
                    entities/free_book.hpp
                    entities/tag.cpp
                    entities/tag.hpp
                    entities/user.cpp
                    entities/user.hpp
                    entities/highlight.cpp
                    entities/highlight.hpp
                    entities/rectf.cpp
                    entities/rectf.hpp
                    entities/bookmark.cpp
                    entities/bookmark.hpp
                    
                    value_objects/book_meta_data.hpp
                    value_objects/login_model.cpp
                    value_objects/login_model.hpp
                    value_objects/register_model.cpp
                    value_objects/register_model.hpp
)

target_compile_definitions(domain PRIVATE DOMAIN_LIBRARY)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/entities
                    ${CMAKE_CURRENT_SOURCE_DIR}/value_objects
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    Qt6::Core
                    Qt6::Quick
)

target_compile_options(domain PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};${LIBRUM_SANITIZERS};${COVERAGE_COMPILE_OPTIONS};-O0;-g>")
target_compile_options(domain PRIVATE "$<$<CONFIG:RELEASE>:-O3>")

target_link_options(domain PRIVATE "${COVERAGE_LINKING_OPTIONS}")


# Generate code coverage
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # Only works when the compiler is GCC
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(domain)
    setup_target_for_coverage_gcovr_html(
              NAME domain_coverage
              EXECUTABLE domain_unit_tests
              BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
              EXCLUDE ${PROJECT_BINARY_DIR}/* ${PROJECT_SOURCE_DIR}/libs/*)
endif()


# Install rules
if(UNIX)
    install(TARGETS domain
            DESTINATION lib
            )
endif()