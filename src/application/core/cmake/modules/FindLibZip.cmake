# - Try to find libzip
# Once done this will define
#
#  LIBZIP_FOUND - system has the zip library
#  LIBZIP_INCLUDE_DIR - the zip include directory
#  LIBZIP_LIBRARY - Link this to use the zip library
#
# SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>
# SPDX-License-Identifier: BSD-3-Clause

if (LIBZIP_LIBRARY AND LIBZIP_INCLUDE_DIR)
  # in cache already
  set(LIBZIP_FOUND TRUE)
else (LIBZIP_LIBRARY AND LIBZIP_INCLUDE_DIR)

  find_path(LIBZIP_INCLUDE_DIR zip.h
    ${GNUWIN32_DIR}/include
  )

  find_library(LIBZIP_LIBRARY NAMES zip
    PATHS
    ${GNUWIN32_DIR}/lib
  )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibZip DEFAULT_MSG LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

  # ensure that they are cached
  set(LIBZIP_INCLUDE_DIR ${LIBZIP_INCLUDE_DIR} CACHE INTERNAL "The libzip include path")
  set(LIBZIP_LIBRARY ${LIBZIP_LIBRARY} CACHE INTERNAL "The libraries needed to use libzip")

endif (LIBZIP_LIBRARY AND LIBZIP_INCLUDE_DIR)
