// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// fontMap.h
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _FONTMAP_H
#define _FONTMAP_H

#include <QMap>
#include <QString>

/**
 * This class represents one line of a font map file, and contains
 * three pieces of information about a font: its file name, the full
 * name of the font, and the encoding.
 *
 * @author Stefan Kebekus   <kebekus@kde.org>
 **/

class fontMapEntry
{
public:
    // File name of the font WITHOUT the path. The full path name must
    // be looked by using the kpathsea library, e.g. by means of the
    // kpsewhich command. A valid entry would be 'ubkd8a.pfb'
    QString fontFileName;

    // This string contains the full name of the font,
    // e.g. 'URWBookmanL-DemiBold'
    QString fullFontName;

    // If the font requires an encoding (see fontEncoding.h for an
    // explanation), this string is not empty and contains the name of
    // the encoding, e.g. '8r'. The path of the associated encoding file
    // (on the author's machine: /usr/share/texmf/dvips/psnfss/8r.enc)
    // must be looked up using the kpsewhich command.
    QString fontEncoding;

    // Some fonts need to be slanted, and the font map file defines by
    // how much. This field is set to 0.0 if no slanting is specified in
    // the map file.
    double slant;
};

/**
 * This class represents the contents of the font map file "ps2pk.map"
 *
 * A font map file is part of the machinery that make it possible to
 * access PostScript (and possibly also TrueType and OpenType) fonts
 * from a DVI file.
 *
 * Long time ago, when TeX was only used with MetaFont fonts, the DVI
 * file would specify a font by giving an 8-character name, such as
 * 'cmr10'. The DVI previewer would then locate the associated PK font
 * file, load it, and retrieve the character shaped. Happy times, they
 * were.
 *
 * Today TeX is also used to access Type1 and TrueType fonts, which do
 * not fit well into the TeX naming scheme. Like in earlier times, the
 * DVI file specifies the name of a font, e.g. 'rpbkd', but nowadays
 * the DVI previewer cannot just go and find a file 'rpbkd.pk'. No,
 * no. Instead, the DVI previewr needs to look up the meaning of
 * 'rpbkd' in a map-file. There it finds that 'rpbkd' refers to a font
 * called 'URWBookmanL-DemiBold', to be found under the file name
 * 'ubkd8a.pfb' whose glyphs are to be encoded using the '8a' encoding
 * file (see the header file 'fontEncoding.h' for more information
 * about encodings)
 *
 * Such map files exists for all dvi output drivers that are part of
 * the TeX distribution that is installed on your
 * computer. Unfortunately, KDVI is not part of a TeX distribution,
 * and therefore does not have its own map file. As a workaround, KDVI
 * uses the map file of the program ps2pk which is similar to KDVI in
 * that the ps2pk driver does not have built-in fonts, unlike the
 * PostScript printers for which dvips is used.
 *
 * @author Stefan Kebekus   <kebekus@kde.org>
 *
 **/

class fontMap
{
public:
    /** The default constructor will try to locate the file 'ps2pk.map',
        and read its contents. If the file 'ps2pk.map' cannot be found
        using the kpsewhich command, or if it cannot be read, or is
        (partially) in an improper format, an error message is printed
        to stderr using the kDebug() stream. */
    fontMap();

    /** find the name of a font file (e.g. 'ubkd8a.pfb') from a TeX font
        name (e.g. 'rpbkd'). This method return a reference to
        QString() if the font could not be found. */
    const QString &findFileName(const QString &TeXName);

    /** find the name of a font (e.g. 'URWBookmanL-DemiBold') from a TeX
        font name (e.g. 'rpbkd'). This method return a reference to
        QString() if the font could not be found. */
    const QString &findFontName(const QString &TeXName);

    /** find the name of an encoding file for a font (e.g. '8r') from a
        TeX font name (e.g. 'rpbkd'). This method return a reference to
        QString() if the font could not be found. */
    const QString &findEncoding(const QString &TeXName);

    /** This method finds the slant of a font. Returns 0.0 if no slant
        was defined. */
    double findSlant(const QString &TeXName);

private:
    /** This member maps TeX font names mapEntry classes that contain
        the font's filenames, full font names and encodings. */
    QMap<QString, fontMapEntry> fontMapEntries;
};

#endif // ifndef _FONTMAP_H
