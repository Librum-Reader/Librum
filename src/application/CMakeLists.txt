# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(application
                SHARED
                    interfaces/services/i_authentication_service.hpp
                    services/authentication_service.cpp
                    services/authentication_service.hpp
                    test_request.hpp
                    test_request.cpp
)

target_include_directories(application
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(application
                PRIVATE
                    domain
                    Qt6::Core
                    Qt6::Network
)

target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")