# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)


# Build
add_library(rum_content OBJECT
                    book.hpp
                    book.cpp
)

target_link_libraries(rum_content
                PRIVATE
                    Qt6::Core
)