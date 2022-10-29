// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*- fontpool.h
//
// SPDX-FileCopyrightText: 2001-2004 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _FONTPOOL_H
#define _FONTPOOL_H

#include <QList>
#include <QObject>
#include <QProcess>
#include "TeXFontDefinition.h"
#include "fontEncodingPool.h"
#include "fontMap.h"

#ifdef HAVE_FREETYPE
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif

/**
 *  A list of fonts and a compilation of utility functions
 *
 * This class holds a list of fonts and is able to perform a number of
 * functions on each of the fonts. The main use of this class is that
 * it is able to control a concurrently running "kpsewhich" program
 * which is used to locate and load the fonts.
 *
 * @author Stefan Kebekus   <kebekus@kde.org>
 *
 **/

class fontPool : public QObject
{
    Q_OBJECT

public:
    // Default constructor.
    explicit fontPool(bool useFontHinting);

    // Default destructor.
    ~fontPool() override;

    /** Method used to set the MetafontMode for the PK font files. This
        data is used when loading fonts. Currently, a change here will be
        applied only to those font which were not yet loaded ---expect
        funny results when changing the data in the mid-work. */
    void setParameters(bool useFontHints);

    /** Sets the DVI file's path. This information is used to set the
        current working directory for the kpsewhich command, so that
        kpsewhich will find fonts that are stored in the DVI file's
        directory. */
    void setExtraSearchPath(const QString& path)
    {
        extraSearchPath = path;
    }

    /** Returns the path that is set as the current working directory
        for the kpsewhich command, so that kpsewhich will find fonts
        that are stored in the DVI file's directory. */
    QString getExtraSearchPath() const
    {
        return extraSearchPath;
    }

    /** Sets the resolution of the output device. */
    void setDisplayResolution(double _displayResolution_in_dpi);

    /** Sets the number of centimeters per DVI unit. */
    void setCMperDVIunit(double CMperDVI);

    double getCMperDVIunit() const
    {
        return CMperDVIunit;
    }

    // If return value is true, font hinting should be used if possible
    bool getUseFontHints() const
    {
        return useFontHints;
    }

    // This method adds a font to the list. If the font is not currently
    // loaded, it's file will be located and font::load_font will be
    // called. Since this is done using a concurrently running process,
    // there is no guarantee that the loading is already performed when
    // the method returns.
    TeXFontDefinition* appendx(const QString& fontname, quint32 checksum,
                               quint32 scale, double enlargement);

    // This is the list which actually holds pointers to the fonts
    QList<TeXFontDefinition*> fontList;

    // This method marks all fonts in the fontpool as "not in use". The
    // fonts are, however, not removed from memory until the method
    // release_fonts is called. The method is called when the dvi-file
    // is closed. Because the next dvi-file which will be loaded is
    // likely to use most of the fonts again, this method implements a
    // convenient way of re-using fonts without loading them repeatedly.
    void mark_fonts_as_unused();

    /** This methods removes all fonts from the fontpool (and thus from
        memory) which are labeled "not in use". For explanation, see the
        mark_fonts_as_unused method. */
    void release_fonts();

#ifdef HAVE_FREETYPE
    /** A handle to the FreeType library, which is used by TeXFont_PFM
        font objects, if KDVI is compiled with FreeType support.  */
    FT_Library FreeType_library;

    /** Simple marker. Set to 'true', if the FreeType library was loaded
        successfully */
    bool FreeType_could_be_loaded;

    /** This maps TeX font names to font file names, full font names and
        encodings. See the file 'fontMap.h' for a detailed
        description. */
    fontMap fontsByTeXName;

    /** This is a list of known font encodings which can be conveniently
        accessed by name. */
    fontEncodingPool encodingPool;
#endif

    /** This flag is set during the construction of the fontPool
        object. It indicates if the QT library supports the alpha
        channel of pixmaps. Experiments show that --depending of the
        configuration of QT at compile and runtime or the availability
        of the XFt extension, alpha channels are either supported, or
        silently converted to 1-bit masks. The rendering routines in the
        TeXFont implementation use this flag to choose the appropriated
        drawing routines for the different setups. */
    bool QPixmapSupportsAlpha;

Q_SIGNALS:
    /** Passed through to the top-level kpart. */
    void error(const QString& message, int duration);
    void warning(const QString& message, int duration);
    void notice(const QString& message, int duration);

public Q_SLOTS:
    // Locates font files on the disk using the kpsewhich program.  If
    // 'locateTFMonly' is true, the method does not look for PFB- or
    // PK-fonts. Instead, only TFM-files are searched. This option can be
    // used as a 'last resort': if a found cannot be found, one can at
    // least use the TFM file to draw filled rectangles for the
    // characters. If not null, the bool pointed at by virtualFontsFound
    // is set to true if one of the fonts found is a virtual font. If no
    // virtual font is found, the variable remains untouched.
    void locateFonts();

private:
    // This method goes through the list of fonts, and marks each of them
    // as 'located'. Used, e.g. after a fatal error in the font lookup
    // process to ensure that the problematic kpsewhich is not used again
    void markFontsAsLocated();

    // Checks if all the fonts file names have been located, and returns
    // true if that is so.
    bool areFontsLocated();

    // This flag is used by PFB fonts to determine if the FREETYPE engine
    // should use hinted fonts or not
    bool useFontHints;

    // Resolution of the output device.
    double displayResolution_in_dpi;

    // Number of centimeters per DVI unit
    double CMperDVIunit;

    /** Members used for font location */

    // Locates font files on the disk using the kpsewhich program.  If
    // 'locateTFMonly' is true, the method does not look for PFB- or
    // PK-fonts. Instead, only TFM-files are searched. This option can be
    // used as a 'last resort': if a found cannot be found, one can at
    // least use the TFM file to draw filled rectangles for the
    // characters. If not null, the bool pointed at by virtualFontsFound
    // is set to true if one of the fonts found is a virtual font. If no
    // virtual font is found, the variable remains untouched.
    void locateFonts(bool makePK, bool locateTFMonly,
                     bool* virtualFontsFound = nullptr);

    // This QString is used internally by the mf_output_receiver()
    // method.  This string is set to QString() in locateFonts(bool,
    // bool, bool *). Values are set and read by the
    // mf_output_receiver(...)  method
    QString MetafontOutput;

    // This QString is used to collect the output of kpsewhich and
    // MetaFont. The string is set to QString() in the
    // locateFonts()-method, and content is gathered by the
    // mf_output_receiver(). This string is used by locateFonts() and
    // locateFonts(bool, bool, bool *) to display error messages.
    QString kpsewhichOutput;

    // This string is set to the DVI file's path. It is used to set the
    // current working directory for the kpsewhich command, so that
    // kpsewhich will find fonts that are stored in the DVI file's
    // directory. Used by the locateFonts() and the locateFonts(bool,
    // bool, bool *) method. Values are set by the
    // setExtraSearchPath(...) method
    QString extraSearchPath;

    // The handle on the external process.
    QProcess* kpsewhich_;

private Q_SLOTS:
    // This slot is called when MetaFont is run via the kpsewhich program.
    // The MetaFont output is transmitted to the fontpool via the @c kpsewhich_
    // member variable. This method passes on progress information to the
    // fontProgress Dialog  and collects the output of MetaFont in the
    // @c MetafontOutput member variable.
    void mf_output_receiver();
};

#endif  // ifndef _FONTPOOL_H
