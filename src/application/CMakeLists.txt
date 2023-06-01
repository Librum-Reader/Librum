# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network REQUIRED)

find_package(ECM ${KF5_REQUIRED_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

add_subdirectory(core)



# Setup
set(application_SRC
    settings.hpp
    
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
    utility/cover_observer.cpp
    utility/cover_observer.hpp
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
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}  # For generated okular settings file
                ${DOMAIN_INCLUDES}
                ${OKULARCORE_INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/services
                ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/gateways
                ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/utility
                ${CMAKE_CURRENT_SOURCE_DIR}/common/enums
                ${CMAKE_CURRENT_SOURCE_DIR}/services
                ${CMAKE_CURRENT_SOURCE_DIR}/utility
)

target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(application
                PRIVATE
                    Qt5::Core
                    Qt5::Quick
                    Qt5::Network
                    domain
                    okularcore
)


# Precompiled header support
option(LIBRUM_BUILD_USING_PCH "Build using pre-compile header support" ON)
if (LIBRUM_BUILD_USING_PCH)
    message("Compiling using pre-compiled headers")
    target_precompile_headers(application PUBLIC librum_application_pch.hpp) # PUBLIC - every linking target gets the same PCHs
    set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
else()
    message(STATUS "Precompiled headers are disabled")
endif()


# Remove 'address sanitizer' from 'application' target when compiling with anything but GCC.
# Okular core has some ODR violations, which lead to 'address sanitizer' failing on startup.
# It works well with GCC though, so just remove it when compiling with any other compiler.
set(SPECIALISED_SANITIZERS ${LIBRUM_SANITIZERS})  # Fixed list of sanitizers

# Replace 'address' with empty string if compiler_id is not GNU
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    string(REPLACE "address," "" SPECIALISED_SANITIZERS "${SPECIALISED_SANITIZERS}")
    message("Disabled 'address sanitizer' for target 'application'")
endif()



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