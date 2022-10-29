/*
    SPDX-FileCopyrightText: 2006-2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "guiutils.h"

// qt/kde includes
#include <KLocalizedString>
#include <KMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QPainter>
#include <QStandardPaths>
#include <QTextDocument>

// local includes
#include <memory>
#include "core/action.h"
#include "core/annotations.h"
#include "core/document.h"

namespace GuiUtils
{
QString captionForAnnotation(const Okular::Annotation* ann)
{
    Q_ASSERT(ann);

    const bool hasComment = !ann->contents().isEmpty();

    QString ret;
    switch(ann->subType())
    {
    case Okular::Annotation::AText:
        if(((Okular::TextAnnotation*)ann)->textType() ==
           Okular::TextAnnotation::Linked)
        {
            ret = i18n("Pop-up Note");
        }
        else
        {
            if(((Okular::TextAnnotation*)ann)->inplaceIntent() ==
               Okular::TextAnnotation::TypeWriter)
            {
                ret = i18n("Typewriter");
            }
            else
            {
                ret = i18n("Inline Note");
            }
        }
        break;
    case Okular::Annotation::ALine:
        if(((Okular::LineAnnotation*)ann)->linePoints().count() == 2)
        {
            ret = hasComment ? i18n("Straight Line with Comment")
                             : i18n("Straight Line");
        }
        else
        {
            ret = hasComment ? i18n("Polygon with Comment") : i18n("Polygon");
        }
        break;
    case Okular::Annotation::AGeom:
        ret = hasComment ? i18n("Geometry with Comment") : i18n("Geometry");
        break;
    case Okular::Annotation::AHighlight:
        switch(((Okular::HighlightAnnotation*)ann)->highlightType())
        {
        case Okular::HighlightAnnotation::Highlight:
            ret =
                hasComment ? i18n("Highlight with Comment") : i18n("Highlight");
            break;
        case Okular::HighlightAnnotation::Squiggly:
            ret = hasComment ? i18n("Squiggle with Comment") : i18n("Squiggle");
            break;
        case Okular::HighlightAnnotation::Underline:
            ret =
                hasComment ? i18n("Underline with Comment") : i18n("Underline");
            break;
        case Okular::HighlightAnnotation::StrikeOut:
            ret = hasComment ? i18n("Strike Out with Comment")
                             : i18n("Strike Out");
            break;
        }
        break;
    case Okular::Annotation::AStamp:
        ret = hasComment ? i18n("Stamp with Comment") : i18n("Stamp");
        break;
    case Okular::Annotation::AInk:
        ret = hasComment ? i18n("Freehand Line with Comment")
                         : i18n("Freehand Line");
        break;
    case Okular::Annotation::ACaret:
        ret = i18n("Caret");
        break;
    case Okular::Annotation::AFileAttachment:
        ret = i18n("File Attachment");
        break;
    case Okular::Annotation::ASound:
        ret = i18n("Sound");
        break;
    case Okular::Annotation::AMovie:
        ret = i18n("Movie");
        break;
    case Okular::Annotation::AScreen:
        ret = i18nc("Caption for a screen annotation", "Screen");
        break;
    case Okular::Annotation::AWidget:
        ret = i18nc("Caption for a widget annotation", "Widget");
        break;
    case Okular::Annotation::ARichMedia:
        ret = i18nc("Caption for a rich media annotation", "Rich Media");
        break;
    case Okular::Annotation::A_BASE:
        break;
    }
    return ret;
}

QString authorForAnnotation(const Okular::Annotation* ann)
{
    Q_ASSERT(ann);

    return !ann->author().isEmpty() ? ann->author()
                                    : i18nc("Unknown author", "Unknown");
}

QString contentsHtml(const Okular::Annotation* ann)
{
    QString text = ann->contents().toHtmlEscaped();
    text.replace(QLatin1Char('\n'), QLatin1String("<br>"));
    return text;
}

QString prettyToolTip(const Okular::Annotation* ann)
{
    Q_ASSERT(ann);

    QString author = authorForAnnotation(ann);
    QString contents = contentsHtml(ann);

    QString tooltip = QStringLiteral("<qt><b>") + i18n("Author: %1", author) +
                      QStringLiteral("</b>");
    if(!contents.isEmpty())
    {
        tooltip +=
            QStringLiteral("<div style=\"font-size: 4px;\"><hr /></div>") +
            contents;
    }

    tooltip += QLatin1String("</qt>");

    return tooltip;
}

void saveEmbeddedFile(Okular::EmbeddedFile* ef, QWidget* parent)
{
    const QString caption = i18n("Where do you want to save %1?", ef->name());
    const QString path =
        QFileDialog::getSaveFileName(parent, caption, ef->name());
    if(path.isEmpty())
    {
        return;
    }
    QFile targetFile(path);
    writeEmbeddedFile(ef, parent, targetFile);
}

void writeEmbeddedFile(Okular::EmbeddedFile* ef, QWidget* parent, QFile& target)
{
    if(!target.open(QIODevice::WriteOnly))
    {
        KMessageBox::error(
            parent,
            i18n("Could not open \"%1\" for writing. File was not saved.",
                 target.fileName()));
        return;
    }
    target.write(ef->data());
    target.close();
}

Okular::Movie* renditionMovieFromScreenAnnotation(
    const Okular::ScreenAnnotation* annotation)
{
    if(!annotation)
    {
        return nullptr;
    }

    if(annotation->action() &&
       annotation->action()->actionType() == Okular::Action::Rendition)
    {
        Okular::RenditionAction* renditionAction =
            static_cast<Okular::RenditionAction*>(annotation->action());
        return renditionAction->movie();
    }

    return nullptr;
}

// from Arthur - qt4
static inline int qt_div_255(int x)
{
    return (x + (x >> 8) + 0x80) >> 8;
}

void colorizeImage(QImage& grayImage, const QColor& color,
                   unsigned int destAlpha)
{
    // Make sure that the image is Format_ARGB32_Premultiplied
    if(grayImage.format() != QImage::Format_ARGB32_Premultiplied)
    {
        grayImage =
            grayImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    // iterate over all pixels changing the alpha component value
    unsigned int* data = reinterpret_cast<unsigned int*>(grayImage.bits());
    unsigned int pixels = grayImage.width() * grayImage.height();
    int red = color.red(), green = color.green(), blue = color.blue();

    int source, sourceSat, sourceAlpha;
    for(unsigned int i = 0; i < pixels; ++i)
    {  // optimize this loop keeping byte order into account
        source = data[i];
        sourceSat = qRed(source);
        int newR = qt_div_255(sourceSat * red),
            newG = qt_div_255(sourceSat * green),
            newB = qt_div_255(sourceSat * blue);
        if((sourceAlpha = qAlpha(source)) == 255)
        {
            // use destAlpha
            data[i] = qRgba(newR, newG, newB, destAlpha);
        }
        else
        {
            // use destAlpha * sourceAlpha product
            if(destAlpha < 255)
            {
                sourceAlpha = qt_div_255(destAlpha * sourceAlpha);
            }
            data[i] = qRgba(newR, newG, newB, sourceAlpha);
        }
    }
}

QIcon createColorIcon(const QList<QColor>& colors, const QIcon& background,
                      ColorIconFlags flags)
{
    QIcon colorIcon;

    // Create a pixmap for each common icon size.
    for(int size : { 16, 22, 24, 32, 48 })
    {
        QPixmap pixmap(QSize(size, size) * qApp->devicePixelRatio());
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        // Configure hairlines for visualization of outline or transparency
        // (visualizeTransparent):
        painter.setPen(QPen(
            qApp->palette().color(QPalette::Active, QPalette::WindowText), 0));
        painter.setBrush(Qt::NoBrush);

        if(background.isNull())
        {
            // Full-size color rectangles.
            // Draw rectangles left to right:
            for(int i = 0; i < colors.count(); ++i)
            {
                if(!colors.at(i).isValid())
                {
                    continue;
                }
                QRect rect(QPoint(size * i / colors.count(), 0),
                           QPoint(size * (i + 1) / colors.count(), size));
                if((flags & VisualizeTransparent) &&
                   (colors.at(i) == Qt::transparent))
                {
                    painter.drawLine(rect.topLeft(), rect.bottomRight());
                    painter.drawLine(rect.bottomLeft(), rect.topRight());
                }
                else
                {
                    painter.fillRect(rect, colors.at(i));
                }
            }

            // Draw hairline outline:
            // To get the hairline on the outermost pixels, we shrink the
            // rectangle by a half pixel on each edge.
            const qreal halfPixelWidth = 0.5 / pixmap.devicePixelRatio();
            painter.drawRect(QRectF(QPointF(halfPixelWidth, halfPixelWidth),
                                    QPointF(qreal(size) - halfPixelWidth,
                                            qreal(size) - halfPixelWidth)));
        }
        else
        {
            // Lower 25% color rectangles.
            // Draw background icon:
            background.paint(&painter, QRect(QPoint(0, 0), QSize(size, size)));

            // Draw rectangles left to right:
            for(int i = 0; i < colors.count(); ++i)
            {
                if(!colors.at(i).isValid())
                {
                    continue;
                }
                QRect rect(QPoint(size * i / colors.count(), size * 3 / 4),
                           QPoint(size * (i + 1) / colors.count(), size));
                if((flags & VisualizeTransparent) &&
                   (colors.at(i) == Qt::transparent))
                {
                    painter.drawLine(rect.topLeft(), rect.bottomRight());
                    painter.drawLine(rect.bottomLeft(), rect.topRight());
                }
                else
                {
                    painter.fillRect(rect, colors.at(i));
                }
            }
        }

        painter.end();
        colorIcon.addPixmap(pixmap);
    }

    return colorIcon;
}

QIcon createOpacityIcon(qreal opacity)
{
    QIcon opacityIcon;

    // Create a pixmap for each common icon size.
    for(int size : { 16, 22, 24, 32, 48 })
    {
        QPixmap pixmap(QSize(size, size) * qApp->devicePixelRatio());
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(Qt::NoPen);
        painter.setBrush(
            qApp->palette().color(QPalette::Active, QPalette::WindowText));

        // Checkerboard pattern
        painter.drawRect(QRectF(QPoint(0, 0), QPoint(size, size) / 2));
        painter.drawRect(QRectF(QPoint(size, size) / 2, QPoint(size, size)));

        // Opacity
        painter.setOpacity(opacity);
        painter.drawRect(QRect(QPoint(0, 0), QPoint(size, size)));

        painter.end();
        opacityIcon.addPixmap(pixmap);
    }

    return opacityIcon;
}

}  // namespace GuiUtils
