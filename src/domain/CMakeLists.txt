# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)




# Build
add_library(domain
                SHARED
                    entities/book.cpp
                    entities/book.hpp
                    entities/tag.cpp
                    entities/tag.hpp
                    entities/user.cpp
                    entities/user.hpp
                    
                    value_objects/book_meta_data.hpp
                    value_objects/login_model.cpp
                    value_objects/login_model.hpp
                    value_objects/register_model.cpp
                    value_objects/register_model.hpp
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/entities
                    ${CMAKE_CURRENT_SOURCE_DIR}/value_objects
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    Qt5::Core
                    Qt5::Quick
)

target_compile_options(domain PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};${LIBRUM_SANITIZERS};-O0;-g>")
target_compile_options(domain PRIVATE "$<$<CONFIG:RELEASE>:-O3>")