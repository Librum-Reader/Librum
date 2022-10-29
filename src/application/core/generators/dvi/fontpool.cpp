// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
//
// fontpool.cpp
//
// SPDX-FileCopyrightText: 2001-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include "fontpool.h"
#include <config.h>
#include <math.h>
#include <KLocalizedString>
#include <QApplication>
#include <QPainter>
#include <QStandardPaths>
#include <cmath>
#include "TeXFont.h"
#include "debug_dvi.h"

//#define DEBUG_FONTPOOL

// List of permissible MetaFontModes which are supported by kdvi.

// const char *MFModes[]       = { "cx", "ljfour", "lexmarks" };
// const char *MFModenames[]   = { "Canon CX", "LaserJet 4", "Lexmark S" };
// const int   MFResolutions[] = { 300, 600, 1200 };

#ifdef PERFORMANCE_MEASUREMENT
QTime fontPoolTimer;
bool fontPoolTimerFlag;
#endif

fontPool::fontPool(bool useFontHinting)
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug) << "fontPool::fontPool() called";
#endif

    setObjectName(QStringLiteral("Font Pool"));

    displayResolution_in_dpi = 100.0;  // A not-too-bad-default
    useFontHints = useFontHinting;
    CMperDVIunit = 0;
    extraSearchPath.clear();

#ifdef HAVE_FREETYPE
    // Initialize the Freetype Library
    if(FT_Init_FreeType(&FreeType_library) != 0)
    {
        qCCritical(OkularDviDebug) << "Cannot load the FreeType library. KDVI "
                                      "proceeds without FreeType support.";
        FreeType_could_be_loaded = false;
    }
    else
    {
        FreeType_could_be_loaded = true;
    }
#endif

    // Check if the QT library supports the alpha channel of
    // QImages. Experiments show that --depending of the configuration
    // of QT at compile and runtime or the availability of the XFt
    // extension, alpha channels are either supported, or silently
    // ignored.
    QImage start(
        1, 1,
        QImage::Format_ARGB32);  // Generate a 1x1 image, black with alpha=0x10
    quint32* destScanLine = reinterpret_cast<quint32*>(start.scanLine(0));
    *destScanLine = 0x80000000;
    QPixmap intermediate = QPixmap::fromImage(start);
    QPixmap dest(1, 1);
    dest.fill(Qt::white);
    QPainter paint(&dest);
    paint.drawPixmap(0, 0, intermediate);
    paint.end();
    start = dest.toImage().convertToFormat(QImage::Format_ARGB32);
    quint8 result = *(start.scanLine(0)) & 0xff;

    if((result == 0xff) || (result == 0x00))
    {
#ifdef DEBUG_FONTPOOL
        qCDebug(OkularDviDebug) << "fontPool::fontPool(): QPixmap does not "
                                   "support the alpha channel";
#endif
        QPixmapSupportsAlpha = false;
    }
    else
    {
#ifdef DEBUG_FONTPOOL
        qCDebug(OkularDviDebug)
            << "fontPool::fontPool(): QPixmap supports the alpha channel";
#endif
        QPixmapSupportsAlpha = true;
    }
}

fontPool::~fontPool()
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug) << "fontPool::~fontPool() called";
#endif

    // need to manually clear the fonts _before_ freetype gets unloaded
    qDeleteAll(fontList);
    fontList.clear();

#ifdef HAVE_FREETYPE
    if(FreeType_could_be_loaded == true)
    {
        FT_Done_FreeType(FreeType_library);
    }
#endif
}

void fontPool::setParameters(bool _useFontHints)
{
    // Check if glyphs need to be cleared
    if(_useFontHints != useFontHints)
    {
        double displayResolution = displayResolution_in_dpi;
        QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
        for(; it_fontp != fontList.end(); ++it_fontp)
        {
            TeXFontDefinition* fontp = *it_fontp;
            fontp->setDisplayResolution(displayResolution * fontp->enlargement);
        }
    }

    useFontHints = _useFontHints;
}

TeXFontDefinition* fontPool::appendx(const QString& fontname, quint32 checksum,
                                     quint32 scale, double enlargement)
{
    // Reuse font if possible: check if a font with that name and
    // natural resolution is already in the fontpool, and use that, if
    // possible.
    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;
        if((fontname == fontp->fontname) &&
           ((int)(enlargement * 1000.0 + 0.5)) ==
               (int)(fontp->enlargement * 1000.0 + 0.5))
        {
            // if font is already in the list
            fontp->mark_as_used();
            return fontp;
        }
    }

    // If font doesn't exist yet, we have to generate a new font.

    double displayResolution = displayResolution_in_dpi;

    TeXFontDefinition* fontp =
        new TeXFontDefinition(fontname, displayResolution * enlargement,
                              checksum, scale, this, enlargement);
    if(fontp == nullptr)
    {
        qCCritical(OkularDviDebug)
            << "Could not allocate memory for a font structure";
        exit(0);
    }
    fontList.append(fontp);

#ifdef PERFORMANCE_MEASUREMENT
    fontPoolTimer.start();
    fontPoolTimerFlag = false;
#endif

    // Now start kpsewhich/MetaFont, etc. if necessary
    return fontp;
}

bool fontPool::areFontsLocated()
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug) << "fontPool::areFontsLocated() called";
#endif

    // Is there a font whose name we did not try to find out yet?
    QList<TeXFontDefinition*>::const_iterator cit_fontp = fontList.constBegin();
    for(; cit_fontp != fontList.constEnd(); ++cit_fontp)
    {
        TeXFontDefinition* fontp = *cit_fontp;
        if(!fontp->isLocated())
        {
            return false;
        }
    }

#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug)
        << "... yes, all fonts are located (but not necessarily loaded).";
#endif
    return true;  // That says that all fonts are located.
}

void fontPool::locateFonts()
{
    kpsewhichOutput.clear();

    // First, we try and find those fonts which exist on disk
    // already. If virtual fonts are found, they will add new fonts to
    // the list of fonts whose font files need to be located, so that we
    // repeat the lookup.
    bool vffound;
    do
    {
        vffound = false;
        locateFonts(false, false, &vffound);
    }
    while(vffound);

    // If still not all fonts are found, look again, this time with
    // on-demand generation of PK fonts enabled.
    if(!areFontsLocated())
    {
        locateFonts(true, false);
    }

    // If still not all fonts are found, we look for TFM files as a last
    // resort, so that we can at least draw filled rectangles for
    // characters.
    if(!areFontsLocated())
    {
        locateFonts(false, true);
    }

    // If still not all fonts are found, we give up. We mark all fonts
    // as 'located', so that we won't look for them any more, and
    // present an error message to the user.
    if(!areFontsLocated())
    {
        markFontsAsLocated();
        Q_EMIT error(
            i18n("<qt><p>Okular was not able to locate all the font files "
                 "which are necessary to display the current DVI file. "
                 "Your document might be unreadable.</p>"
                 "<p><small><b>PATH:</b> %1</small></p>"
                 "<p><small>%2</small></p></qt>",
                 QString::fromLocal8Bit(qgetenv("PATH")),
                 kpsewhichOutput.replace(QLatin1String("\n"),
                                         QLatin1String("<br/>"))),
            -1);
    }
}

void fontPool::locateFonts(bool makePK, bool locateTFMonly,
                           bool* virtualFontsFound)
{
    // Make sure kpsewhich is in PATH and not just in the CWD
    static const QString kpsewhichFullPath =
        QStandardPaths::findExecutable(QStringLiteral("kpsewhich"));
    if(kpsewhichFullPath.isEmpty())
    {
        return;
    }

    // Set up the kpsewhich process. If pass == 0, look for vf-fonts and
    // disable automatic font generation as vf-fonts can't be
    // generated. If pass == 0, enable font generation, if it was
    // enabled globally.

    // Now generate the command line for the kpsewhich
    // program. Unfortunately, this can be rather long and involved...
    QStringList kpsewhich_args;
    kpsewhich_args << QStringLiteral("--dpi") << QStringLiteral("1200")
                   << QStringLiteral("--mode") << QStringLiteral("lexmarks");

    // Disable automatic pk-font generation.
    kpsewhich_args << QString::fromLocal8Bit(makePK ? "--mktex" : "--no-mktex")
                   << QStringLiteral("pk");

    // Names of fonts that shall be located
    quint16 numFontsInJob = 0;
    QList<TeXFontDefinition*>::const_iterator cit_fontp = fontList.constBegin();
    for(; cit_fontp != fontList.constEnd(); ++cit_fontp)
    {
        TeXFontDefinition* fontp = *cit_fontp;
        if(!fontp->isLocated())
        {
            numFontsInJob++;

            if(locateTFMonly == true)
            {
                kpsewhich_args << QStringLiteral("%1.tfm").arg(fontp->fontname);
            }
            else
            {
#ifdef HAVE_FREETYPE
                if(FreeType_could_be_loaded == true)
                {
                    const QString& filename =
                        fontsByTeXName.findFileName(fontp->fontname);
                    if(!filename.isEmpty())
                    {
                        kpsewhich_args << QStringLiteral("%1").arg(filename);
                    }
                }
#endif
                kpsewhich_args
                    << QStringLiteral("%1.vf").arg(fontp->fontname)
                    << QStringLiteral("%1.1200pk").arg(fontp->fontname);
            }
        }
    }

    if(numFontsInJob == 0)
    {
        return;
    }

    // If PK fonts are generated, the kpsewhich command will re-route
    // the output of MetaFont into its stderr. Here we make sure this
    // output is intercepted and parsed.
    kpsewhich_ = new QProcess();
    connect(kpsewhich_, &QProcess::readyReadStandardError, this,
            &fontPool::mf_output_receiver);

    // Now run... kpsewhich. In case of error, kick up a fuss.
    // This string is not going to be quoted, as it might be were it
    // a real command line, but who cares?
    const QString kpsewhich_exe = QStringLiteral("kpsewhich");
    kpsewhichOutput +=
        QStringLiteral("<b>") + kpsewhich_exe + QLatin1Char(' ') +
        kpsewhich_args.join(QStringLiteral(" ")) + QStringLiteral("</b>");

    kpsewhich_->start(kpsewhichFullPath, kpsewhich_args,
                      QIODevice::ReadOnly | QIODevice::Text);
    if(!kpsewhich_->waitForStarted())
    {
        QApplication::restoreOverrideCursor();
        Q_EMIT error(
            i18n("<qt><p>There were problems running <em>kpsewhich</em>. As a "
                 "result, "
                 "some font files could not be located, and your document "
                 "might be unreadable.<br/>"
                 "Possible reason: the <em>kpsewhich</em> program is perhaps "
                 "not installed on your system, "
                 "or it cannot be found in the current search path.</p>"
                 "<p><small><b>PATH:</b> %1</small></p>"
                 "<p><small>%2</small></p></qt>",
                 QString::fromLocal8Bit(qgetenv("PATH")),
                 kpsewhichOutput.replace(QLatin1String("\n"),
                                         QLatin1String("<br/>"))),
            -1);

        // This makes sure the we don't try to run kpsewhich again
        markFontsAsLocated();
        delete kpsewhich_;
        return;
    }
    // We wait here while the external program runs concurrently.
    kpsewhich_->waitForFinished();

    // Handle fatal errors.
    int const kpsewhich_exit_code = kpsewhich_->exitCode();
    if(kpsewhich_exit_code < 0)
    {
        Q_EMIT warning(i18n("<qt>The font generation by <em>kpsewhich</em> was "
                            "aborted (exit code %1, error %2). As a "
                            "result, some font files could not be located, and "
                            "your document might be unreadable.</qt>",
                            kpsewhich_exit_code, kpsewhich_->errorString()),
                       -1);

        // This makes sure the we don't try to run kpsewhich again
        if(makePK == false)
        {
            markFontsAsLocated();
        }
    }

    // Create a list with all filenames found by the kpsewhich program.
    const QStringList fileNameList =
        QString::fromLocal8Bit(kpsewhich_->readAll())
            .split(QLatin1Char('\n'), QString::SkipEmptyParts);

    // Now associate the file names found with the fonts
    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;

        if(fontp->filename.isEmpty() == true)
        {
            QStringList matchingFiles;
#ifdef HAVE_FREETYPE
            const QString& fn = fontsByTeXName.findFileName(fontp->fontname);
            if(!fn.isEmpty())
            {
                matchingFiles = fileNameList.filter(fn);
            }
#endif
            if(matchingFiles.isEmpty() == true)
            {
                matchingFiles += fileNameList.filter(
                    QLatin1Char('/') + fontp->fontname + QLatin1Char('.'));
            }

            if(matchingFiles.isEmpty() != true)
            {
#ifdef DEBUG_FONTPOOL
                qCDebug(OkularDviDebug) << "Associated " << fontp->fontname
                                        << " to " << matchingFiles.first();
#endif
                QString fname = matchingFiles.first();
                fontp->fontNameReceiver(fname);
                fontp->flags |= TeXFontDefinition::FONT_KPSE_NAME;
                if(fname.endsWith(QLatin1String(".vf")))
                {
                    if(virtualFontsFound != nullptr)
                    {
                        *virtualFontsFound = true;
                    }
                    // Constructing a virtual font will most likely insert other
                    // fonts into the fontList. After that, fontList.next() will
                    // no longer work. It is therefore safer to start over.
                    it_fontp = fontList.begin();
                    continue;
                }
            }
        }  // of if (fontp->filename.isEmpty() == true)
    }
    delete kpsewhich_;
}

void fontPool::setCMperDVIunit(double _CMperDVI)
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug)
        << "fontPool::setCMperDVIunit( " << _CMperDVI << " )";
#endif

    if(CMperDVIunit == _CMperDVI)
    {
        return;
    }

    CMperDVIunit = _CMperDVI;

    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;
        fontp->setDisplayResolution(displayResolution_in_dpi *
                                    fontp->enlargement);
    }
}

void fontPool::setDisplayResolution(double _displayResolution_in_dpi)
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug)
        << "fontPool::setDisplayResolution( displayResolution_in_dpi="
        << _displayResolution_in_dpi << " ) called";
#endif

    // Ignore minute changes by less than 2 DPI. The difference would
    // hardly be visible anyway. That saves a lot of re-painting,
    // e.g. when the user resizes the window, and a flickery mouse
    // changes the window size by 1 pixel all the time.
    if(fabs(displayResolution_in_dpi - _displayResolution_in_dpi) <= 2.0)
    {
#ifdef DEBUG_FONTPOOL
        qCDebug(OkularDviDebug) << "fontPool::setDisplayResolution(...): "
                                   "resolution wasn't changed. Aborting.";
#endif
        return;
    }

    displayResolution_in_dpi = _displayResolution_in_dpi;
    double displayResolution = displayResolution_in_dpi;

    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;
        fontp->setDisplayResolution(displayResolution * fontp->enlargement);
    }

    // Do something that causes re-rendering of the dvi-window
    /*@@@@
    Q_EMIT fonts_have_been_loaded(this);
    */
}

void fontPool::markFontsAsLocated()
{
    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;
        fontp->markAsLocated();
    }
}

void fontPool::mark_fonts_as_unused()
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug) << "fontPool::mark_fonts_as_unused() called";
#endif

    QList<TeXFontDefinition*>::iterator it_fontp = fontList.begin();
    for(; it_fontp != fontList.end(); ++it_fontp)
    {
        TeXFontDefinition* fontp = *it_fontp;
        fontp->flags &= ~TeXFontDefinition::FONT_IN_USE;
    }
}

void fontPool::release_fonts()
{
#ifdef DEBUG_FONTPOOL
    qCDebug(OkularDviDebug) << "Release_fonts";
#endif

    QMutableListIterator<TeXFontDefinition*> it_fontp(fontList);
    while(it_fontp.hasNext())
    {
        TeXFontDefinition* fontp = it_fontp.next();
        if((fontp->flags & TeXFontDefinition::FONT_IN_USE) !=
           TeXFontDefinition::FONT_IN_USE)
        {
            delete fontp;
            it_fontp.remove();
        }
    }
}

void fontPool::mf_output_receiver()
{
    const QString output_data =
        QString::fromLocal8Bit(kpsewhich_->readAllStandardError());

    kpsewhichOutput.append(output_data);
    MetafontOutput.append(output_data);

    // We'd like to print only full lines of text.
    int numleft;
    while((numleft = MetafontOutput.indexOf(QLatin1Char('\n'))) != -1)
    {
        QString line = MetafontOutput.left(numleft + 1);
#ifdef DEBUG_FONTPOOL
        qCDebug(OkularDviDebug) << "MF OUTPUT RECEIVED: " << line;
#endif
        // If the Output of the kpsewhich program contains a line starting
        // with "kpathsea:", this means that a new MetaFont-run has been
        // started. We filter these lines out and update the display
        // accordingly.
        int startlineindex = line.indexOf(QStringLiteral("kpathsea:"));
        if(startlineindex != -1)
        {
            int endstartline =
                line.indexOf(QStringLiteral("\n"), startlineindex);
            QString startLine =
                line.mid(startlineindex, endstartline - startlineindex);

            // The last word in the startline is the name of the font which we
            // are generating. The second-to-last word is the resolution in
            // dots per inch. Display this info in the text label below the
            // progress bar.
            int lastblank = startLine.lastIndexOf(QLatin1Char(' '));
            QString fontName = startLine.mid(lastblank + 1);
            int secondblank =
                startLine.lastIndexOf(QLatin1Char(' '), lastblank - 1);
            QString dpi =
                startLine.mid(secondblank + 1, lastblank - secondblank - 1);

            Q_EMIT warning(
                i18n("Currently generating %1 at %2 dpi...", fontName, dpi),
                -1);
        }
        MetafontOutput = MetafontOutput.remove(0, numleft + 1);
    }
}
