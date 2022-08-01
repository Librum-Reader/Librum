# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Quick Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(adapters
                SHARED
                    interfaces/controllers/i_authentication_controller.hpp
                    interfaces/persistance/i_user_storage_access.hpp
                    
                    controllers/authentication_controller.cpp
                    controllers/authentication_controller.hpp
                    
                    gateways/user_storage_gateway.cpp
                    gateways/user_storage_gateway.hpp
                    
                    models/app_information.hpp
                    models/chapter_tree/chapter_tree_model.cpp
                    models/chapter_tree/chapter_tree_node.cpp
                    models/chapter_tree/chapter_tree_model.hpp
                    models/chapter_tree/chapter_tree_node.hpp
                    
                    DTOs/login_dto.hpp
                    DTOs/register_dto.hpp
)

target_include_directories(adapters
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/persistance
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/gateways
                    ${CMAKE_CURRENT_SOURCE_DIR}/DTOs
                PRIVATE
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/models/chapter_tree
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

target_compile_definitions(adapters
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(adapters
                PRIVATE
                    domain
                    application
                    Qt6::Core
                    Qt6::Quick
                    Qt6::Network
)

target_compile_options(adapters PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(adapters PRIVATE "$<$<CONFIG:RELEASE>:-O3>")



# Qml-Module
qt_add_qml_module(adapters
  URI "Librum.Models"
  VERSION 1.0
)