# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)




# Build
add_library(domain
                SHARED
                    models/login_model.cpp
                    models/login_model.hpp
                    models/register_model.cpp
                    models/register_model.hpp
                    models/book.cpp
                    models/book.hpp
                    models/book_meta_data.hpp
                    models/tag.cpp
                    models/tag.hpp
                    models/user.cpp
                    models/user.hpp
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    Qt5::Core
                    Qt5::Quick
)

target_compile_options(domain PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};-g3;-Og>")
target_compile_options(domain PRIVATE "$<$<CONFIG:RELEASE>:-O3>")