# - Try to find the chm library
# Once done this will define
#
#  CHM_FOUND - system has the chm library
#  CHM_INCLUDE_DIR - the chm include directory
#  CHM_LIBRARY - Link this to use the chm library
#
# SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>
# SPDX-License-Identifier: BSD-3-Clause

if (CHM_LIBRARY AND CHM_INCLUDE_DIR)
  # in cache already
  set(CHM_FOUND TRUE)
else (CHM_LIBRARY AND CHM_INCLUDE_DIR)

  find_path(CHM_INCLUDE_DIR chm_lib.h
    ${GNUWIN32_DIR}/include
  )

  find_library(CHM_LIBRARY NAMES chm
    PATHS
    ${GNUWIN32_DIR}/lib
  )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(CHM DEFAULT_MSG CHM_INCLUDE_DIR CHM_LIBRARY )
  # ensure that they are cached
  set(CHM_INCLUDE_DIR ${CHM_INCLUDE_DIR} CACHE INTERNAL "The chmlib include path")
  set(CHM_LIBRARY ${CHM_LIBRARY} CACHE INTERNAL "The libraries needed to use chmlib")

endif (CHM_LIBRARY AND CHM_INCLUDE_DIR)
