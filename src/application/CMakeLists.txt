# Dependencies
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Network)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Setup
set(application_SRC
    interfaces/services/i_authentication_service.hpp
    interfaces/services/i_book_service.hpp
    interfaces/services/i_user_service.hpp
    interfaces/services/i_settings_service.hpp
    interfaces/gateways/i_user_storage_gateway.hpp
    interfaces/gateways/i_book_storage_gateway.hpp
    interfaces/gateways/i_authentication_gateway.hpp
    interfaces/utility/i_book_metadata_helper.hpp
    interfaces/utility/i_downloaded_books_tracker.hpp
    interfaces/utility/i_book_storage_manager.hpp
    
    services/authentication_service.cpp
    services/authentication_service.hpp
    services/book_service.cpp
    services/book_service.hpp
    services/user_service.cpp
    services/user_service.hpp
    services/settings_service.cpp
    services/settings_service.hpp
    
    common/enums/book_operation_status.hpp
    common/enums/setting_keys.hpp
    common/enums/setting_groups.hpp
    common/enums/error_code.hpp
    
    utility/book_metadata_helper.cpp
    utility/book_metadata_helper.hpp
    utility/downloaded_books_tracker.cpp
    utility/downloaded_books_tracker.hpp
    utility/merge_status.hpp
    utility/book_storage_manager.cpp
    utility/book_storage_manager.hpp
    utility/book_for_deletion.hpp
    utility/enum_utils.hpp
    utility/application_settings.hpp
    utility/book_merger.cpp
    utility/book_merger.hpp
    utility/book_merger.cpp
    utility/automatic_login_helper.hpp
    utility/error_code_converter.hpp
    
    core/document.hpp
    core/document.cpp
    core/page.hpp
    core/page.cpp
    core/toc/toc_item.hpp
    core/toc/toc_item.cpp
    core/toc/toc_model.hpp
    core/toc/toc_model.cpp
    core/toc/filtered_toc_model.hpp
    core/toc/filtered_toc_model.cpp
)


# Build mupdf
if(UNIX)
    set(MUPDF_OUTPUT "${PROJECT_SOURCE_DIR}/libs/mupdf/build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release>/libmupdfcpp.so")
    set(MUPDF_OUTPUT "${PROJECT_SOURCE_DIR}/libs/mupdf/build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release>/libmupdfcpp.so" PARENT_SCOPE)
    set(MUPDF_BUILD_COMMAND "./scripts/mupdfwrap.py --venv -d build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release> -b -j 0 m01")
elseif(WIN32)
    set(MUPDF_OUTPUT "${PROJECT_SOURCE_DIR}/libs/mupdf/platform/win32/x64/Release/mupdfcpp64.lib")
    set(MUPDF_OUTPUT "${PROJECT_SOURCE_DIR}/libs/mupdf/platform/win32/x64/Release/mupdfcpp64.lib" PARENT_SCOPE)
    set(MUPDF_BUILD_COMMAND "python scripts/mupdfwrap.py --venv -b -j 0 m01")
endif()


add_custom_command(
    OUTPUT ${MUPDF_OUTPUT}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/mupdf
    COMMAND ${MUPDF_BUILD_COMMAND}
    COMMENT "Building mupdf (This takes a while) ..."
)

add_custom_target(mupdf ALL
   DEPENDS ${MUPDF_OUTPUT}
)



# Build
add_library(application
                SHARED
                    ${application_SRC}
)

# Make sure to build mupdf before the application
add_dependencies(application mupdf)

target_compile_definitions(application PRIVATE APPLICATION_LIBRARY)

target_include_directories(application
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${DOMAIN_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/gateways
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/utility
                    ${CMAKE_CURRENT_SOURCE_DIR}/common/enums
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/utility
                    ${CMAKE_CURRENT_SOURCE_DIR}/core
                    ${PROJECT_SOURCE_DIR}/libs/mupdf/platform/c++/include
                    ${PROJECT_SOURCE_DIR}/libs/mupdf/include
)

target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(application
                PRIVATE
                    Qt6::Core
                    Qt6::Quick
                    Qt6::Network
                    rapidfuzz::rapidfuzz
                    domain
                    ${MUPDF_OUTPUT}
)



target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_WARNINGS};${SPECIALISED_SANITIZERS};${COVERAGE_COMPILE_OPTIONS};-O0;-g>")
target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")

target_link_options(application PRIVATE "${COVERAGE_LINKING_OPTIONS}")


# Generate code coverage
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # Only works when the compiler is GCC
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(application)
    setup_target_for_coverage_gcovr_html(
              NAME application_coverage
              EXECUTABLE application_unit_tests
              BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
              EXCLUDE ${PROJECT_BINARY_DIR}/* ${PROJECT_SOURCE_DIR}/libs/*)
endif()
