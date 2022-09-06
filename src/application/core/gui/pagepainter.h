/*
    SPDX-FileCopyrightText: 2005 Enrico Ros <eros.kde@email.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PAGEPAINTER_H_
#define _OKULAR_PAGEPAINTER_H_

#include <QBrush>
#include <QImage>
#include <QPen>

#include "core/annotations.h"
#include "core/area.h" // for NormalizedPoint

class QPainter;
class QRect;
namespace Okular
{
class DocumentObserver;
class Page;
}

/**
 * @short Paints a Okular::Page to an open painter using given flags.
 */
class Q_DECL_EXPORT PagePainter
{
public:
    // list of flags passed to the painting function. by OR-ing those flags
    // you can decide whether or not to permit drawing of a certain feature.
    enum PagePainterFlags { Accessibility = 1, EnhanceLinks = 2, EnhanceImages = 4, Highlights = 8, TextSelection = 16, Annotations = 32 };

    /**
     * Draw @p page on @p destPainter.
     *
     * @param destPainter Page will be drawn on this painter.
     * @param page Which page do draw.
     * @param observer Request pixmaps generated for this DocumentObserver.
     * @param flags PagePainterFlags, which features to draw.
     * @param scaledWidth The requested width of uncropped page in @p destPainter coordinates.
     * @param scaledHeight The requested height of uncropped page in @p destPainter coordinates.
     * @param pageLimits Where to paint in @p destPainter coordinates. (I. e. painter crop.) Should begin at (0, 0).
     */
    static void paintPageOnPainter(QPainter *destPainter, const Okular::Page *page, Okular::DocumentObserver *observer, int flags, int scaledWidth, int scaledHeight, const QRect pageLimits);

    /**
     * Draw @p page on @p destPainter.
     *
     * @param destPainter Page will be drawn on this painter.
     * @param page Which page do draw.
     * @param observer Request pixmaps generated for this DocumentObserver.
     * @param flags PagePainterFlags, which features to draw.
     * @param scaledWidth The requested width of uncropped page in @p destPainter coordinates.
     * @param scaledHeight The requested height of uncropped page in @p destPainter coordinates.
     * @param pageLimits Where to paint in @p destPainter coordinates. (I. e. painter crop.) Should begin at (0, 0).
     * @param crop Which area of the page to paint in @p pageLimits.
     * @param viewPortPoint Which point of the page to highlight, e. g. a source location. @c nullptr to disable.
     */
    static void paintCroppedPageOnPainter(QPainter *destPainter,
                                          const Okular::Page *page,
                                          Okular::DocumentObserver *observer,
                                          int flags,
                                          int scaledWidth,
                                          int scaledHeight,
                                          const QRect pageLimits,
                                          const Okular::NormalizedRect &crop,
                                          Okular::NormalizedPoint *viewPortPoint);

private:
    // BEGIN Change Colors feature
    /**
     * Collapse color space (from white to black) to a line from @p foreground to @p background.
     */
    static void recolor(QImage *image, const QColor &foreground, const QColor &background);
    /**
     * Collapse color space to a line from white to black,
     * then move from @p threshold to 128 and stretch the line by @p contrast.
     */
    static void blackWhite(QImage *image, int contrast, int threshold);
    /**
     * Invert the lightness axis of the HSL color cone.
     */
    static void invertLightness(QImage *image);
    /**
     * Inverts luma of @p image using the luma coefficients @p Y_R, @p Y_G, @p Y_B (should sum up to 1),
     * and assuming linear 8bit RGB color space.
     */
    static void invertLuma(QImage *image, float Y_R, float Y_G, float Y_B);
    /**
     * Inverts luma of a pixel given in @p R, @p G, @p B,
     * using the luma coefficients @p Y_R, @p Y_G, @p Y_B (should sum up to 1),
     * and assuming linear 8bit RGB color space.
     */
    static void invertLumaPixel(uchar &R, uchar &G, uchar &B, float Y_R, float Y_G, float Y_B);
    /**
     * Shifts hue of each pixel by 120 degrees, by simply swapping channels.
     */
    static void hueShiftPositive(QImage *image);
    /**
     * Shifts hue of each pixel by 240 degrees, by simply swapping channels.
     */
    static void hueShiftNegative(QImage *image);
    // END Change Colors feature

    // my pretty dear raster function
    typedef QList<Okular::NormalizedPoint> NormalizedPath;
    enum RasterOperation { Normal, Multiply };

    /**
     * Draw @p normPath on @p image.
     *
     * @note @p normPath needs to be normalized in respect to @p image, not to the actual page.
     */
    static void drawShapeOnImage(QImage &image, const NormalizedPath &normPath, bool closeShape, const QPen &pen, const QBrush &brush = QBrush(), double penWidthMultiplier = 1.0, RasterOperation op = Normal);

    /**
     * Draw an ellipse described by @p rect on @p image.
     *
     * @param rect Two NormalizedPoints describing the bounding rect. Need to be normalized in respect to @p image, not to the actual page.
     */
    static void drawEllipseOnImage(QImage &image, const NormalizedPath &rect, const QPen &pen, const QBrush &brush, double penWidthMultiplier, RasterOperation op = Normal);

    friend class LineAnnotPainter;
};

/**
 * @short Painting helper for a single Okular::LineAnnotation.
 */
class LineAnnotPainter
{
public:
    /**
     * @param a The annotation to paint. Accessed by draw().
     * @param pageSizeA The full size of the page on which to paint.
     * @param pageScale The scale of the page when you call draw().
     * @param toNormalizedImage How to transform normalized coordinates of @p a to normalized coordinates of your paint device. (If your paint device represents the whole page, use the unit matrix QTransform().)
     */
    LineAnnotPainter(const Okular::LineAnnotation *a, QSizeF pageSizeA, double pageScale, const QTransform &toNormalizedImage);

    /**
     * Draw the annotation on @p image.
     */
    void draw(QImage &image) const;

private:
    void drawMainLine(QImage &image) const;
    void drawShortenedLine(double mainSegmentLength, double size, QImage &image, const QTransform &toNormalizedPage) const;
    void drawLineEnds(double mainSegmentLength, double size, QImage &image, const QTransform &transform) const;
    void drawLineEndArrow(double xEndPos, double size, double flipX, bool close, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLineEndButt(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLineEndCircle(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLineEndSquare(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLineEndDiamond(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLineEndSlash(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const;
    void drawLeaderLine(double xEndPos, QImage &image, const QTransform &toNormalizedPage) const;
    template<typename T> QList<Okular::NormalizedPoint> transformPath(const T &path, const QTransform &transform) const
    {
        QList<Okular::NormalizedPoint> transformedPath;
        for (const Okular::NormalizedPoint &item : path) {
            Okular::NormalizedPoint p;
            transform.map(item.x, item.y, &p.x, &p.y);
            transformedPath.append(p);
        }
        return transformedPath;
    }

    static double shortenForArrow(double size, Okular::LineAnnotation::TermStyle endStyle);

private:
    const Okular::LineAnnotation *la;
    QSizeF pageSize;
    double pageScale;
    QTransform toNormalizedImage;
    double aspectRatio;
    const QPen linePen;
    QBrush fillBrush;
};

#endif
