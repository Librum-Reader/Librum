# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)




# Build
add_library(domain
                SHARED
                    models/login_model.cpp
                    models/login_model.hpp
                    models/register_model.cpp
                    models/register_model.hpp
                    models/book.cpp
                    models/book.hpp
                    models/tag.cpp
                    models/tag.hpp
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/rum_content
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    Qt5::Core
)