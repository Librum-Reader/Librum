/*
    SPDX-FileCopyrightText: 2006-2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_GUIUTILS_H
#define OKULAR_GUIUTILS_H

#include <QColor>
#include <QIcon>
#include <QString>

class QImage;
class QPixmap;
class QSize;
class QWidget;
class QFile;

namespace Okular
{
class Annotation;
class EmbeddedFile;
class Movie;
class ScreenAnnotation;
}  // namespace Okular

namespace GuiUtils
{
/**
 * Returns the translated string with the type of the given @p annotation.
 */
QString captionForAnnotation(const Okular::Annotation* annotation);
QString authorForAnnotation(const Okular::Annotation* annotation);

QString contentsHtml(const Okular::Annotation* annotation);

QString prettyToolTip(const Okular::Annotation* annotation);

void saveEmbeddedFile(Okular::EmbeddedFile* ef, QWidget* parent);
void writeEmbeddedFile(Okular::EmbeddedFile* ef, QWidget* parent,
                       QFile& targetFile);

/**
 * Returns the movie object that is referenced by a rendition action of the
 * passed screen @p annotation or @c 0 if the screen annotation has no rendition
 * action set or the rendition action does not contain a media rendition.
 */
Okular::Movie* renditionMovieFromScreenAnnotation(
    const Okular::ScreenAnnotation* annotation);

// colorize a gray image to the given color
void colorizeImage(QImage& image, const QColor& color,
                   unsigned int alpha = 255);

enum ColorIconFlags
{
    NoFlags = 0x0,
    VisualizeTransparent = 0x1  ///< Visualizes Qt::transparent with a cross.
};

/**
 * Paints color rectangles on the lower 25% of an icon.
 *
 * If no icon is given, the whole icon square is filled, and a 1px border is
 * added.
 *
 * Examples:
 *  * Different icons for different kinds of color selection, like fill and
 * outline.
 *  * Selection of a color scheme preset, where each scheme has a different icon
 * and ~1..3 specific colors.
 *
 * @param colors Which color rectangles to paint, from left to right (even on
 * RTL). Colors may be transparent. Invalid colors are skipped.
 * @param background Which icon to use as background.
 * @param flags Special wishes.
 *
 * @returns A newly created QIcon.
 */
QIcon createColorIcon(const QList<QColor>& colors,
                      const QIcon& background = QIcon(),
                      ColorIconFlags flags = NoFlags);

/**
 * Creates an opacity icon, using QPalette foreground color
 * painted on top of a checkerboard pattern using @p opacity.
 *
 * @param opacity 0 = invisible, 1 = opaque.
 */
QIcon createOpacityIcon(qreal opacity);

}  // namespace GuiUtils

#endif
