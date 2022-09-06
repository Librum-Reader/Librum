# - Try to find the DjVuLibre library
# Once done this will define
#
#  DJVULIBRE_FOUND - system has the DjVuLibre library
#  DJVULIBRE_INCLUDE_DIR - the DjVuLibre include directory
#  DJVULIBRE_LIBRARY - Link this to use the DjVuLibre library

if (DJVULIBRE_INCLUDE_DIR AND DJVULIBRE_LIBRARY)

  # in cache already
  set(DJVULIBRE_FOUND TRUE)

else (DJVULIBRE_INCLUDE_DIR AND DJVULIBRE_LIBRARY)
  IF (NOT WIN32)
        find_package(PkgConfig)

        pkg_check_modules(PC_DJVULIBRE ddjvuapi)
  endif(NOT WIN32)

    find_path(DJVULIBRE_INCLUDE_DIR libdjvu/ddjvuapi.h
      PATHS
      ${PC_DJVULIBRE_INCLUDEDIR}
      ${PC_DJVULIBRE_INCLUDE_DIRS}
      ${GNUWIN32_DIR}/include
    )

    find_library(DJVULIBRE_LIBRARY NAMES djvulibre
      PATHS
      ${PC_DJVULIBRE_LIBDIR}
      ${PC_DJVULIBRE_LIBRARY_DIRS}
      ${GNUWIN32_DIR}/lib
    )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(DjVuLibre DEFAULT_MSG DJVULIBRE_INCLUDE_DIR DJVULIBRE_LIBRARY )
  
  mark_as_advanced(DJVULIBRE_INCLUDE_DIR DJVULIBRE_LIBRARY)

endif (DJVULIBRE_INCLUDE_DIR AND DJVULIBRE_LIBRARY)
