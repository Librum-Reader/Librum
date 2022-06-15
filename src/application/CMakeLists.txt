cmake_minimum_required(VERSION 3.21)


## Build
#add_library(application
#                SHARED
#)

#target_include_directories(application
#                PUBLIC
#                    ${CMAKE_CURRENT_SOURCE_DIR}/
#)
#target_compile_definitions(controller
#                PRIVATE
#                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
#)
#target_link_libraries(application
#                PUBLIC
#                    domain
#)
#target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
#target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")