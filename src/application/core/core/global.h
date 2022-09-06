/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_GLOBAL_H
#define OKULAR_GLOBAL_H

#include <QGlobalStatic>

/**
 * \namespace Okular global.h
 *
 * \brief The documentation to the global Okular namespace.
 */

namespace Okular
{
/**
 * Describes the DRM capabilities.
 */
enum Permission {
    AllowModify = 1,    ///< Allows to modify the document
    AllowCopy = 2,      ///< Allows to copy the document
    AllowPrint = 4,     ///< Allows to print the document
    AllowNotes = 8,     ///< Allows to add annotations to the document
    AllowFillForms = 16 ///< Allows to fill the forms in the document
};
Q_DECLARE_FLAGS(Permissions, Permission)

/**
 * Describes the direction of searching.
 */
enum SearchDirection {
    FromTop,       ///< Searching from top of the page, next result is to be found, there was no earlier search result.
    FromBottom,    ///< Searching from bottom of the page, next result is to be found, there was no earlier search result.
    NextResult,    ///< Searching for the next result on the page, earlier result should be located so we search from the last result not from the beginning of the page.
    PreviousResult ///< Searching for the previous result on the page, earlier result should be located so we search from the last result not from the beginning of the page.
};

/**
 * A rotation.
 */
enum Rotation {
    Rotation0 = 0,   ///< Not rotated.
    Rotation90 = 1,  ///< Rotated 90 degrees clockwise.
    Rotation180 = 2, ///< Rotated 180 degrees clockwise.
    Rotation270 = 3  ///< Rotated 2700 degrees clockwise.
};

/**
 * Describes the type of generation of objects
 */
enum GenerationType {
    Synchronous, ///< Will create the object in a synchronous way
    Asynchronous ///< Will create the object in an asynchronous way
};

/**
 * The side(s) to be considered when merging areas.
 */
enum MergeSide {
    MergeRight = 0,  ///< Merge only if the right side of the first area intersect.
    MergeBottom = 1, ///< Merge only if the bottom side of the first area intersect.
    MergeLeft = 2,   ///< Merge only if the left side of the first area intersect.
    MergeTop = 3,    ///< Merge only if the top side of the first area intersect.
    MergeAll = 4     ///< Merge if the areas intersects, no matter which side(s).
};

/**
 * Describes the possible script types.
 */
enum ScriptType {
    JavaScript = 0 ///< JavaScript code
};

}

#endif
