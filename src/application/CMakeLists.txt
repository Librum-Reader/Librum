set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Setup
set(application_SRC
    interfaces/services/i_authentication_service.hpp
    interfaces/services/i_free_books_service.hpp
    interfaces/services/i_library_service.hpp
    interfaces/services/i_user_service.hpp
    interfaces/services/i_settings_service.hpp
    interfaces/services/i_app_info_service.hpp
    interfaces/services/i_book_service.hpp
    interfaces/services/i_dictionary_service.hpp
    interfaces/services/i_ai_explanation_service.hpp
    interfaces/services/i_folder_service.hpp
    interfaces/gateways/i_free_books_storage_gateway.hpp
    interfaces/gateways/i_user_storage_gateway.hpp
    interfaces/gateways/i_library_storage_gateway.hpp
    interfaces/gateways/i_authentication_gateway.hpp
    interfaces/gateways/i_app_info_gateway.hpp
    interfaces/gateways/i_dictionary_gateway.hpp
    interfaces/gateways/i_ai_explanation_gateway.hpp
    interfaces/gateways/i_folder_storage_gateway.hpp
    interfaces/managers/i_library_storage_manager.hpp
    interfaces/utility/i_metadata_extractor.hpp
    interfaces/utility/i_local_library_tracker.hpp
    interfaces/utility/i_book_getter.hpp
    interfaces/services/i_tools_service.hpp

    services/authentication_service.cpp
    services/authentication_service.hpp
    services/free_books_service.cpp
    services/free_books_service.hpp
    services/library_service.cpp
    services/library_service.hpp
    services/user_service.cpp
    services/user_service.hpp
    services/settings_service.cpp
    services/settings_service.hpp
    services/app_info_service.cpp
    services/app_info_service.hpp
    services/book_service.hpp
    services/book_service.cpp
    services/dictionary_service.cpp
    services/dictionary_service.hpp
    services/ai_explanation_service.cpp
    services/ai_explanation_service.hpp
    services/folder_service.hpp
    services/folder_service.cpp
    services/tools_service.hpp
    services/tools_service.cpp

    managers/library_storage_manager.cpp
    managers/library_storage_manager.hpp
    
    common/enums/book_operation_status.hpp
    common/enums/setting_keys.hpp
    common/enums/setting_groups.hpp
    common/enums/error_code.hpp
    
    utility/local_library_tracker.cpp
    utility/local_library_tracker.hpp
    utility/merge_status.hpp
    utility/book_for_deletion.hpp
    utility/enum_utils.hpp
    utility/application_settings.hpp
    utility/book_merger.cpp
    utility/book_merger.hpp
    utility/book_merger.cpp
    utility/automatic_login_helper.hpp
    utility/error_code_converter.hpp
    utility/book_utils.hpp
    utility/save_book_helper.hpp
    utility/library_book_getter.hpp
    utility/library_book_getter.cpp
    utility/external_book_getter.hpp
    utility/external_book_getter.cpp
    
    core/page_generator.hpp
    core/page_generator.cpp
    core/metadata_extractor.cpp
    core/metadata_extractor.hpp
    core/toc/toc_item.hpp
    core/toc/toc_item.cpp
    core/toc/toc_model.hpp
    core/toc/toc_model.cpp
    core/toc/filtered_toc_model.hpp
    core/toc/filtered_toc_model.cpp
    core/utils/book_searcher.hpp
    core/utils/book_searcher.cpp
    core/utils/fz_utils.hpp
    core/utils/text_selector.hpp
    core/utils/text_selector.cpp
    core/utils/search_options.hpp
    core/utils/mutool_utils.hpp
)



########## Build mupdf ##########
set(VENV_OPTION "")
if(NOT ${NO_VENV})
    set(VENV_OPTION "--venv")
endif()

if(ANDROID)
    set(EXTRA_MAKE_AGRS --m-vars 'HAVE_OBJCOPY=no HAVE_LIBCRYPTO=no')
endif()

if(UNIX)
    set(MUPDF_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/libs/mupdf/build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so" PARENT_SCOPE)
    set(MUPDF_BUILD_COMMAND ./scripts/mupdfwrap.py ${VENV_OPTION} -d build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release> -b --m-target 'libs tools' ${EXTRA_MAKE_AGRS} -j 0 m01)
elseif(WIN32)
    set(MUPDF_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/libs/mupdf/platform/win32/x64/$<IF:$<CONFIG:Debug>,Debug,Release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib" PARENT_SCOPE)
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib")
    set(MUPDF_BUILD_COMMAND python scripts/mupdfwrap.py ${VENV_OPTION} -d build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release> -b -j 0 m01)
endif()

message("MuPdf build command: " ${MUPDF_BUILD_COMMAND})


set(CC_COMMAND "${CMAKE_C_COMPILER}")
set(CXX_COMMAND "${CMAKE_CXX_COMPILER}")

if(ANDROID)
    string(APPEND CC_COMMAND " --target=aarch64-none-linux-android23 --sysroot=/home/creapermann/Android/Sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/linux-x86_64/sysroot")
    string(APPEND CXX_COMMAND " --target=aarch64-none-linux-android23 --sysroot=/home/creapermann/Android/Sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/linux-x86_64/sysroot")
    set(ANDROID_COMPILERS "CC=${CC_COMMAND}" "CXX=${CXX_COMMAND}")
endif()


# On linux, we want to use the system provided libjpeg,
# since the one that comes with mupdf is not compatible with the one that comes with Qt.
if(LINUX)
    set(USE_SYSTEM_LIBJPEG_VALUE yes)
else()
    set(USE_SYSTEM_LIBJPEG_VALUE no)
endif()

if(UNIX)
    set(EXECUTABLE_EXTENSION "")
else()
    set(EXECUTABLE_EXTENSION ".exe")
endif()

add_custom_target(mupdf
   # Build mupdf
   COMMAND ${CMAKE_COMMAND} -E env
        ${ANDROID_COMPILERS}
        "USE_SYSTEM_LIBJPEG=${USE_SYSTEM_LIBJPEG_VALUE}"
        "USE_SONAME=no"
        ${MUPDF_BUILD_COMMAND}
   # Copy mutool to the build directory
   COMMAND ${CMAKE_COMMAND} -E copy
        "${MUPDF_OUTPUT_DIR}/mutool${EXECUTABLE_EXTENSION}"
        "${PROJECT_BINARY_DIR}/mutool${EXECUTABLE_EXTENSION}"
   BYPRODUCTS ${MUPDF_OUTPUT}
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/mupdf
   COMMENT "Building mupdf (This takes a while) ..."
)


#Copy the mupdf dlls to the build directory for windows
if(WIN32)
    add_custom_command(
      TARGET mupdf POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              "${MUPDF_OUTPUT_DIR}/mupdfcpp64.dll"
              "${PROJECT_BINARY_DIR}/mupdfcpp64.dll"
      BYPRODUCTS "${PROJECT_BINARY_DIR}/mupdfcpp64.dll"
      DEPENDS ${MUPDF_OUTPUT}
    )
endif()



########## Build application ##########
add_library(application
                SHARED
                    ${application_SRC}
)

add_dependencies(application mupdf) # Ensure the mupdf target is built before the application target

target_compile_definitions(application PRIVATE APPLICATION_LIBRARY)

target_include_directories(application
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${DOMAIN_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/gateways
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/utility
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/managers
                    ${CMAKE_CURRENT_SOURCE_DIR}/common/enums
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/utility
                    ${CMAKE_CURRENT_SOURCE_DIR}/managers
                    ${CMAKE_CURRENT_SOURCE_DIR}/core
                    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
)

# Make sure to ignore warnings from mupdf by adding it as a system include directory
target_include_directories(application SYSTEM PUBLIC
                    ${PROJECT_SOURCE_DIR}/libs/mupdf/platform/c++/include
                    ${PROJECT_SOURCE_DIR}/libs/mupdf/include
)

target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(application
                PUBLIC
                    rapidfuzz::rapidfuzz
                PRIVATE
                    Qt6::Core
                    Qt6::Quick
                    Qt6::Widgets
                    Qt6::Network
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


###### Install rules ######
if(LINUX)
    install(TARGETS application
            DESTINATION lib
            )
    
    # Install mupdf's shared libraries
    install(FILES ${MUPDF_OUTPUT_DIR}/libmupdfcpp.so
                  ${MUPDF_OUTPUT_DIR}/libmupdf.so
                  ${MUPDF_OUTPUT_DIR}/mutool${EXECUTABLE_EXTENSION}
            DESTINATION lib)

    # Install links with correct permissions
    if(EXISTS "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so.24.0")
        install(FILES ${MUPDF_OUTPUT_DIR}/libmupdfcpp.so.24.0
                      ${MUPDF_OUTPUT_DIR}/libmupdf.so.24.0
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ GROUP_EXECUTE
                            WORLD_READ WORLD_EXECUTE
                DESTINATION lib)
    endif()

elseif(APPLE)
    install(TARGETS application
            DESTINATION lib
            )
    
    # Install mupdf's shared libraries
    install(FILES ${MUPDF_OUTPUT_DIR}/libmupdfcpp.so
                  ${MUPDF_OUTPUT_DIR}/libmupdf.dylib
            DESTINATION lib)
endif()
