function(set_okular_optional_package_properties _name _props)
    if(NOT "${_props}" STREQUAL "PROPERTIES")
        message(FATAL_ERROR "PROPERTIES keyword is missing in set_okular_optional_package_properties() call.")
    endif()

    set(options) # none
    set(oneValueArgs DESCRIPTION URL PURPOSE)
    set(multiValueArgs) # none

    CMAKE_PARSE_ARGUMENTS(_SPP "${options}" "${oneValueArgs}" "${multiValueArgs}"  ${ARGN})

    if(_SPP_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to set_okular_optional_package_properties(): \"${_SPP_UNPARSED_ARGUMENTS}\"")
    endif()

    set(DEPENDENCY_TYPE "REQUIRED")
    if (${_name} IN_LIST FORCE_NOT_REQUIRED_DEPENDENCIES)
        set(DEPENDENCY_TYPE "RECOMMENDED")
    endif()
    set_package_properties(${_name} PROPERTIES
        TYPE ${DEPENDENCY_TYPE}
        DESCRIPTION ${_SPP_DESCRIPTION}
        URL ${_SPP_URL}
        PURPOSE "${_SPP_PURPOSE} You can make the dependency optional adding ${_name} to the FORCE_NOT_REQUIRED_DEPENDENCIES cmake option"
    )
endfunction()