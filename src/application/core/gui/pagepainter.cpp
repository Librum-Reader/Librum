/*
    SPDX-FileCopyrightText: 2005 Enrico Ros <eros.kde@email.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pagepainter.h"

// qt / kde includes
#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QRect>
#include <QTransform>
#include <QVarLengthArray>

// system includes
#include <math.h>

// local includes
#include "core/annotations.h"
#include "core/observer.h"
#include "core/page.h"
#include "core/page_p.h"
#include "core/tile.h"
#include "core/utils.h"
#include "debug_ui.h"
#include "guiutils.h"
#include "settings.h"
#include "settings_core.h"

Q_GLOBAL_STATIC_WITH_ARGS(QPixmap, busyPixmap, (QIcon::fromTheme(QLatin1String("okular")).pixmap(48)))

#define TEXTANNOTATION_ICONSIZE 24

inline QPen buildPen(const Okular::Annotation *ann, double width, const QColor &color)
{
    QColor c = color;
    c.setAlphaF(ann->style().opacity());
    QPen p(QBrush(c), width, ann->style().lineStyle() == Okular::Annotation::Dashed ? Qt::DashLine : Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    return p;
}

void PagePainter::paintPageOnPainter(QPainter *destPainter, const Okular::Page *page, Okular::DocumentObserver *observer, int flags, int scaledWidth, int scaledHeight, const QRect limits)
{
    paintCroppedPageOnPainter(destPainter, page, observer, flags, scaledWidth, scaledHeight, limits, Okular::NormalizedRect(0, 0, 1, 1), nullptr);
}

void PagePainter::paintCroppedPageOnPainter(QPainter *destPainter,
                                            const Okular::Page *page,
                                            Okular::DocumentObserver *observer,
                                            int flags,
                                            int scaledWidth,
                                            int scaledHeight,
                                            const QRect limits,
                                            const Okular::NormalizedRect &crop,
                                            Okular::NormalizedPoint *viewPortPoint)
{
    qreal dpr = destPainter->device()->devicePixelRatioF();

    /* Calculate the cropped geometry of the page */
    QRect scaledCrop = crop.geometry(scaledWidth, scaledHeight);

    /* variables prefixed with d are in the device pixels coordinate system, which translates to the rendered output - that means,
     * multiplied with the device pixel ratio of the target PaintDevice */
    const QRect dScaledCrop(QRectF(scaledCrop.x() * dpr, scaledCrop.y() * dpr, scaledCrop.width() * dpr, scaledCrop.height() * dpr).toAlignedRect());

    int croppedWidth = scaledCrop.width();
    int croppedHeight = scaledCrop.height();

    int dScaledWidth = ceil(scaledWidth * dpr);
    int dScaledHeight = ceil(scaledHeight * dpr);
    const QRect dLimits(QRectF(limits.x() * dpr, limits.y() * dpr, limits.width() * dpr, limits.height() * dpr).toAlignedRect());

    QColor paperColor = Qt::white;
    QColor backgroundColor = paperColor;
    if (Okular::SettingsCore::changeColors()) {
        switch (Okular::SettingsCore::renderMode()) {
        case Okular::SettingsCore::EnumRenderMode::Inverted:
        case Okular::SettingsCore::EnumRenderMode::InvertLightness:
        case Okular::SettingsCore::EnumRenderMode::InvertLuma:
        case Okular::SettingsCore::EnumRenderMode::InvertLumaSymmetric:
            backgroundColor = Qt::black;
            break;
        case Okular::SettingsCore::EnumRenderMode::Paper:
            paperColor = Okular::SettingsCore::paperColor();
            backgroundColor = paperColor;
            break;
        case Okular::SettingsCore::EnumRenderMode::Recolor:
            backgroundColor = Okular::Settings::recolorBackground();
            break;
        default:;
        }
    }
    destPainter->fillRect(limits, backgroundColor);

    const bool hasTilesManager = page->hasTilesManager(observer);
    QPixmap pixmap;

    if (!hasTilesManager) {
        /** 1 - RETRIEVE THE 'PAGE+ID' PIXMAP OR A SIMILAR 'PAGE' ONE **/
        const QPixmap *p = page->_o_nearestPixmap(observer, dScaledWidth, dScaledHeight);

        if (p != nullptr) {
            pixmap = *p;
            pixmap.setDevicePixelRatio(dpr);
        }

        /** 1B - IF NO PIXMAP, DRAW EMPTY PAGE **/
        double pixmapRescaleRatio = !pixmap.isNull() ? dScaledWidth / (double)pixmap.width() : -1;
        long pixmapPixels = !pixmap.isNull() ? (long)pixmap.width() * (long)pixmap.height() : 0;
        if (pixmap.isNull() || pixmapRescaleRatio > 20.0 || pixmapRescaleRatio < 0.25 || (dScaledWidth > pixmap.width() && pixmapPixels > 60000000L)) {
            // draw something on the blank page: the okular icon or a cross (as a fallback)
            if (!busyPixmap()->isNull()) {
                busyPixmap->setDevicePixelRatio(dpr);
                destPainter->drawPixmap(QPoint(10, 10), *busyPixmap());
            } else {
                destPainter->setPen(Qt::gray);
                destPainter->drawLine(0, 0, croppedWidth - 1, croppedHeight - 1);
                destPainter->drawLine(0, croppedHeight - 1, croppedWidth - 1, 0);
            }
            return;
        }
    }

    /** 2 - FIND OUT WHAT TO PAINT (Flags + Configuration + Presence) **/
    bool canDrawHighlights = (flags & Highlights) && !page->m_highlights.isEmpty();
    bool canDrawTextSelection = (flags & TextSelection) && page->textSelection();
    bool canDrawAnnotations = (flags & Annotations) && !page->m_annotations.isEmpty();
    bool enhanceLinks = (flags & EnhanceLinks) && Okular::Settings::highlightLinks();
    bool enhanceImages = (flags & EnhanceImages) && Okular::Settings::highlightImages();

    // vectors containing objects to draw
    // make this a qcolor, rect map, since we don't need
    // to know s_id here! we are only drawing this right?
    QList<QPair<QColor, Okular::NormalizedRect>> *bufferedHighlights = nullptr;
    QList<Okular::Annotation *> *bufferedAnnotations = nullptr;
    QList<Okular::Annotation *> *unbufferedAnnotations = nullptr;
    Okular::Annotation *boundingRectOnlyAnn = nullptr; // Paint the bounding rect of this annotation
    // fill up lists with visible annotation/highlight objects/text selections
    if (canDrawHighlights || canDrawTextSelection || canDrawAnnotations) {
        // precalc normalized 'limits rect' for intersection
        double nXMin = ((double)limits.left() / scaledWidth) + crop.left, nXMax = ((double)limits.right() / scaledWidth) + crop.left, nYMin = ((double)limits.top() / scaledHeight) + crop.top,
               nYMax = ((double)limits.bottom() / scaledHeight) + crop.top;
        // append all highlights inside limits to their list
        if (canDrawHighlights) {
            if (!bufferedHighlights) {
                bufferedHighlights = new QList<QPair<QColor, Okular::NormalizedRect>>();
            }
            /*            else
                        {*/

            Okular::NormalizedRect *limitRect = new Okular::NormalizedRect(nXMin, nYMin, nXMax, nYMax);
            Okular::HighlightAreaRect::const_iterator hIt;
            for (const Okular::HighlightAreaRect *highlight : page->m_highlights) {
                for (hIt = highlight->constBegin(); hIt != highlight->constEnd(); ++hIt) {
                    if ((*hIt).intersects(limitRect)) {
                        bufferedHighlights->append(qMakePair(highlight->color, *hIt));
                    }
                }
            }
            delete limitRect;
            //}
        }
        if (canDrawTextSelection) {
            if (!bufferedHighlights) {
                bufferedHighlights = new QList<QPair<QColor, Okular::NormalizedRect>>();
            }
            /*            else
                        {*/
            Okular::NormalizedRect *limitRect = new Okular::NormalizedRect(nXMin, nYMin, nXMax, nYMax);
            const Okular::RegularAreaRect *textSelection = page->textSelection();
            Okular::HighlightAreaRect::const_iterator hIt = textSelection->constBegin(), hEnd = textSelection->constEnd();
            for (; hIt != hEnd; ++hIt) {
                if ((*hIt).intersects(limitRect)) {
                    bufferedHighlights->append(qMakePair(page->textSelectionColor(), *hIt));
                }
            }
            delete limitRect;
            //}
        }
        // append annotations inside limits to the un/buffered list
        if (canDrawAnnotations) {
            for (Okular::Annotation *ann : page->m_annotations) {
                int flags = ann->flags();

                if (flags & Okular::Annotation::Hidden) {
                    continue;
                }

                if (flags & Okular::Annotation::ExternallyDrawn) {
                    // ExternallyDrawn annots are never rendered by PagePainter.
                    // Just paint the boundingRect if the annot is moved or resized.
                    if (flags & (Okular::Annotation::BeingMoved | Okular::Annotation::BeingResized)) {
                        boundingRectOnlyAnn = ann;
                    }
                    continue;
                }

                bool intersects = ann->transformedBoundingRectangle().intersects(nXMin, nYMin, nXMax, nYMax);
                if (ann->subType() == Okular::Annotation::AText) {
                    Okular::TextAnnotation *ta = static_cast<Okular::TextAnnotation *>(ann);
                    if (ta->textType() == Okular::TextAnnotation::Linked) {
                        Okular::NormalizedRect iconrect(ann->transformedBoundingRectangle().left,
                                                        ann->transformedBoundingRectangle().top,
                                                        ann->transformedBoundingRectangle().left + TEXTANNOTATION_ICONSIZE / page->width(),
                                                        ann->transformedBoundingRectangle().top + TEXTANNOTATION_ICONSIZE / page->height());
                        intersects = iconrect.intersects(nXMin, nYMin, nXMax, nYMax);
                    }
                }
                if (intersects) {
                    Okular::Annotation::SubType type = ann->subType();
                    if (type == Okular::Annotation::ALine || type == Okular::Annotation::AHighlight || type == Okular::Annotation::AInk /*|| (type == Annotation::AGeom && ann->style().opacity() < 0.99)*/) {
                        if (!bufferedAnnotations) {
                            bufferedAnnotations = new QList<Okular::Annotation *>();
                        }
                        bufferedAnnotations->append(ann);
                    } else {
                        if (!unbufferedAnnotations) {
                            unbufferedAnnotations = new QList<Okular::Annotation *>();
                        }
                        unbufferedAnnotations->append(ann);
                    }
                }
            }
        }
        // end of intersections checking
    }

    /** 3 - ENABLE BACKBUFFERING IF DIRECT IMAGE MANIPULATION IS NEEDED **/
    bool bufferAccessibility = (flags & Accessibility) && Okular::SettingsCore::changeColors() && (Okular::SettingsCore::renderMode() != Okular::SettingsCore::EnumRenderMode::Paper);
    bool useBackBuffer = bufferAccessibility || bufferedHighlights || bufferedAnnotations || viewPortPoint;
    QPixmap *backPixmap = nullptr;
    QPainter *mixedPainter = nullptr;
    QRect limitsInPixmap = limits.translated(scaledCrop.topLeft());
    QRect dLimitsInPixmap = dLimits.translated(dScaledCrop.topLeft());

    // limits within full (scaled but uncropped) pixmap

    /** 4A -- REGULAR FLOW. PAINT PIXMAP NORMAL OR RESCALED USING GIVEN QPAINTER **/
    if (!useBackBuffer) {
        if (hasTilesManager) {
            const Okular::NormalizedRect normalizedLimits(limitsInPixmap, scaledWidth, scaledHeight);
            const QList<Okular::Tile> tiles = page->tilesAt(observer, normalizedLimits);
            QList<Okular::Tile>::const_iterator tIt = tiles.constBegin(), tEnd = tiles.constEnd();
            while (tIt != tEnd) {
                const Okular::Tile &tile = *tIt;
                QRect tileRect = tile.rect().geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft());
                QRect dTileRect = QRectF(tileRect.x() * dpr, tileRect.y() * dpr, tileRect.width() * dpr, tileRect.height() * dpr).toAlignedRect();
                QRect limitsInTile = limits & tileRect;
                QRectF dLimitsInTile = dLimits & dTileRect;

                if (!limitsInTile.isEmpty()) {
                    QPixmap *tilePixmap = tile.pixmap();
                    tilePixmap->setDevicePixelRatio(dpr);

                    if (tilePixmap->width() == dTileRect.width() && tilePixmap->height() == dTileRect.height()) {
                        destPainter->drawPixmap(limitsInTile.topLeft(), *tilePixmap, dLimitsInTile.translated(-dTileRect.topLeft()));
                    } else {
                        destPainter->drawPixmap(tileRect, *tilePixmap);
                    }
                }
                tIt++;
            }
        } else {
            QPixmap scaledCroppedPixmap = pixmap.scaled(dScaledWidth, dScaledHeight).copy(dLimitsInPixmap);
            scaledCroppedPixmap.setDevicePixelRatio(dpr);
            destPainter->drawPixmap(limits.topLeft(), scaledCroppedPixmap, QRectF(0, 0, dLimits.width(), dLimits.height()));
        }

        // 4A.2. active painter is the one passed to this method
        mixedPainter = destPainter;
    }
    /** 4B -- BUFFERED FLOW. IMAGE PAINTING + OPERATIONS. QPAINTER OVER PIXMAP  **/
    else {
        // the image over which we are going to draw
        QImage backImage = QImage(dLimits.width(), dLimits.height(), QImage::Format_ARGB32_Premultiplied);
        backImage.setDevicePixelRatio(dpr);
        backImage.fill(paperColor);
        QPainter p(&backImage);

        if (hasTilesManager) {
            const Okular::NormalizedRect normalizedLimits(limitsInPixmap, scaledWidth, scaledHeight);
            const QList<Okular::Tile> tiles = page->tilesAt(observer, normalizedLimits);
            QList<Okular::Tile>::const_iterator tIt = tiles.constBegin(), tEnd = tiles.constEnd();
            while (tIt != tEnd) {
                const Okular::Tile &tile = *tIt;
                QRect tileRect = tile.rect().geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft());
                QRect dTileRect(QRectF(tileRect.x() * dpr, tileRect.y() * dpr, tileRect.width() * dpr, tileRect.height() * dpr).toAlignedRect());
                QRect limitsInTile = limits & tileRect;
                QRect dLimitsInTile = dLimits & dTileRect;

                if (!limitsInTile.isEmpty()) {
                    QPixmap *tilePixmap = tile.pixmap();
                    tilePixmap->setDevicePixelRatio(dpr);

                    if (tilePixmap->width() == dTileRect.width() && tilePixmap->height() == dTileRect.height()) {
                        p.drawPixmap(limitsInTile.translated(-limits.topLeft()).topLeft(), *tilePixmap, dLimitsInTile.translated(-dTileRect.topLeft()));
                    } else {
                        double xScale = tilePixmap->width() / (double)dTileRect.width();
                        double yScale = tilePixmap->height() / (double)dTileRect.height();
                        QTransform transform(xScale, 0, 0, yScale, 0, 0);
                        p.drawPixmap(limitsInTile.translated(-limits.topLeft()), *tilePixmap, transform.mapRect(dLimitsInTile).translated(-transform.mapRect(dTileRect).topLeft()));
                    }
                }
                ++tIt;
            }
        } else {
            // 4B.1. draw the page pixmap: normal or scaled
            QPixmap scaledCroppedPixmap = pixmap.scaled(dScaledWidth, dScaledHeight).copy(dLimitsInPixmap);
            scaledCroppedPixmap.setDevicePixelRatio(dpr);
            p.drawPixmap(0, 0, scaledCroppedPixmap);
        }

        p.end();

        // 4B.2. modify pixmap following accessibility settings
        if (bufferAccessibility) {
            switch (Okular::SettingsCore::renderMode()) {
            case Okular::SettingsCore::EnumRenderMode::Inverted:
                // Invert image pixels using QImage internal function
                backImage.invertPixels(QImage::InvertRgb);
                break;
            case Okular::SettingsCore::EnumRenderMode::Recolor:
                recolor(&backImage, Okular::Settings::recolorForeground(), Okular::Settings::recolorBackground());
                break;
            case Okular::SettingsCore::EnumRenderMode::BlackWhite:
                blackWhite(&backImage, Okular::Settings::bWContrast(), Okular::Settings::bWThreshold());
                break;
            case Okular::SettingsCore::EnumRenderMode::InvertLightness:
                invertLightness(&backImage);
                break;
            case Okular::SettingsCore::EnumRenderMode::InvertLuma:
                invertLuma(&backImage, 0.2126, 0.7152, 0.0722); // sRGB / Rec. 709 luma coefficients
                break;
            case Okular::SettingsCore::EnumRenderMode::InvertLumaSymmetric:
                invertLuma(&backImage, 0.3333, 0.3334, 0.3333); // Symmetric coefficients, to keep colors saturated.
                break;
            case Okular::SettingsCore::EnumRenderMode::HueShiftPositive:
                hueShiftPositive(&backImage);
                break;
            case Okular::SettingsCore::EnumRenderMode::HueShiftNegative:
                hueShiftNegative(&backImage);
                break;
            }
        }

        // 4B.3. highlight rects in page
        if (bufferedHighlights) {
            // draw highlights that are inside the 'limits' paint region
            for (const auto &highlight : qAsConst(*bufferedHighlights)) {
                const Okular::NormalizedRect &r = highlight.second;
                // find out the rect to highlight on pixmap
                QRect highlightRect = r.geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft()).intersected(limits);
                highlightRect.translate(-limits.left(), -limits.top());

                const QColor highlightColor = highlight.first;
                QPainter painter(&backImage);
                painter.setCompositionMode(QPainter::CompositionMode_Multiply);
                painter.fillRect(highlightRect, highlightColor);

                auto frameColor = highlightColor.darker(150);
                const QRect frameRect = r.geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft()).translated(-limits.left(), -limits.top());
                painter.setPen(frameColor);
                painter.drawRect(frameRect);
            }
        }

        // 4B.4. paint annotations [COMPOSITED ONES]
        if (bufferedAnnotations) {
            // Albert: This is quite "heavy" but all the backImage that reach here are QImage::Format_ARGB32_Premultiplied
            // and have to be so that the QPainter::CompositionMode_Multiply works
            // we could also put a
            // backImage = backImage.convertToFormat(QImage::Format_ARGB32_Premultiplied)
            // that would be almost a noop, but we'll leave the assert for now
            Q_ASSERT(backImage.format() == QImage::Format_ARGB32_Premultiplied);
            // precalc constants for normalizing [0,1] page coordinates into normalized [0,1] limit rect coordinates
            double pageScale = (double)croppedWidth / page->width();
            double xOffset = (double)limits.left() / (double)scaledWidth + crop.left, xScale = (double)scaledWidth / (double)limits.width(), yOffset = (double)limits.top() / (double)scaledHeight + crop.top,
                   yScale = (double)scaledHeight / (double)limits.height();

            // paint all buffered annotations in the page
            QList<Okular::Annotation *>::const_iterator aIt = bufferedAnnotations->constBegin(), aEnd = bufferedAnnotations->constEnd();
            for (; aIt != aEnd; ++aIt) {
                Okular::Annotation *a = *aIt;
                Okular::Annotation::SubType type = a->subType();
                QColor acolor = a->style().color();
                if (!acolor.isValid()) {
                    acolor = Qt::yellow;
                }
                acolor.setAlphaF(a->style().opacity());

                // draw LineAnnotation MISSING: caption, dash pattern, endings for multipoint lines
                if (type == Okular::Annotation::ALine) {
                    LineAnnotPainter linepainter {(Okular::LineAnnotation *)a, {page->width(), page->height()}, pageScale, {xScale, 0., 0., yScale, -xOffset * xScale, -yOffset * yScale}};
                    linepainter.draw(backImage);
                }
                // draw HighlightAnnotation MISSING: under/strike width, feather, capping
                else if (type == Okular::Annotation::AHighlight) {
                    // get the annotation
                    Okular::HighlightAnnotation *ha = (Okular::HighlightAnnotation *)a;
                    Okular::HighlightAnnotation::HighlightType type = ha->highlightType();

                    // draw each quad of the annotation
                    int quads = ha->highlightQuads().size();
                    for (int q = 0; q < quads; q++) {
                        NormalizedPath path;
                        const Okular::HighlightAnnotation::Quad &quad = ha->highlightQuads()[q];
                        // normalize page point to image
                        for (int i = 0; i < 4; i++) {
                            Okular::NormalizedPoint point;
                            point.x = (quad.transformedPoint(i).x - xOffset) * xScale;
                            point.y = (quad.transformedPoint(i).y - yOffset) * yScale;
                            path.append(point);
                        }
                        // draw the normalized path into image
                        switch (type) {
                        // highlight the whole rect
                        case Okular::HighlightAnnotation::Highlight:
                            drawShapeOnImage(backImage, path, true, Qt::NoPen, acolor, pageScale, Multiply);
                            break;
                        // highlight the bottom part of the rect
                        case Okular::HighlightAnnotation::Squiggly:
                            path[3].x = (path[0].x + path[3].x) / 2.0;
                            path[3].y = (path[0].y + path[3].y) / 2.0;
                            path[2].x = (path[1].x + path[2].x) / 2.0;
                            path[2].y = (path[1].y + path[2].y) / 2.0;
                            drawShapeOnImage(backImage, path, true, Qt::NoPen, acolor, pageScale, Multiply);
                            break;
                        // make a line at 3/4 of the height
                        case Okular::HighlightAnnotation::Underline:
                            path[0].x = (3 * path[0].x + path[3].x) / 4.0;
                            path[0].y = (3 * path[0].y + path[3].y) / 4.0;
                            path[1].x = (3 * path[1].x + path[2].x) / 4.0;
                            path[1].y = (3 * path[1].y + path[2].y) / 4.0;
                            path.pop_back();
                            path.pop_back();
                            drawShapeOnImage(backImage, path, false, QPen(acolor, 2), QBrush(), pageScale);
                            break;
                        // make a line at 1/2 of the height
                        case Okular::HighlightAnnotation::StrikeOut:
                            path[0].x = (path[0].x + path[3].x) / 2.0;
                            path[0].y = (path[0].y + path[3].y) / 2.0;
                            path[1].x = (path[1].x + path[2].x) / 2.0;
                            path[1].y = (path[1].y + path[2].y) / 2.0;
                            path.pop_back();
                            path.pop_back();
                            drawShapeOnImage(backImage, path, false, QPen(acolor, 2), QBrush(), pageScale);
                            break;
                        }
                    }
                }
                // draw InkAnnotation MISSING:invar width, PENTRACER
                else if (type == Okular::Annotation::AInk) {
                    // get the annotation
                    Okular::InkAnnotation *ia = (Okular::InkAnnotation *)a;

                    // draw each ink path
                    const QList<QList<Okular::NormalizedPoint>> transformedInkPaths = ia->transformedInkPaths();

                    const QPen inkPen = buildPen(a, a->style().width(), acolor);

                    for (const QList<Okular::NormalizedPoint> &inkPath : transformedInkPaths) {
                        // normalize page point to image
                        NormalizedPath path;
                        for (const Okular::NormalizedPoint &inkPoint : inkPath) {
                            Okular::NormalizedPoint point;
                            point.x = (inkPoint.x - xOffset) * xScale;
                            point.y = (inkPoint.y - yOffset) * yScale;
                            path.append(point);
                        }
                        // draw the normalized path into image
                        drawShapeOnImage(backImage, path, false, inkPen, QBrush(), pageScale);
                    }
                }
            } // end current annotation drawing
        }
        if (viewPortPoint) {
            QPainter painter(&backImage);
            painter.translate(-limits.left(), -limits.top());
            painter.setPen(QApplication::palette().color(QPalette::Active, QPalette::Highlight));
            painter.drawLine(0, viewPortPoint->y * scaledHeight + 1, scaledWidth - 1, viewPortPoint->y * scaledHeight + 1);
            // ROTATION CURRENTLY NOT IMPLEMENTED
            /*
                        if( page->rotation() == Okular::Rotation0)
                        {

                        }
                        else if(page->rotation() == Okular::Rotation270)
                        {
                            painter.drawLine( viewPortPoint->y * scaledHeight + 1, 0, viewPortPoint->y * scaledHeight + 1, scaledWidth - 1);
                        }
                        else if(page->rotation() == Okular::Rotation180)
                        {
                            painter.drawLine( 0, (1.0 - viewPortPoint->y) * scaledHeight - 1, scaledWidth - 1, (1.0 - viewPortPoint->y) * scaledHeight - 1 );
                        }
                        else if(page->rotation() == Okular::Rotation90) // not right, rotation clock-wise
                        {
                            painter.drawLine( scaledWidth - (viewPortPoint->y * scaledHeight + 1), 0, scaledWidth - (viewPortPoint->y * scaledHeight + 1), scaledWidth - 1);
                        }
            */
        }

        // 4B.5. create the back pixmap converting from the local image
        backPixmap = new QPixmap(QPixmap::fromImage(backImage));
        backPixmap->setDevicePixelRatio(dpr);

        // 4B.6. create a painter over the pixmap and set it as the active one
        mixedPainter = new QPainter(backPixmap);
        mixedPainter->translate(-limits.left(), -limits.top());
    }

    /** 5 -- MIXED FLOW. Draw ANNOTATIONS [OPAQUE ONES] on ACTIVE PAINTER  **/
    if (unbufferedAnnotations) {
        // iterate over annotations and paint AText, AGeom, AStamp
        QList<Okular::Annotation *>::const_iterator aIt = unbufferedAnnotations->constBegin(), aEnd = unbufferedAnnotations->constEnd();
        for (; aIt != aEnd; ++aIt) {
            Okular::Annotation *a = *aIt;

            // honor opacity settings on supported types
            unsigned int opacity = (unsigned int)(a->style().color().alpha() * a->style().opacity());
            // skip the annotation drawing if all the annotation is fully
            // transparent, but not with text annotations
            if (opacity <= 0 && a->subType() != Okular::Annotation::AText) {
                continue;
            }

            QColor acolor = a->style().color();
            if (!acolor.isValid()) {
                acolor = Qt::yellow;
            }
            acolor.setAlpha(opacity);

            // Annotation boundary in destPainter coordinates:
            QRect annotBoundary = a->transformedBoundingRectangle().geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft());
            QRect annotRect = annotBoundary.intersected(limits);
            // Visible portion of the annotation at annotBoundary size:
            QRect innerRect = annotRect.translated(-annotBoundary.topLeft());
            QRectF dInnerRect(innerRect.x() * dpr, innerRect.y() * dpr, innerRect.width() * dpr, innerRect.height() * dpr);

            Okular::Annotation::SubType type = a->subType();

            // draw TextAnnotation
            if (type == Okular::Annotation::AText) {
                Okular::TextAnnotation *text = (Okular::TextAnnotation *)a;
                if (text->textType() == Okular::TextAnnotation::InPlace) {
                    QImage image(annotBoundary.size(), QImage::Format_ARGB32);
                    image.fill(acolor.rgba());
                    QPainter painter(&image);
                    painter.setFont(text->textFont());
                    painter.setPen(text->textColor());
                    Qt::AlignmentFlag halign = (text->inplaceAlignment() == 1 ? Qt::AlignHCenter : (text->inplaceAlignment() == 2 ? Qt::AlignRight : Qt::AlignLeft));
                    const double invXScale = (double)page->width() / scaledWidth;
                    const double invYScale = (double)page->height() / scaledHeight;
                    const double borderWidth = text->style().width();
                    painter.scale(1 / invXScale, 1 / invYScale);
                    painter.drawText(
                        borderWidth * invXScale, borderWidth * invYScale, (image.width() - 2 * borderWidth) * invXScale, (image.height() - 2 * borderWidth) * invYScale, Qt::AlignTop | halign | Qt::TextWordWrap, text->contents());
                    painter.resetTransform();
                    // Required as asking for a zero width pen results
                    // in a default width pen (1.0) being created
                    if (borderWidth != 0) {
                        QPen pen(Qt::black, borderWidth);
                        painter.setPen(pen);
                        painter.drawRect(0, 0, image.width() - 1, image.height() - 1);
                    }
                    painter.end();

                    mixedPainter->drawImage(annotBoundary.topLeft(), image);
                } else if (text->textType() == Okular::TextAnnotation::Linked) {
                    // get pixmap, colorize and alpha-blend it
                    QPixmap pixmap = QIcon::fromTheme(text->textIcon().toLower()).pixmap(32);

                    QPixmap scaledCroppedPixmap = pixmap.scaled(TEXTANNOTATION_ICONSIZE * dpr, TEXTANNOTATION_ICONSIZE * dpr).copy(dInnerRect.toAlignedRect());
                    scaledCroppedPixmap.setDevicePixelRatio(dpr);
                    QImage scaledCroppedImage = scaledCroppedPixmap.toImage();

                    // if the annotation color is valid (ie it was set), then
                    // use it to colorize the icon, otherwise the icon will be
                    // "gray"
                    if (a->style().color().isValid()) {
                        GuiUtils::colorizeImage(scaledCroppedImage, a->style().color(), opacity);
                    }
                    pixmap = QPixmap::fromImage(scaledCroppedImage);

                    // draw the mangled image to painter
                    mixedPainter->drawPixmap(annotRect.topLeft(), pixmap);
                }

            }
            // draw StampAnnotation
            else if (type == Okular::Annotation::AStamp) {
                Okular::StampAnnotation *stamp = (Okular::StampAnnotation *)a;

                // get pixmap and alpha blend it if needed
                QPixmap pixmap = Okular::AnnotationUtils::loadStamp(stamp->stampIconName(), qMax(annotBoundary.width(), annotBoundary.height()) * dpr);
                if (!pixmap.isNull()) // should never happen but can happen on huge sizes
                {
                    QPixmap scaledCroppedPixmap = pixmap.scaled(annotBoundary.width() * dpr, annotBoundary.height() * dpr).copy(dInnerRect.toAlignedRect());
                    scaledCroppedPixmap.setDevicePixelRatio(dpr);

                    // Draw pixmap with opacity:
                    mixedPainter->save();
                    mixedPainter->setOpacity(mixedPainter->opacity() * opacity / 255.0);
                    mixedPainter->drawPixmap(annotRect.topLeft(), scaledCroppedPixmap);
                    mixedPainter->restore();
                }
            }
            // draw GeomAnnotation
            else if (type == Okular::Annotation::AGeom) {
                Okular::GeomAnnotation *geom = (Okular::GeomAnnotation *)a;
                // check whether there's anything to draw
                if (geom->style().width() || geom->geometricalInnerColor().isValid()) {
                    mixedPainter->save();
                    const double width = geom->style().width() * Okular::Utils::realDpi(nullptr).width() / (72.0 * 2.0) * scaledWidth / page->width();
                    QRectF r(.0, .0, annotBoundary.width(), annotBoundary.height());
                    r.adjust(width, width, -width, -width);
                    r.translate(annotBoundary.topLeft());
                    if (geom->geometricalInnerColor().isValid()) {
                        r.adjust(width, width, -width, -width);
                        const QColor color = geom->geometricalInnerColor();
                        mixedPainter->setPen(Qt::NoPen);
                        mixedPainter->setBrush(QColor(color.red(), color.green(), color.blue(), opacity));
                        if (geom->geometricalType() == Okular::GeomAnnotation::InscribedSquare) {
                            mixedPainter->drawRect(r);
                        } else {
                            mixedPainter->drawEllipse(r);
                        }
                        r.adjust(-width, -width, width, width);
                    }
                    if (geom->style().width()) // need to check the original size here..
                    {
                        mixedPainter->setPen(buildPen(a, width * 2, acolor));
                        mixedPainter->setBrush(Qt::NoBrush);
                        if (geom->geometricalType() == Okular::GeomAnnotation::InscribedSquare) {
                            mixedPainter->drawRect(r);
                        } else {
                            mixedPainter->drawEllipse(r);
                        }
                    }
                    mixedPainter->restore();
                }
            }

            // draw extents rectangle
            if (Okular::Settings::debugDrawAnnotationRect()) {
                mixedPainter->setPen(a->style().color());
                mixedPainter->drawRect(annotBoundary);
            }
        }
    }

    if (boundingRectOnlyAnn) {
        QRect annotBoundary = boundingRectOnlyAnn->transformedBoundingRectangle().geometry(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft());
        mixedPainter->setPen(Qt::DashLine);
        mixedPainter->drawRect(annotBoundary);
    }

    /** 6 -- MIXED FLOW. Draw LINKS+IMAGES BORDER on ACTIVE PAINTER  **/
    if (enhanceLinks || enhanceImages) {
        mixedPainter->save();
        mixedPainter->scale(scaledWidth, scaledHeight);
        mixedPainter->translate(-crop.left, -crop.top);

        QColor normalColor = QApplication::palette().color(QPalette::Active, QPalette::Highlight);
        // enlarging limits for intersection is like growing the 'rectGeometry' below
        QRect limitsEnlarged = limits;
        limitsEnlarged.adjust(-2, -2, 2, 2);
        // draw rects that are inside the 'limits' paint region as opaque rects
        for (Okular::ObjectRect *rect : page->m_rects) {
            if ((enhanceLinks && rect->objectType() == Okular::ObjectRect::Action) || (enhanceImages && rect->objectType() == Okular::ObjectRect::Image)) {
                if (limitsEnlarged.intersects(rect->boundingRect(scaledWidth, scaledHeight).translated(-scaledCrop.topLeft()))) {
                    mixedPainter->strokePath(rect->region(), QPen(normalColor, 0));
                }
            }
        }
        mixedPainter->restore();
    }

    /** 7 -- BUFFERED FLOW. Copy BACKPIXMAP on DESTINATION PAINTER **/
    if (useBackBuffer) {
        delete mixedPainter;
        destPainter->drawPixmap(limits.left(), limits.top(), *backPixmap);
        delete backPixmap;
    }

    // delete object containers
    delete bufferedHighlights;
    delete bufferedAnnotations;
    delete unbufferedAnnotations;
}

void PagePainter::recolor(QImage *image, const QColor &foreground, const QColor &background)
{
    if (image->format() != QImage::Format_ARGB32_Premultiplied) {
        qCWarning(OkularUiDebug) << "Wrong image format! Converting...";
        *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    Q_ASSERT(image->format() == QImage::Format_ARGB32_Premultiplied);

    const float scaleRed = background.redF() - foreground.redF();
    const float scaleGreen = background.greenF() - foreground.greenF();
    const float scaleBlue = background.blueF() - foreground.blueF();

    const int foreground_red = foreground.red();
    const int foreground_green = foreground.green();
    const int foreground_blue = foreground.blue();

    QRgb *data = reinterpret_cast<QRgb *>(image->bits());
    const int pixels = image->width() * image->height();

    for (int i = 0; i < pixels; ++i) {
        const int lightness = qGray(data[i]);

        const float r = scaleRed * lightness + foreground_red;
        const float g = scaleGreen * lightness + foreground_green;
        const float b = scaleBlue * lightness + foreground_blue;

        const unsigned a = qAlpha(data[i]);
        data[i] = qRgba(r, g, b, a);
    }
}

void PagePainter::blackWhite(QImage *image, int contrast, int threshold)
{
    unsigned int *data = reinterpret_cast<unsigned int *>(image->bits());
    int con = contrast;
    int thr = 255 - threshold;

    int pixels = image->width() * image->height();
    for (int i = 0; i < pixels; ++i) {
        // Piecewise linear function of val, through (0, 0), (thr, 128), (255, 255)
        int val = qGray(data[i]);
        if (val > thr) {
            val = 128 + (127 * (val - thr)) / (255 - thr);
        } else if (val < thr) {
            val = (128 * val) / thr;
        }

        // Linear contrast stretching through (thr, thr)
        if (con > 2) {
            val = thr + (val - thr) * con / 2;
            val = qBound(0, val, 255);
        }

        const unsigned a = qAlpha(data[i]);
        data[i] = qRgba(val, val, val, a);
    }
}

void PagePainter::invertLightness(QImage *image)
{
    if (image->format() != QImage::Format_ARGB32_Premultiplied) {
        qCWarning(OkularUiDebug) << "Wrong image format! Converting...";
        *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    Q_ASSERT(image->format() == QImage::Format_ARGB32_Premultiplied);

    QRgb *data = reinterpret_cast<QRgb *>(image->bits());
    int pixels = image->width() * image->height();
    for (int i = 0; i < pixels; ++i) {
        // Invert lightness of the pixel using the cylindric HSL color model.
        // Algorithm is based on https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB (2019-03-17).
        // Important simplifications are that inverting lightness does not change chroma and hue.
        // This means the sector (of the chroma/hue plane) is not changed,
        // so we can use a linear calculation after determining the sector using qMin() and qMax().
        uchar R = qRed(data[i]);
        uchar G = qGreen(data[i]);
        uchar B = qBlue(data[i]);

        // Get only the needed HSL components. These are chroma C and the common component m.
        // Get common component m
        uchar m = qMin(R, qMin(G, B));
        // Remove m from color components
        R -= m;
        G -= m;
        B -= m;
        // Get chroma C
        uchar C = qMax(R, qMax(G, B));

        // Get common component m' after inverting lightness L.
        // Hint: Lightness L = m + C / 2; L' = 255 - L = 255 - (m + C / 2) => m' = 255 - C - m
        uchar m_ = 255 - C - m;

        // Add m' to color compontents
        R += m_;
        G += m_;
        B += m_;

        // Save new color
        const unsigned A = qAlpha(data[i]);
        data[i] = qRgba(R, G, B, A);
    }
}

void PagePainter::invertLuma(QImage *image, float Y_R, float Y_G, float Y_B)
{
    if (image->format() != QImage::Format_ARGB32_Premultiplied) {
        qCWarning(OkularUiDebug) << "Wrong image format! Converting...";
        *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    Q_ASSERT(image->format() == QImage::Format_ARGB32_Premultiplied);

    QRgb *data = reinterpret_cast<QRgb *>(image->bits());
    int pixels = image->width() * image->height();
    for (int i = 0; i < pixels; ++i) {
        uchar R = qRed(data[i]);
        uchar G = qGreen(data[i]);
        uchar B = qBlue(data[i]);

        invertLumaPixel(R, G, B, Y_R, Y_G, Y_B);

        // Save new color
        const unsigned A = qAlpha(data[i]);
        data[i] = qRgba(R, G, B, A);
    }
}

void PagePainter::invertLumaPixel(uchar &R, uchar &G, uchar &B, float Y_R, float Y_G, float Y_B)
{
    // Invert luma of the pixel using the bicone HCY color model, stretched to cylindric HSY.
    // Algorithm is based on https://en.wikipedia.org/wiki/HSL_and_HSV#Luma,_chroma_and_hue_to_RGB (2019-03-19).
    // For an illustration see https://experilous.com/1/product/make-it-colorful/ (2019-03-19).

    // Special case: The algorithm does not work when hue is undefined.
    if (R == G && G == B) {
        R = 255 - R;
        G = 255 - G;
        B = 255 - B;
        return;
    }

    // Get input and output luma Y, Y_inv in range 0..255
    float Y = R * Y_R + G * Y_G + B * Y_B;
    float Y_inv = 255 - Y;

    // Get common component m and remove from color components.
    // This moves us to the bottom faces of the HCY bicone, i. e. we get C and X in R, G, B.
    uint_fast8_t m = qMin(R, qMin(G, B));
    R -= m;
    G -= m;
    B -= m;

    // We operate in a hue plane of the luma/chroma/hue bicone.
    // The hue plane is a triangle.
    // This bicone is distorted, so we can not simply mirror the triangle.
    // We need to stretch it to a luma/saturation rectangle, so we need to stretch chroma C and the proportional X.

    // First, we need to calculate luma Y_full_C for the outer corner of the triangle.
    // Then we can interpolate the max chroma C_max, C_inv_max for our luma Y, Y_inv.
    // Then we calculate C_inv and X_inv by scaling them by the ratio of C_max and C_inv_max.

    // Calculate luma Y_full_C (in range equivalent to gray 0..255) for chroma = 1 at this hue.
    // Piecewise linear, with the corners of the bicone at the sum of one or two luma coefficients.
    float Y_full_C;
    if (R >= B && B >= G) {
        Y_full_C = 255 * Y_R + 255 * Y_B * B / R;
    } else if (R >= G && G >= B) {
        Y_full_C = 255 * Y_R + 255 * Y_G * G / R;
    } else if (G >= R && R >= B) {
        Y_full_C = 255 * Y_G + 255 * Y_R * R / G;
    } else if (G >= B && B >= R) {
        Y_full_C = 255 * Y_G + 255 * Y_B * B / G;
    } else if (B >= G && G >= R) {
        Y_full_C = 255 * Y_B + 255 * Y_G * G / B;
    } else {
        Y_full_C = 255 * Y_B + 255 * Y_R * R / B;
    }

    // Calculate C_max, C_inv_max, to scale C and X.
    float C_max, C_inv_max;
    if (Y >= Y_full_C) {
        C_max = Y_inv / (255 - Y_full_C);
    } else {
        C_max = Y / Y_full_C;
    }
    if (Y_inv >= Y_full_C) {
        C_inv_max = Y / (255 - Y_full_C);
    } else {
        C_inv_max = Y_inv / Y_full_C;
    }

    // Scale C and X. C and X already lie in R, G, B.
    float C_scale = C_inv_max / C_max;
    float R_ = R * C_scale;
    float G_ = G * C_scale;
    float B_ = B * C_scale;

    // Calculate missing luma (in range 0..255), to get common component m_inv
    float m_inv = Y_inv - (Y_R * R_ + Y_G * G_ + Y_B * B_);

    // Add m_inv to color compontents
    R_ += m_inv;
    G_ += m_inv;
    B_ += m_inv;

    // Return colors rounded
    R = R_ + 0.5;
    G = G_ + 0.5;
    B = B_ + 0.5;
}

void PagePainter::hueShiftPositive(QImage *image)
{
    if (image->format() != QImage::Format_ARGB32_Premultiplied) {
        qCWarning(OkularUiDebug) << "Wrong image format! Converting...";
        *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    Q_ASSERT(image->format() == QImage::Format_ARGB32_Premultiplied);

    QRgb *data = reinterpret_cast<QRgb *>(image->bits());
    int pixels = image->width() * image->height();
    for (int i = 0; i < pixels; ++i) {
        uchar R = qRed(data[i]);
        uchar G = qGreen(data[i]);
        uchar B = qBlue(data[i]);

        // Save new color
        const unsigned A = qAlpha(data[i]);
        data[i] = qRgba(B, R, G, A);
    }
}

void PagePainter::hueShiftNegative(QImage *image)
{
    if (image->format() != QImage::Format_ARGB32_Premultiplied) {
        qCWarning(OkularUiDebug) << "Wrong image format! Converting...";
        *image = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    Q_ASSERT(image->format() == QImage::Format_ARGB32_Premultiplied);

    QRgb *data = reinterpret_cast<QRgb *>(image->bits());
    int pixels = image->width() * image->height();
    for (int i = 0; i < pixels; ++i) {
        uchar R = qRed(data[i]);
        uchar G = qGreen(data[i]);
        uchar B = qBlue(data[i]);

        // Save new color
        const unsigned A = qAlpha(data[i]);
        data[i] = qRgba(G, B, R, A);
    }
}

void PagePainter::drawShapeOnImage(QImage &image, const NormalizedPath &normPath, bool closeShape, const QPen &pen, const QBrush &brush, double penWidthMultiplier, RasterOperation op
                                   // float antiAliasRadius
)
{
    // safety checks
    int pointsNumber = normPath.size();
    if (pointsNumber < 2) {
        return;
    }

    const double dpr = image.devicePixelRatio();
    const double fImageWidth = image.width() / dpr;
    const double fImageHeight = image.height() / dpr;

    // stroke outline
    double penWidth = (double)pen.width() * penWidthMultiplier;
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen2 = pen;
    pen2.setWidthF(penWidth);
    painter.setPen(pen2);
    painter.setBrush(brush);

    if (op == Multiply) {
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    }

    if (brush.style() == Qt::NoBrush) {
        // create a polygon
        QPolygonF poly(closeShape ? pointsNumber + 1 : pointsNumber);
        for (int i = 0; i < pointsNumber; ++i) {
            poly[i] = QPointF(normPath[i].x * fImageWidth, normPath[i].y * fImageHeight);
        }
        if (closeShape) {
            poly[pointsNumber] = poly[0];
        }

        painter.drawPolyline(poly);
    } else {
        // create a 'path'
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);

        path.moveTo(normPath[0].x * fImageWidth, normPath[0].y * fImageHeight);
        for (int i = 1; i < pointsNumber; i++) {
            path.lineTo(normPath[i].x * fImageWidth, normPath[i].y * fImageHeight);
        }
        if (closeShape) {
            path.closeSubpath();
        }

        painter.drawPath(path);
    }
}

void PagePainter::drawEllipseOnImage(QImage &image, const NormalizedPath &rect, const QPen &pen, const QBrush &brush, double penWidthMultiplier, RasterOperation op)
{
    const double dpr = image.devicePixelRatio();
    const double fImageWidth = image.width() / dpr;
    const double fImageHeight = image.height() / dpr;

    // stroke outline
    const double penWidth = (double)pen.width() * penWidthMultiplier;
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen2 = pen;
    pen2.setWidthF(penWidth);
    painter.setPen(pen2);
    painter.setBrush(brush);

    if (op == Multiply) {
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    }

    const QPointF &topLeft {rect[0].x * fImageWidth, rect[0].y * fImageHeight};
    const QSizeF &size {(rect[1].x - rect[0].x) * fImageWidth, (rect[1].y - rect[0].y) * fImageHeight};
    const QRectF imgRect {topLeft, size};
    if (brush.style() == Qt::NoBrush) {
        painter.drawArc(imgRect, 0, 16 * 360);
    } else {
        painter.drawEllipse(imgRect);
    }
}

LineAnnotPainter::LineAnnotPainter(const Okular::LineAnnotation *a, QSizeF pageSize, double pageScale, const QTransform &toNormalizedImage)
    : la {a}
    , pageSize {pageSize}
    , pageScale {pageScale}
    , toNormalizedImage {toNormalizedImage}
    , aspectRatio {pageSize.height() / pageSize.width()}
    , linePen {buildPen(a, a->style().width(), a->style().color())}
{
    if ((la->lineClosed() || la->transformedLinePoints().count() == 2) && la->lineInnerColor().isValid()) {
        fillBrush = QBrush(la->lineInnerColor());
    }
}

void LineAnnotPainter::draw(QImage &image) const
{
    const QList<Okular::NormalizedPoint> transformedLinePoints = la->transformedLinePoints();
    if (transformedLinePoints.count() == 2) {
        const Okular::NormalizedPoint delta {transformedLinePoints.last().x - transformedLinePoints.first().x, transformedLinePoints.first().y - transformedLinePoints.last().y};
        const double angle {atan2(delta.y * aspectRatio, delta.x)};
        const double cosA {cos(-angle)};
        const double sinA {sin(-angle)};
        const QTransform tmpMatrix = QTransform {cosA, sinA / aspectRatio, -sinA, cosA / aspectRatio, transformedLinePoints.first().x, transformedLinePoints.first().y};
        const double deaspectedY {delta.y * aspectRatio};
        const double mainSegmentLength {sqrt(delta.x * delta.x + deaspectedY * deaspectedY)};
        const double lineendSize {std::min(6. * la->style().width() / pageSize.width(), mainSegmentLength / 2.)};

        drawShortenedLine(mainSegmentLength, lineendSize, image, tmpMatrix);
        drawLineEnds(mainSegmentLength, lineendSize, image, tmpMatrix);
        drawLeaderLine(0., image, tmpMatrix);
        drawLeaderLine(mainSegmentLength, image, tmpMatrix);
    } else if (transformedLinePoints.count() > 2) {
        drawMainLine(image);
    }
}

void LineAnnotPainter::drawMainLine(QImage &image) const
{
    // draw the line as normalized path into image
    PagePainter::drawShapeOnImage(image, transformPath(la->transformedLinePoints(), toNormalizedImage), la->lineClosed(), linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawShortenedLine(double mainSegmentLength, double size, QImage &image, const QTransform &toNormalizedPage) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const QList<Okular::NormalizedPoint> path {{shortenForArrow(size, la->lineStartStyle()), 0}, {mainSegmentLength - shortenForArrow(size, la->lineEndStyle()), 0}};
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), la->lineClosed(), linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEnds(double mainSegmentLength, double size, QImage &image, const QTransform &transform) const
{
    switch (la->lineStartStyle()) {
    case Okular::LineAnnotation::Square:
        drawLineEndSquare(0, -size, transform, image);
        break;
    case Okular::LineAnnotation::Circle:
        drawLineEndCircle(0, -size, transform, image);
        break;
    case Okular::LineAnnotation::Diamond:
        drawLineEndDiamond(0, -size, transform, image);
        break;
    case Okular::LineAnnotation::OpenArrow:
        drawLineEndArrow(0, -size, 1., false, transform, image);
        break;
    case Okular::LineAnnotation::ClosedArrow:
        drawLineEndArrow(0, -size, 1., true, transform, image);
        break;
    case Okular::LineAnnotation::None:
        break;
    case Okular::LineAnnotation::Butt:
        drawLineEndButt(0, size, transform, image);
        break;
    case Okular::LineAnnotation::ROpenArrow:
        drawLineEndArrow(0, size, 1., false, transform, image);
        break;
    case Okular::LineAnnotation::RClosedArrow:
        drawLineEndArrow(0, size, 1., true, transform, image);
        break;
    case Okular::LineAnnotation::Slash:
        drawLineEndSlash(0, -size, transform, image);
        break;
    }
    switch (la->lineEndStyle()) {
    case Okular::LineAnnotation::Square:
        drawLineEndSquare(mainSegmentLength, size, transform, image);
        break;
    case Okular::LineAnnotation::Circle:
        drawLineEndCircle(mainSegmentLength, size, transform, image);
        break;
    case Okular::LineAnnotation::Diamond:
        drawLineEndDiamond(mainSegmentLength, size, transform, image);
        break;
    case Okular::LineAnnotation::OpenArrow:
        drawLineEndArrow(mainSegmentLength, size, 1., false, transform, image);
        break;
    case Okular::LineAnnotation::ClosedArrow:
        drawLineEndArrow(mainSegmentLength, size, 1., true, transform, image);
        break;
    case Okular::LineAnnotation::None:
        break;
    case Okular::LineAnnotation::Butt:
        drawLineEndButt(mainSegmentLength, size, transform, image);
        break;
    case Okular::LineAnnotation::ROpenArrow:
        drawLineEndArrow(mainSegmentLength, size, -1., false, transform, image);
        break;
    case Okular::LineAnnotation::RClosedArrow:
        drawLineEndArrow(mainSegmentLength, size, -1., true, transform, image);
        break;
    case Okular::LineAnnotation::Slash:
        drawLineEndSlash(mainSegmentLength, size, transform, image);
        break;
    }
}

void LineAnnotPainter::drawLineEndArrow(double xEndPos, double size, double flipX, bool close, const QTransform &toNormalizedPage, QImage &image) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const QList<Okular::NormalizedPoint> path {
        {xEndPos - size * flipX, size / 2.},
        {xEndPos, 0},
        {xEndPos - size * flipX, -size / 2.},
    };
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), close, linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEndButt(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const double halfSize {size / 2.};
    const QList<Okular::NormalizedPoint> path {
        {xEndPos, halfSize},
        {xEndPos, -halfSize},
    };
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), true, linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEndCircle(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const
{
    /* transform the circle midpoint to intermediate normalized coordinates
     * where it's easy to construct the bounding rect of the circle */
    Okular::NormalizedPoint center;
    toNormalizedPage.map(xEndPos - size / 2., 0, &center.x, &center.y);
    const double halfSize {size / 2.};
    const QList<Okular::NormalizedPoint> path {
        {center.x - halfSize, center.y - halfSize / aspectRatio},
        {center.x + halfSize, center.y + halfSize / aspectRatio},
    };

    /* then transform bounding rect with toNormalizedImage */
    PagePainter::drawEllipseOnImage(image, transformPath(path, toNormalizedImage), linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEndSquare(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const QList<Okular::NormalizedPoint> path {{xEndPos, size / 2.}, {xEndPos - size, size / 2.}, {xEndPos - size, -size / 2.}, {xEndPos, -size / 2.}};
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), true, linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEndDiamond(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const QList<Okular::NormalizedPoint> path {{xEndPos, 0}, {xEndPos - size / 2., size / 2.}, {xEndPos - size, 0}, {xEndPos - size / 2., -size / 2.}};
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), true, linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLineEndSlash(double xEndPos, double size, const QTransform &toNormalizedPage, QImage &image) const
{
    const QTransform combinedTransform {toNormalizedPage * toNormalizedImage};
    const double halfSize {size / 2.};
    const double xOffset {cos(M_PI / 3.) * halfSize};
    const QList<Okular::NormalizedPoint> path {
        {xEndPos - xOffset, halfSize},
        {xEndPos + xOffset, -halfSize},
    };
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), true, linePen, fillBrush, pageScale);
}

void LineAnnotPainter::drawLeaderLine(double xEndPos, QImage &image, const QTransform &toNormalizedPage) const
{
    const QTransform combinedTransform = toNormalizedPage * toNormalizedImage;
    const double ll = aspectRatio * la->lineLeadingForwardPoint() / pageSize.height();
    const double lle = aspectRatio * la->lineLeadingBackwardPoint() / pageSize.height();
    const int sign {ll > 0 ? -1 : 1};
    QList<Okular::NormalizedPoint> path;

    if (fabs(ll) > 0) {
        path.append({xEndPos, ll});
        // do we have the extension on the "back"?
        if (fabs(lle) > 0) {
            path.append({xEndPos, sign * lle});
        } else {
            path.append({xEndPos, 0});
        }
    }
    PagePainter::drawShapeOnImage(image, transformPath(path, combinedTransform), false, linePen, fillBrush, pageScale);
}

double LineAnnotPainter::shortenForArrow(double size, Okular::LineAnnotation::TermStyle endStyle)
{
    double shortenBy {0};

    if (endStyle == Okular::LineAnnotation::Square || endStyle == Okular::LineAnnotation::Circle || endStyle == Okular::LineAnnotation::Diamond || endStyle == Okular::LineAnnotation::ClosedArrow) {
        shortenBy = size;
    }

    return shortenBy;
}

/* kate: replace-tabs on; indent-width 4; */
