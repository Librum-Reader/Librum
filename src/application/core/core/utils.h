/*
    SPDX-FileCopyrightText: 2006 Luigi Toscano <luigi.toscano@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_UTILS_H_
    #define _OKULAR_UTILS_H_

    #include "area.h"
    #include "okularcore_export.h"

class QRect;
class QImage;

namespace Okular
{
/**
 * @short General utility functions.
 *
 * This class contains some static functions of general utility.
 */
class OKULARCORE_EXPORT Utils
{
public:
    /**
     * Rotate the rect \p source in the area \p width x \p height with the
     * specified \p orientation .
     */
    static QRect rotateRect(const QRect source, int width, int height,
                            int orientation);

    /**
     * Return the real DPI of the display containing given window
     *
     * On X11, it can indicate the real horizontal DPI value without any Xrdb
     * setting. Otherwise, returns the same as realDpiX/Y(),
     *
     * @since 22.04
     */
    static QSizeF realDpi(const QWindow* windowOnScreen);

    /**
     * Compute the smallest rectangle that contains all non-white pixels in
     * image), in normalized [0,1] coordinates.
     *
     * @since 0.7 (KDE 4.1)
     */
    static NormalizedRect imageBoundingBox(const QImage* image);
};

}  // namespace Okular

#endif

/* kate: replace-tabs on; indent-width 4; */
