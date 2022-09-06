# - Find Discount
# Find the discount markdown library.
#
# This module defines
#  Discount_FOUND - whether the discount library was found
#  PkgConfig::Discount - the discount target to link to

# SPDX-FileCopyrightText: 2017 Julian Wolff <wolff@julianwolff.de>
# SPDX-FileCopyrightText: 2018 Sune Vuorela <sune@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

include(FindPackageHandleStandardArgs)

find_package(PkgConfig REQUIRED)

pkg_check_modules(Discount IMPORTED_TARGET "libmarkdown")

find_package_handle_standard_args(Discount DEFAULT_MSG Discount_LIBRARIES)
