# Copies all files with the same hierarchy (folder)
# via configure_file but only when the file content is different:
#
# copy_recursive(${SOURCE_PATH} ${DESTINATION_PATH} ${REGEX})
#
# If you want to copy all files simply set the parameter to: "*"
# Example:
#
# include(CopyRecursive)
# copy_recursive(${CMAKE_CURRENT_SOURCE_DIR}/kde/ScreenPlay ${CMAKE_BINARY_DIR}/bin/kde/ScreenPlay "*")
#

function(copy_recursive SOURCE_PATH DESTINATION_PATH REGEX)
    # Get all files
    file(GLOB_RECURSE
        FILES
        ${SOURCE_PATH}
        "${SOURCE_PATH}/${REGEX}")

    # Copy each file
    foreach(file ${FILES})
        file(RELATIVE_PATH RELATIVE_FILE_PATH ${SOURCE_PATH} ${file})
        get_filename_component(FOLDER ${RELATIVE_FILE_PATH} DIRECTORY ${SOURCE_PATH})
        file(MAKE_DIRECTORY "${DESTINATION_PATH}/${FOLDER}")
        message(DEBUG "File created at: ${file}   - ${DESTINATION_PATH}/${RELATIVE_FILE_PATH}")
        configure_file(${file} "${DESTINATION_PATH}/${RELATIVE_FILE_PATH}" COPYONLY)
    endforeach()

endfunction()
