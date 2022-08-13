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
                    interfaces/services/i_book_service.hpp
                    interfaces/gateways/i_user_storage_gateway.hpp
                    interfaces/converters/i_pdf_to_rum_converter.hpp
                    
                    services/authentication_service.cpp
                    services/authentication_service.hpp
                    services/book_service.cpp
                    services/book_service.hpp
                    
                    converters/pdf_to_rum_converter.cpp
                    converters/pdf_to_rum_converter.hpp
)

target_include_directories(application
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/gateways
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/converters
                    ${CMAKE_CURRENT_SOURCE_DIR}/converters
                    ${DOMAIN_INCLUDES}
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