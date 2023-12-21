set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Build
add_library(presentation
                SHARED
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    qmlSources.qrc
                    modules/CppElements/key_sequence_recorder.cpp
                    modules/CppElements/key_sequence_recorder.hpp
                    modules/CppElements/sidebar_state.cpp
                    modules/CppElements/sidebar_state.hpp
                    modules/CppElements/page_view.cpp
                    modules/CppElements/page_view.hpp
)


# Translations
set(translation_files
    translations/librum_en.ts
    translations/librum_de.ts
    translations/librum_ru.ts
    translations/librum_zh.ts
    translations/librum_uk.ts
)
qt_add_translations(presentation TS_FILES ${translation_files})


target_compile_definitions(presentation PRIVATE PRESENTATION_LIBRARY)

target_compile_definitions(presentation
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(presentation
                PRIVATE
                    adapters
                    application
                    domain
                    ${MUPDF_OUTPUT}
                    Qt6::Core
                    Qt6::Quick
)

target_include_directories(presentation
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements
                    ${APPLICATION_INCLUDES}
)

target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS};-Wall;-O0;-g>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")


# Install rules
if(UNIX)
    install(TARGETS presentation
            DESTINATION lib
            )
endif()
