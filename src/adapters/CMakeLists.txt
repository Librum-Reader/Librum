set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Build
add_library(adapters
                SHARED
                    interfaces/controllers/i_authentication_controller.hpp
                    interfaces/controllers/i_free_books_controller.hpp
                    interfaces/persistance/i_free_books_storage_access.hpp
                    interfaces/controllers/i_library_controller.hpp
                    interfaces/controllers/i_user_controller.hpp
                    interfaces/controllers/i_settings_controller.hpp
                    interfaces/controllers/i_page_controller.hpp
                    interfaces/controllers/i_book_controller.hpp
                    interfaces/controllers/i_app_info_controller.hpp
                    interfaces/controllers/i_dictionary_controller.hpp
                    interfaces/controllers/i_ai_explanation_controller.hpp
                    interfaces/controllers/i_folder_controller.hpp
                    interfaces/persistance/i_user_storage_access.hpp
                    interfaces/persistance/i_library_storage_access.hpp
                    interfaces/persistance/i_authentication_access.hpp
                    interfaces/persistance/i_app_info_access.hpp
                    interfaces/persistance/i_dictionary_access.hpp
                    interfaces/persistance/i_ai_explanation_access.hpp
                    interfaces/persistance/i_folder_storage_access.hpp
                    
                    controllers/authentication_controller.cpp
                    controllers/authentication_controller.hpp
                    controllers/free_books_controller.cpp
                    controllers/free_books_controller.hpp
                    controllers/library_controller.cpp
                    controllers/library_controller.hpp
                    controllers/user_controller.cpp
                    controllers/user_controller.hpp
                    controllers/settings_controller.cpp
                    controllers/settings_controller.hpp
                    controllers/app_info_controller.cpp
                    controllers/app_info_controller.hpp
                    controllers/book_controller.cpp
                    controllers/book_controller.hpp
                    controllers/page_controller.cpp
                    controllers/page_controller.hpp
                    controllers/dictionary_controller.cpp
                    controllers/dictionary_controller.hpp
                    controllers/ai_explanation_controller.cpp
                    controllers/ai_explanation_controller.hpp
                    controllers/folder_controller.hpp
                    controllers/folder_controller.cpp
                    controllers/external_book_controller.hpp
                    controllers/external_book_controller.cpp
                    controllers/tools_controller.hpp
                    controllers/tools_controller.cpp

                    gateways/user_storage_gateway.cpp
                    gateways/user_storage_gateway.hpp
                    gateways/authentication_gateway.cpp
                    gateways/authentication_gateway.hpp
                    gateways/free_books_storage_gateway.cpp
                    gateways/free_books_storage_gateway.hpp
                    gateways/library_storage_gateway.cpp
                    gateways/library_storage_gateway.hpp
                    gateways/app_info_gateway.cpp
                    gateways/app_info_gateway.hpp
                    gateways/dictionary_gateway.hpp
                    gateways/dictionary_gateway.cpp
                    gateways/ai_explanation_gateway.cpp
                    gateways/ai_explanation_gateway.hpp
                    gateways/folder_storage_gateway.hpp
                    gateways/folder_storage_gateway.cpp
                    
                    data_models/library_model/library_model.cpp
                    data_models/library_model/library_model.hpp
                    data_models/library_model/library_proxy_model.cpp
                    data_models/library_model/library_proxy_model.hpp
                    data_models/library_model/filter_request.hpp
                    data_models/free_books_model/free_books_model.cpp
                    data_models/free_books_model/free_books_model.hpp
                    data_models/user_tags_model/user_tags_model.cpp
                    data_models/user_tags_model/user_tags_model.hpp
                    data_models/shortcuts_model/shortcuts_model.cpp
                    data_models/shortcuts_model/shortcuts_model.hpp
                    data_models/shortcuts_model/shortcuts_proxy_model.cpp
                    data_models/shortcuts_model/shortcuts_proxy_model.hpp
                    data_models/bookmarks_model/bookmarks_model.cpp
                    data_models/bookmarks_model/bookmarks_model.hpp
                    data_models/bookmarks_model/bookmarks_proxy_model.cpp
                    data_models/bookmarks_model/bookmarks_proxy_model.hpp
                    data_models/folders_model/folders_model.hpp
                    data_models/folders_model/folders_model.cpp
                    data_models/folders_model/folders_proxy_model.hpp
                    data_models/folders_model/folders_proxy_model.cpp
                    data_models/icon_model/icon_model.hpp
                    data_models/icon_model/icon_model.cpp
                    data_models/icon_model/icon_proxy_model.hpp
                    data_models/icon_model/icon_proxy_model.cpp
                    data_models/icon_model/icon_item.hpp
                    data_models/book_title_model/book_title_model.hpp
                    data_models/book_title_model/book_title_model.cpp
                    data_models/book_title_model/book_title_proxy_model.hpp
                    data_models/book_title_model/book_title_proxy_model.cpp
                    
                    DTOs/login_dto.hpp
                    DTOs/register_dto.hpp
                    DTOs/book_dto.hpp
                    DTOs/tag_dto.hpp
                    DTOs/highlight_dto.hpp
                    DTOs/rectf_dto.hpp
                    DTOs/word_definition_dto.hpp
                    DTOs/folder_dto.hpp
                    interfaces/controllers/i_tools_controller.hpp
)

target_compile_definitions(adapters PRIVATE ADAPTERS_LIBRARY)

target_include_directories(adapters
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/persistance
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/gateways
                    ${CMAKE_CURRENT_SOURCE_DIR}/DTOs
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/library_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/free_books_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/user_tags_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/shortcuts_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/bookmarks_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/folders_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/icon_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models/book_title_model
                    ${CMAKE_CURRENT_SOURCE_DIR}/data_models
                PRIVATE
                    ${DOMAIN_INCLUDES}
                    ${APPLICATION_INCLUDES}
)

target_compile_definitions(adapters
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(adapters
                PRIVATE
                    domain
                    application
                    ${MUPDF_OUTPUT}
                    rapidfuzz::rapidfuzz
                    Qt6::Core
                    Qt6::Quick
                    Qt6::Network
)

target_compile_options(adapters PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};${LIBRUM_SANITIZERS};${COVERAGE_COMPILE_OPTIONS};-O0;-g>")
target_compile_options(adapters PRIVATE "$<$<CONFIG:RELEASE>:-O3>")

target_link_options(adapters PRIVATE "${COVERAGE_LINKING_OPTIONS}")


# Generate code coverage
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # Only works when the compiler is GCC
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(adapters)
    setup_target_for_coverage_gcovr_html(
              NAME adapters_coverage
              EXECUTABLE adapters_unit_tests
              BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
              EXCLUDE ${PROJECT_BINARY_DIR}/* ${PROJECT_SOURCE_DIR}/libs/*)
endif()

# Install rules
if(UNIX)
    install(TARGETS adapters
            DESTINATION lib
            )
endif()
