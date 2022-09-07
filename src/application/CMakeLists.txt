# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)





# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

find_package(ECM ${KF5_REQUIRED_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

add_subdirectory(core)




# Setup
set(application_SRC
    settings.hpp

    interfaces/services/i_authentication_service.hpp
    interfaces/services/i_book_service.hpp
    interfaces/gateways/i_user_storage_gateway.hpp
    
    services/authentication_service.cpp
    services/authentication_service.hpp
    services/book_service.cpp
    services/book_service.hpp
)


find_package(KF5Config)
kconfig_add_kcfg_files(application_SRC GENERATE_MOC ${CMAKE_CURRENT_SOURCE_DIR}/core/conf/settings_mobile.kcfgc)





# Build
add_library(application
                SHARED
                    ${application_SRC}
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
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/core
                    ${CMAKE_CURRENT_BINARY_DIR}/core/
                    ${CMAKE_CURRENT_SOURCE_DIR}/core/core
                    ${CMAKE_CURRENT_BINARY_DIR}/core/core
)

target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(application
                PRIVATE
                    Qt5::Core
                    Qt5::Network
                    domain
                    okularcore
)

target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")
