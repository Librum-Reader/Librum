set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Build
add_library(infrastructure
                SHARED
                    persistance/user_storage_access.cpp
                    persistance/user_storage_access.hpp
                    persistance/library_storage_access.cpp
                    persistance/library_storage_access.hpp
                    persistance/authentication_access.cpp
                    persistance/authentication_access.hpp
                    persistance/app_info_access.cpp
                    persistance/app_info_access.hpp
                    persistance/dictionary_access.cpp
                    persistance/dictionary_access.hpp
                    
                    persistance/api_error_helper.hpp
                    data/endpoints.hpp
)

target_include_directories(infrastructure
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/persistance
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/data
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${ADAPTERS_INCLUDES}
)
target_compile_definitions(infrastructure
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(infrastructure
                PRIVATE
                    adapters
                    Qt6::Core
                    Qt6::Gui
                    Qt6::Network
)
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};${LIBRUM_SANITIZERS};-O0;-g>")
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:RELEASE>:-O3>")


# Install rules
if(UNIX)
    install(TARGETS infrastructure
            DESTINATION lib
            )
endif()
