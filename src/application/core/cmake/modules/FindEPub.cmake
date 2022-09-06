# - Find EPub
# Find the EPub library.
#
# This module defines
#  EPUB_FOUND - whether the EPub library was found
#  EPUB_LIBRARIES - the EPub library
#  EPUB_INCLUDE_DIR - the include path of the EPub library

# SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
# SPDX-License-Identifier: BSD-3-Clause


if (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

  # Already in cache
  set (EPUB_FOUND TRUE)

else (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

  find_library (EPUB_LIBRARIES
    NAMES epub libepub
  )

  find_path (EPUB_INCLUDE_DIR
    NAMES epub.h
  )

  include (FindPackageHandleStandardArgs)
  find_package_handle_standard_args (EPub DEFAULT_MSG EPUB_LIBRARIES EPUB_INCLUDE_DIR)

endif (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

mark_as_advanced(EPUB_INCLUDE_DIR EPUB_LIBRARIES)
