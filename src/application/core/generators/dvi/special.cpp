// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-

// special.cpp

// Methods for dviRenderer which deal with "\special" commands found in the
// DVI file

// Copyright 2000--2004, Stefan Kebekus (kebekus@kde.org).

#include <config.h>
#include "debug_dvi.h"
#include "dviFile.h"
#include "dviRenderer.h"
#include "hyperlink.h"
#include "psgs.h"
//#include "renderedDocumentPage.h"

#include <KLocalizedString>
#include <QByteArray>
#include <QFile>
#include <QFontDatabase>
#include <QImage>
#include <QMimeDatabase>
#include <QMimeType>
#include <QPainter>
#include "debug_dvi.h"

void dviRenderer::printErrorMsgForSpecials(const QString& msg)
{
    if(dviFile->errorCounter < 25)
    {
        qCCritical(OkularDviDebug) << msg;
        dviFile->errorCounter++;
        if(dviFile->errorCounter == 25)
        {
            qCCritical(OkularDviDebug)
                << i18n("That makes 25 errors. Further error messages will not "
                        "be printed.");
        }
    }
}

// Parses a color specification, as explained in the manual to
// dvips. If the spec could not be parsed, an invalid color will be
// returned.

QColor dviRenderer::parseColorSpecification(const QString& colorSpec)
{
    // Initialize the map of known colors, if that is not done yet.
    if(namedColors.isEmpty())
    {
        namedColors[QStringLiteral("Red")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Tan")] = QColor(
            (int)(255.0 * 0.86), (int)(255.0 * 0.58), (int)(255.0 * 0.44));
        namedColors[QStringLiteral("Blue")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0), (int)(255.0 * 1));
        namedColors[QStringLiteral("Cyan")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 1), (int)(255.0 * 1));
        namedColors[QStringLiteral("Gray")] =
            QColor((int)(255.0 * 0.5), (int)(255.0 * 0.5), (int)(255.0 * 0.5));
        namedColors[QStringLiteral("Plum")] =
            QColor((int)(255.0 * 0.5), (int)(255.0 * 0), (int)(255.0 * 1));
        namedColors[QStringLiteral("Black")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Brown")] =
            QColor((int)(255.0 * 0.4), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Green")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 1), (int)(255.0 * 0));
        namedColors[QStringLiteral("Melon")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.54), (int)(255.0 * 0.5));
        namedColors[QStringLiteral("Peach")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.5), (int)(255.0 * 0.3));
        namedColors[QStringLiteral("Sepia")] =
            QColor((int)(255.0 * 0.3), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("White")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 1), (int)(255.0 * 1));
        namedColors[QStringLiteral("Maroon")] =
            QColor((int)(255.0 * 0.68), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Orange")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.39), (int)(255.0 * 0.13));
        namedColors[QStringLiteral("Orchid")] =
            QColor((int)(255.0 * 0.68), (int)(255.0 * 0.36), (int)(255.0 * 1));
        namedColors[QStringLiteral("Purple")] =
            QColor((int)(255.0 * 0.55), (int)(255.0 * 0.14), (int)(255.0 * 1));
        namedColors[QStringLiteral("Salmon")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.47), (int)(255.0 * 0.62));
        namedColors[QStringLiteral("Violet")] =
            QColor((int)(255.0 * 0.21), (int)(255.0 * 0.12), (int)(255.0 * 1));
        namedColors[QStringLiteral("Yellow")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 1), (int)(255.0 * 0));
        namedColors[QStringLiteral("Apricot")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.68), (int)(255.0 * 0.48));
        namedColors[QStringLiteral("Emerald")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 1), (int)(255.0 * 0.5));
        namedColors[QStringLiteral("Fuchsia")] = QColor(
            (int)(255.0 * 0.45), (int)(255.0 * 0.01), (int)(255.0 * 0.92));
        namedColors[QStringLiteral("Magenta")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0), (int)(255.0 * 1));
        namedColors[QStringLiteral("SkyBlue")] =
            QColor((int)(255.0 * 0.38), (int)(255.0 * 1), (int)(255.0 * 0.88));
        namedColors[QStringLiteral("Thistle")] =
            QColor((int)(255.0 * 0.88), (int)(255.0 * 0.41), (int)(255.0 * 1));
        namedColors[QStringLiteral("BrickRed")] =
            QColor((int)(255.0 * 0.72), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Cerulean")] =
            QColor((int)(255.0 * 0.06), (int)(255.0 * 0.89), (int)(255.0 * 1));
        namedColors[QStringLiteral("Lavender")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.52), (int)(255.0 * 1));
        namedColors[QStringLiteral("Mahogany")] =
            QColor((int)(255.0 * 0.65), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("Mulberry")] = QColor(
            (int)(255.0 * 0.64), (int)(255.0 * 0.08), (int)(255.0 * 0.98));
        namedColors[QStringLiteral("NavyBlue")] =
            QColor((int)(255.0 * 0.06), (int)(255.0 * 0.46), (int)(255.0 * 1));
        namedColors[QStringLiteral("SeaGreen")] =
            QColor((int)(255.0 * 0.31), (int)(255.0 * 1), (int)(255.0 * 0.5));
        namedColors[QStringLiteral("TealBlue")] = QColor(
            (int)(255.0 * 0.12), (int)(255.0 * 0.98), (int)(255.0 * 0.64));
        namedColors[QStringLiteral("BlueGreen")] =
            QColor((int)(255.0 * 0.15), (int)(255.0 * 1), (int)(255.0 * 0.67));
        namedColors[QStringLiteral("CadetBlue")] = QColor(
            (int)(255.0 * 0.38), (int)(255.0 * 0.43), (int)(255.0 * 0.77));
        namedColors[QStringLiteral("Dandelion")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.71), (int)(255.0 * 0.16));
        namedColors[QStringLiteral("Goldenrod")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.9), (int)(255.0 * 0.16));
        namedColors[QStringLiteral("LimeGreen")] =
            QColor((int)(255.0 * 0.5), (int)(255.0 * 1), (int)(255.0 * 0));
        namedColors[QStringLiteral("OrangeRed")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0), (int)(255.0 * 0.5));
        namedColors[QStringLiteral("PineGreen")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0.75), (int)(255.0 * 0.16));
        namedColors[QStringLiteral("RawSienna")] =
            QColor((int)(255.0 * 0.55), (int)(255.0 * 0), (int)(255.0 * 0));
        namedColors[QStringLiteral("RedOrange")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.23), (int)(255.0 * 0.13));
        namedColors[QStringLiteral("RedViolet")] =
            QColor((int)(255.0 * 0.59), (int)(255.0 * 0), (int)(255.0 * 0.66));
        namedColors[QStringLiteral("Rhodamine")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.18), (int)(255.0 * 1));
        namedColors[QStringLiteral("RoyalBlue")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0.5), (int)(255.0 * 1));
        namedColors[QStringLiteral("RubineRed")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0), (int)(255.0 * 0.87));
        namedColors[QStringLiteral("Turquoise")] =
            QColor((int)(255.0 * 0.15), (int)(255.0 * 1), (int)(255.0 * 0.8));
        namedColors[QStringLiteral("VioletRed")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.19), (int)(255.0 * 1));
        namedColors[QStringLiteral("Aquamarine")] =
            QColor((int)(255.0 * 0.18), (int)(255.0 * 1), (int)(255.0 * 0.7));
        namedColors[QStringLiteral("BlueViolet")] = QColor(
            (int)(255.0 * 0.1), (int)(255.0 * 0.05), (int)(255.0 * 0.96));
        namedColors[QStringLiteral("DarkOrchid")] =
            QColor((int)(255.0 * 0.6), (int)(255.0 * 0.2), (int)(255.0 * 0.8));
        namedColors[QStringLiteral("OliveGreen")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0.6), (int)(255.0 * 0));
        namedColors[QStringLiteral("Periwinkle")] =
            QColor((int)(255.0 * 0.43), (int)(255.0 * 0.45), (int)(255.0 * 1));
        namedColors[QStringLiteral("Bittersweet")] =
            QColor((int)(255.0 * 0.76), (int)(255.0 * 0.01), (int)(255.0 * 0));
        namedColors[QStringLiteral("BurntOrange")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.49), (int)(255.0 * 0));
        namedColors[QStringLiteral("ForestGreen")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0.88), (int)(255.0 * 0));
        namedColors[QStringLiteral("GreenYellow")] =
            QColor((int)(255.0 * 0.85), (int)(255.0 * 1), (int)(255.0 * 0.31));
        namedColors[QStringLiteral("JungleGreen")] =
            QColor((int)(255.0 * 0.01), (int)(255.0 * 1), (int)(255.0 * 0.48));
        namedColors[QStringLiteral("ProcessBlue")] =
            QColor((int)(255.0 * 0.04), (int)(255.0 * 1), (int)(255.0 * 1));
        namedColors[QStringLiteral("RoyalPurple")] =
            QColor((int)(255.0 * 0.25), (int)(255.0 * 0.1), (int)(255.0 * 1));
        namedColors[QStringLiteral("SpringGreen")] =
            QColor((int)(255.0 * 0.74), (int)(255.0 * 1), (int)(255.0 * 0.24));
        namedColors[QStringLiteral("YellowGreen")] =
            QColor((int)(255.0 * 0.56), (int)(255.0 * 1), (int)(255.0 * 0.26));
        namedColors[QStringLiteral("MidnightBlue")] =
            QColor((int)(255.0 * 0), (int)(255.0 * 0.44), (int)(255.0 * 0.57));
        namedColors[QStringLiteral("YellowOrange")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.58), (int)(255.0 * 0));
        namedColors[QStringLiteral("CarnationPink")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.37), (int)(255.0 * 1));
        namedColors[QStringLiteral("CornflowerBlue")] =
            QColor((int)(255.0 * 0.35), (int)(255.0 * 0.87), (int)(255.0 * 1));
        namedColors[QStringLiteral("WildStrawberry")] =
            QColor((int)(255.0 * 1), (int)(255.0 * 0.04), (int)(255.0 * 0.61));
    }

    QString specType = colorSpec.section(QLatin1Char(' '), 0, 0);

    if(specType.indexOf(QStringLiteral("rgb"), 0, Qt::CaseInsensitive) == 0)
    {
        bool ok;

        double r = colorSpec.section(QLatin1Char(' '), 1, 1).toDouble(&ok);
        if((ok == false) || (r < 0.0) || (r > 1.0))
        {
            return QColor();
        }

        double g = colorSpec.section(QLatin1Char(' '), 2, 2).toDouble(&ok);
        if((ok == false) || (g < 0.0) || (g > 1.0))
        {
            return QColor();
        }

        double b = colorSpec.section(QLatin1Char(' '), 3, 3).toDouble(&ok);
        if((ok == false) || (b < 0.0) || (b > 1.0))
        {
            return QColor();
        }

        return QColor((int)(r * 255.0 + 0.5), (int)(g * 255.0 + 0.5),
                      (int)(b * 255.0 + 0.5));
    }

    if(specType.indexOf(QStringLiteral("hsb"), 0, Qt::CaseInsensitive) == 0)
    {
        bool ok;

        double h = colorSpec.section(QLatin1Char(' '), 1, 1).toDouble(&ok);
        if((ok == false) || (h < 0.0) || (h > 1.0))
        {
            return QColor();
        }

        double s = colorSpec.section(QLatin1Char(' '), 2, 2).toDouble(&ok);
        if((ok == false) || (s < 0.0) || (s > 1.0))
        {
            return QColor();
        }

        double b = colorSpec.section(QLatin1Char(' '), 3, 3).toDouble(&ok);
        if((ok == false) || (b < 0.0) || (b > 1.0))
        {
            return QColor();
        }

        return QColor::fromHsv((int)(h * 359.0 + 0.5), (int)(s * 255.0 + 0.5),
                               (int)(b * 255.0 + 0.5));
    }

    if(specType.indexOf(QStringLiteral("cmyk"), 0, Qt::CaseInsensitive) == 0)
    {
        bool ok;

        double c = colorSpec.section(QLatin1Char(' '), 1, 1).toDouble(&ok);
        if((ok == false) || (c < 0.0) || (c > 1.0))
        {
            return QColor();
        }

        double m = colorSpec.section(QLatin1Char(' '), 2, 2).toDouble(&ok);
        if((ok == false) || (m < 0.0) || (m > 1.0))
        {
            return QColor();
        }

        double y = colorSpec.section(QLatin1Char(' '), 3, 3).toDouble(&ok);
        if((ok == false) || (y < 0.0) || (y > 1.0))
        {
            return QColor();
        }

        double k = colorSpec.section(QLatin1Char(' '), 3, 3).toDouble(&ok);
        if((ok == false) || (k < 0.0) || (k > 1.0))
        {
            return QColor();
        }

        // Convert cmyk coordinates to rgb.
        double r = 1.0 - c - k;
        if(r < 0.0)
        {
            r = 0.0;
        }
        double g = 1.0 - m - k;
        if(g < 0.0)
        {
            g = 0.0;
        }
        double b = 1.0 - y - k;
        if(b < 0.0)
        {
            b = 0.0;
        }

        return QColor((int)(r * 255.0 + 0.5), (int)(g * 255.0 + 0.5),
                      (int)(b * 255.0 + 0.5));
    }

    if(specType.indexOf(QStringLiteral("gray"), 0, Qt::CaseInsensitive) == 0)
    {
        bool ok;

        double g = colorSpec.section(QLatin1Char(' '), 1, 1).toDouble(&ok);
        if((ok == false) || (g < 0.0) || (g > 1.0))
        {
            return QColor();
        }

        return QColor((int)(g * 255.0 + 0.5), (int)(g * 255.0 + 0.5),
                      (int)(g * 255.0 + 0.5));
    }

    // Check if the color is one of the known named colors.
    QMap<QString, QColor>::Iterator f = namedColors.find(specType);
    if(f != namedColors.end())
    {
        return *f;
    }

    return QColor(specType);
}

void dviRenderer::color_special(const QString& msg)
{
    QString const cp = msg.trimmed();

    QString command = cp.section(QLatin1Char(' '), 0, 0);

    if(command == QLatin1String("pop"))
    {
        // Take color off the stack
        if(colorStack.isEmpty())
        {
            printErrorMsgForSpecials(
                i18n("Error in DVIfile '%1', page %2. Color pop command issued "
                     "when the color stack is empty.",
                     dviFile->filename, current_page));
        }
        else
        {
            colorStack.pop();
        }
        return;
    }

    if(command == QLatin1String("push"))
    {
        // Get color specification
        const QColor col =
            parseColorSpecification(cp.section(QLatin1Char(' '), 1));
        // Set color
        if(col.isValid())
        {
            colorStack.push(col);
        }
        else
        {
            colorStack.push(Qt::black);
        }
        return;
    }

    // Get color specification and set the color for the rest of this
    // page
    QColor col = parseColorSpecification(cp);
    // Set color
    if(col.isValid())
    {
        globalColor = col;
    }
    else
    {
        globalColor = Qt::black;
    }
    return;
}

void dviRenderer::html_href_special(const QString& msg)
{
    QString cp = msg;
    cp.truncate(cp.indexOf(QLatin1Char('"')));

#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "HTML-special, href " << cp.toLatin1();
#endif
    HTML_href = new QString(cp);
}

void dviRenderer::html_anchor_end()
{
#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "HTML-special, anchor-end";
#endif

    if(HTML_href != nullptr)
    {
        delete HTML_href;
        HTML_href = nullptr;
    }
}

void dviRenderer::source_special(const QString& cp)
{
    // only when rendering really takes place: set source_href to the
    // current special string. When characters are rendered, the
    // rendering routine will then generate a DVI_HyperLink and add it
    // to the proper list. This DVI_HyperLink is used to match mouse
    // positions with the hyperlinks for inverse search.
    if(source_href)
    {
        *source_href = cp;
    }
    else
    {
        source_href = new QString(cp);
    }
}

void parse_special_argument(const QString& strg, const char* argument_name,
                            int* variable)
{
    int index = strg.indexOf(QString::fromLocal8Bit(argument_name));
    if(index >= 0)
    {
        QString tmp = strg.mid(index + strlen(argument_name));
        index = tmp.indexOf(QLatin1Char(' '));
        if(index >= 0)
        {
            tmp.truncate(index);
        }

        bool OK;
        float const tmp_float = tmp.toFloat(&OK);

        if(OK)
        {
            *variable = int(tmp_float + 0.5);
        }
        else
        {
            // Maybe we should open a dialog here.
            qCCritical(OkularDviDebug)
                << i18n("Malformed parameter in the epsf special command.\n"
                        "Expected a float to follow %1 in %2",
                        QString::fromLocal8Bit(argument_name), strg);
        }
    }
}

void dviRenderer::epsf_special(const QString& cp)
{
#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "epsf-special: psfile=" << cp;
#endif

    QString include_command = cp.simplified();

    // The line is supposed to start with "..ile=", and then comes the
    // filename. Figure out what the filename is and stow it away. Of
    // course, this does not work if the filename contains spaces
    // (already the simplified() above is wrong). If you have
    // files like this, go away.
    QString EPSfilename_orig = include_command;
    EPSfilename_orig.truncate(EPSfilename_orig.indexOf(QLatin1Char(' ')));

    // Strip enclosing quotation marks which are included by some LaTeX
    // macro packages (but not by others). This probably means that
    // graphic files are no longer found if the filename really does
    // contain quotes, but we don't really care that much.
    if((EPSfilename_orig.at(0) == QLatin1Char('\"')) &&
       (EPSfilename_orig.at(EPSfilename_orig.length() - 1) ==
        QLatin1Char('\"')))
    {
        EPSfilename_orig =
            EPSfilename_orig.mid(1, EPSfilename_orig.length() - 2);
    }
    QString EPSfilename =
        ghostscript_interface::locateEPSfile(EPSfilename_orig, baseURL);

    // Now parse the arguments.
    int llx = 0;
    int lly = 0;
    int urx = 0;
    int ury = 0;
    int rwi = 0;
    int rhi = 0;
    int angle = 0;

    // just to avoid ambiguities; the filename could contain keywords
    include_command =
        include_command.mid(include_command.indexOf(QLatin1Char(' ')));

    parse_special_argument(include_command, "llx=", &llx);
    parse_special_argument(include_command, "lly=", &lly);
    parse_special_argument(include_command, "urx=", &urx);
    parse_special_argument(include_command, "ury=", &ury);
    parse_special_argument(include_command, "rwi=", &rwi);
    parse_special_argument(include_command, "rhi=", &rhi);
    parse_special_argument(include_command, "angle=", &angle);

    // If we have a png, gif, jpeg or mng file, we need to draw it here.
    QMimeDatabase db;
    QMimeType const mime_type =
        db.mimeTypeForFile(EPSfilename, QMimeDatabase::MatchContent);
    QString const& mime_type_name =
        mime_type.isValid() ? mime_type.name() : QString();
    bool const isGFX = (mime_type_name == QLatin1String("image/png") ||
                        mime_type_name == QLatin1String("image/gif") ||
                        mime_type_name == QLatin1String("image/jpeg") ||
                        mime_type_name == QLatin1String("video/x-mng"));

    // So, if we do not have a PostScript file, but a graphics file, and
    // if that file exists, we draw it here.
    if(isGFX && QFile::exists(EPSfilename))
    {
        // Don't show PostScript, just draw the bounding box. For this,
        // calculate the size of the bounding box in Pixels.
        double bbox_width = urx - llx;
        double bbox_height = ury - lly;

        if((rwi != 0) && (bbox_width != 0))
        {
            bbox_height *= rwi / bbox_width;
            bbox_width = rwi;
        }
        if((rhi != 0) && (bbox_height != 0))
        {
            bbox_width *= rhi / bbox_height;
            bbox_height = rhi;
        }

        double fontPixelPerDVIunit = dviFile->getCmPerDVIunit() * 1200.0 / 2.54;

        bbox_width *= 0.1 * 65536.0 * fontPixelPerDVIunit / shrinkfactor;
        bbox_height *= 0.1 * 65536.0 * fontPixelPerDVIunit / shrinkfactor;

        QImage image(EPSfilename);
        image = image.scaled((int)(bbox_width), (int)(bbox_height),
                             Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        foreGroundPainter->drawImage(
            ((int)((currinf.data.dvi_h) / (shrinkfactor * 65536))),
            currinf.data.pxl_v - (int)bbox_height, image);
        return;
    }

    if(!_postscript || !QFile::exists(EPSfilename))
    {
        // Don't show PostScript, just draw the bounding box. For this,
        // calculate the size of the bounding box in Pixels.
        double bbox_width = urx - llx;
        double bbox_height = ury - lly;

        if((rwi != 0) && (bbox_width != 0))
        {
            bbox_height *= rwi / bbox_width;
            bbox_width = rwi;
        }
        if((rhi != 0) && (bbox_height != 0))
        {
            bbox_width *= rhi / bbox_height;
            bbox_height = rhi;
        }

        double fontPixelPerDVIunit = dviFile->getCmPerDVIunit() * 1200.0 / 2.54;

        bbox_width *= 0.1 * 65536.0 * fontPixelPerDVIunit / shrinkfactor;
        bbox_height *= 0.1 * 65536.0 * fontPixelPerDVIunit / shrinkfactor;

        QRect bbox(((int)((currinf.data.dvi_h) / (shrinkfactor * 65536))),
                   currinf.data.pxl_v - (int)bbox_height, (int)bbox_width,
                   (int)bbox_height);

        foreGroundPainter->save();

        if(QFile::exists(EPSfilename))
        {
            foreGroundPainter->setBrush(Qt::lightGray);
        }
        else
        {
            foreGroundPainter->setBrush(Qt::red);
        }
        foreGroundPainter->setPen(Qt::black);
        foreGroundPainter->drawRoundedRect(bbox, 2, 2);
        QFont f = foreGroundPainter->font();
        f.setPointSize(8);
        foreGroundPainter->setFont(f);
        /* if the fonts are mapped for some reason to X bitmap fonts,
           the call to drawText() in the non-GUI thread will produce a crash.
           Ensure that the rendering of the text is performed only if
           the threaded font rendering is available */
        if(QFile::exists(EPSfilename))
        {
            foreGroundPainter->drawText(bbox, (int)(Qt::AlignCenter),
                                        EPSfilename);
        }
        else
        {
            foreGroundPainter->drawText(
                bbox, (int)(Qt::AlignCenter),
                i18n("File not found: \n %1", EPSfilename_orig));
        }
        foreGroundPainter->restore();
    }

    return;
}

void dviRenderer::TPIC_flushPath_special()
{
#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "TPIC special flushPath";
#endif

    if(number_of_elements_in_path == 0)
    {
        printErrorMsgForSpecials(QStringLiteral(
            "TPIC special flushPath called when path was empty."));
        return;
    }

    QPen pen(Qt::black, (int)(penWidth_in_mInch * resolutionInDPI / 1000.0 +
                              0.5));  // Sets the pen size in milli-inches
    foreGroundPainter->setPen(pen);
    foreGroundPainter->drawPolyline(TPIC_path.constData(),
                                    number_of_elements_in_path);
    number_of_elements_in_path = 0;
}

void dviRenderer::TPIC_addPath_special(const QString& cp)
{
#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "TPIC special addPath: " << cp;
#endif

    // Adds a point to the path list
    QString cp_noWhiteSpace = cp.trimmed();
    bool ok;
    float xKoord = cp_noWhiteSpace.section(QLatin1Char(' '), 0, 0).toFloat(&ok);
    if(ok == false)
    {
        printErrorMsgForSpecials(
            QStringLiteral(
                "TPIC special; cannot parse first argument in 'pn %1'.")
                .arg(cp));
        return;
    }
    float yKoord = cp_noWhiteSpace.section(QLatin1Char(' '), 1, 1).toFloat(&ok);
    if(ok == false)
    {
        printErrorMsgForSpecials(
            QStringLiteral(
                "TPIC special; cannot parse second argument in 'pn %1'.")
                .arg(cp));
        return;
    }

    float mag = dviFile->getMagnification() / 1000.0;

    int x = (int)(currinf.data.dvi_h / (shrinkfactor * 65536.0) +
                  mag * xKoord * resolutionInDPI / 1000.0 + 0.5);
    int y = (int)(currinf.data.pxl_v + mag * yKoord * resolutionInDPI / 1000.0 +
                  0.5);

    // Initialize the point array used to store the path
    if(TPIC_path.size() == 0)
    {
        number_of_elements_in_path = 0;
    }
    if(TPIC_path.size() == number_of_elements_in_path)
    {
        TPIC_path.resize(number_of_elements_in_path + 100);
    }
    TPIC_path.setPoint(number_of_elements_in_path++, x, y);
}

void dviRenderer::TPIC_setPen_special(const QString& cp)
{
#ifdef DEBUG_SPECIAL
    qCDebug(OkularDviDebug) << "TPIC special setPen: " << cp;
#endif

    // Sets the pen size in milli-inches
    bool ok;
    penWidth_in_mInch = cp.trimmed().toFloat(&ok);
    if(ok == false)
    {
        printErrorMsgForSpecials(
            QStringLiteral("TPIC special; cannot parse argument in 'pn %1'.")
                .arg(cp));
        penWidth_in_mInch = 0.0;
        return;
    }
}

void dviRenderer::applicationDoSpecial(char* cp)
{
    QString special_command = QString::fromLocal8Bit(cp);

    // First come specials which is only interpreted during rendering,
    // and NOT during the prescan phase

    // font color specials
    if(qstrnicmp(cp, "color", 5) == 0)
    {
        color_special(special_command.mid(5));
        return;
    }

    // HTML reference
    if(qstrnicmp(cp, "html:<A href=", 13) == 0)
    {
        html_href_special(special_command.mid(14));
        return;
    }

    // HTML anchor end
    if(qstrnicmp(cp, "html:</A>", 9) == 0)
    {
        html_anchor_end();
        return;
    }

    // TPIC specials
    if(qstrnicmp(cp, "pn", 2) == 0)
    {
        TPIC_setPen_special(special_command.mid(2));
        return;
    }
    if(qstrnicmp(cp, "pa ", 3) == 0)
    {
        TPIC_addPath_special(special_command.mid(3));
        return;
    }
    if(qstrnicmp(cp, "fp", 2) == 0)
    {
        TPIC_flushPath_special();
        return;
    }

    // Encapsulated Postscript File
    if(qstrnicmp(cp, "PSfile=", 7) == 0)
    {
        epsf_special(special_command.mid(7));
        return;
    }

    // source special
    if(qstrnicmp(cp, "src:", 4) == 0)
    {
        source_special(special_command.mid(4));
        return;
    }

    // Unfortunately, in some TeX distribution the hyperref package uses
    // the dvips driver by default, rather than the hypertex driver. As
    // a result, the DVI files produced are full of PostScript that
    // specifies links and anchors, and KDVI would call the ghostscript
    // interpreter for every page which makes it really slow. This is a
    // major nuisance, so that we try to filter and interpret the
    // hypertex generated PostScript here.
    if(special_command.startsWith(QLatin1String("ps:SDict begin")))
    {
        // Hyperref: start of hyperref rectangle. At this stage it is not
        // yet clear if the rectangle will contain a hyperlink, an anchor,
        // or another type of object. We suspect that this rectangle will
        // define a hyperlink, allocate a QString and set HTML_href to
        // point to this string. The string contains the name of the
        // destination which ---due to the nature of the PostScript
        // language--- will be defined only after characters are drawn and
        // the hyperref rectangle has been closed. We use "glopglyph" as a
        // temporary name. Since the pointer HTML_href is not NULL, the
        // character drawing routines will now underline all characters in
        // blue to point out that they correspond to a hyperlink. Also, as
        // soon as characters are drawn, the drawing routines will
        // allocate a Hyperlink and add it to the top of the vector
        // currentlyDrawnPage->hyperLinkList.
        if(special_command == QLatin1String("ps:SDict begin H.S end"))
        {
            // At this stage, the vector 'hyperLinkList' should not contain
            // links with unspecified destinations (i.e. destination set to
            // 'glopglyph'). As a protection against bad DVI files, we make
            // sure to remove all link rectangles which point to
            // 'glopglyph'.
            while(!currentlyDrawnPage->hyperLinkList.isEmpty())
            {
                if(currentlyDrawnPage->hyperLinkList.last().linkText ==
                   QLatin1String("glopglyph"))
                {
                    currentlyDrawnPage->hyperLinkList.pop_back();
                }
                else
                {
                    break;
                }
            }

            HTML_href = new QString(QStringLiteral("glopglyph"));
            return;
        }

        // Hyperref: end of hyperref rectangle of unknown type or hyperref
        // link rectangle. In these cases we set HTML_href to NULL, which
        // causes the character drawing routines to stop drawing
        // characters underlined in blue. Note that the name of the
        // destination is still set to "glopglyph". In a well-formed DVI
        // file, this special command is immediately followed by another
        // special, where the destination is specified. This special is
        // treated below.
        if((special_command == QLatin1String("ps:SDict begin H.R end")) ||
           special_command.endsWith(QLatin1String("H.L end")))
        {
            if(HTML_href != nullptr)
            {
                delete HTML_href;
                HTML_href = nullptr;
            }
            return;  // end of hyperref rectangle
        }

        // Hyperref: end of anchor rectangle. If this special is
        // encountered, the rectangle, which was started with "ps:SDict
        // begin H.S end" does not contain a link, but an anchor for a
        // link. Anchors, however, have already been dealt with in the
        // prescan phase and will not be considered here. Thus, we set
        // HTML_href to NULL so that character drawing routines will no
        // longer underline hyperlinks in blue, and remove the link from
        // the hyperLinkList. NOTE: in a well-formed DVI file, the "H.A"
        // special comes directly after the "H.S" special. A
        // hyperlink-anchor rectangle therefore never contains characters,
        // so no character will by accidentally underlined in blue.
        if(special_command.endsWith(QLatin1String("H.A end")))
        {
            if(HTML_href != nullptr)
            {
                delete HTML_href;
                HTML_href = nullptr;
            }
            while(!currentlyDrawnPage->hyperLinkList.isEmpty())
            {
                if(currentlyDrawnPage->hyperLinkList.last().linkText ==
                   QLatin1String("glopglyph"))
                {
                    currentlyDrawnPage->hyperLinkList.pop_back();
                }
                else
                {
                    break;
                }
            }
            return;  // end of hyperref anchor
        }

        // Hyperref: specification of a hyperref link rectangle's
        // destination. As mentioned above, the destination of a hyperlink
        // is specified only AFTER the rectangle has been specified. We
        // will therefore go through the list of rectangles stored in
        // currentlyDrawnPage->hyperLinkList, find those whose destination
        // is open and fill in the value found here. NOTE: the character
        // drawing routines sometimes split a single hyperlink rectangle
        // into several rectangles (e.g. if the font changes, or when a
        // line break is encountered)
        if(special_command.startsWith(QLatin1String("ps:SDict begin [")) &&
           special_command.endsWith(QLatin1String(" pdfmark end")))
        {
            if(!currentlyDrawnPage->hyperLinkList.isEmpty())
            {
                QString targetName =
                    special_command.section(QLatin1Char('('), 1, 1)
                        .section(QLatin1Char(')'), 0, 0);
                QVector<Hyperlink>::iterator it;
                for(it = currentlyDrawnPage->hyperLinkList.begin();
                    it != currentlyDrawnPage->hyperLinkList.end(); ++it)
                {
                    if(it->linkText == QLatin1String("glopglyph"))
                    {
                        it->linkText = targetName;
                    }
                }
            }
            return;  // hyperref definition of link/anchor/bookmark/etc
        }
    }

    // Detect text rotation specials that are included by the graphicx
    // package. If one of these specials is found, the state of the
    // painter is saved, and the coordinate system is rotated
    // accordingly
    if(special_command.startsWith(
           QLatin1String("ps: gsave currentpoint currentpoint translate ")) &&
       special_command.endsWith(
           QLatin1String(" neg rotate neg exch neg exch translate")))
    {
        bool ok;
        double angle =
            special_command.section(QLatin1Char(' '), 5, 5).toDouble(&ok);
        if(ok == true)
        {
            int x = ((int)((currinf.data.dvi_h) / (shrinkfactor * 65536)));
            int y = currinf.data.pxl_v;

            foreGroundPainter->save();
            // Rotate about the current point
            foreGroundPainter->translate(x, y);
            foreGroundPainter->rotate(-angle);
            foreGroundPainter->translate(-x, -y);
        }
        else
        {
            printErrorMsgForSpecials(
                i18n("Error in DVIfile '%1', page %2. Could not interpret "
                     "angle in text rotation special.",
                     dviFile->filename, current_page));
        }
    }

    // The graphicx package marks the end of rotated text with this
    // special. The state of the painter is restored.
    if(special_command == QLatin1String("ps: currentpoint grestore moveto"))
    {
        foreGroundPainter->restore();
    }

    // The following special commands are not used here; they are of
    // interest only during the prescan phase. We recognize them here
    // anyway, to make sure that KDVI doesn't complain about
    // unrecognized special commands.
    if((cp[0] == '!') || (cp[0] == '"') ||
       (qstrnicmp(cp, "html:<A name=", 13) == 0) ||
       (qstrnicmp(cp, "ps:", 3) == 0) || (qstrnicmp(cp, "papersize", 9) == 0) ||
       (qstrnicmp(cp, "header", 6) == 0) ||
       (qstrnicmp(cp, "background", 10) == 0))
    {
        return;
    }

    printErrorMsgForSpecials(
        i18n("The special command '%1' is not implemented.", special_command));
    return;
}
