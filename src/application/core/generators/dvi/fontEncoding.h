// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// fontEncoding.h
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _FONTENCODING_H
#define _FONTENCODING_H

#include <QString>

/**
 * This class represents the contents of a font encoding file,
 * e.g. "8r.enc"
 *
 * Explanation of font encodings: TeX was designed to only use
 * MetaFont fonts. A DVI file refers to a MetaFont font by giving an
 * at-most-8-character name, such as 'cmr10'. The DVI previewer would
 * then locate the associated PK font file (e.g. cmr10.600pk), load
 * it, and retrieve the character shaped.
 *
 * Today TeX is also used to access Type1 and TrueType fonts, which it
 * was never designed to do. As in the case of MetaFont font, the DVI
 * file specifies the name of a font, e.g. 'rpbkd', and the DVI
 * previewer finds the associated font file 'ubkd8a.pfb' by means of a
 * map file (see fontMap.h). The font map file also specifies an
 * encoding (e.g. '8r', to be found in a file '8r.enc'). Font
 * encodings are necessary because TeX can only use the first 256
 * characters of a font, while modern PostScript fonts often contain
 * more.
 *
 * In a PostScript font, glyphs can often be accessed in two ways:
 *
 * (a) by an integer, the 'glyph index', which need not be
 * positive. Glyph indices can be found in every font.
 *
 * (b) by the name of the glyph, such as 'A', 'plusminus' or
 * 'ogonek'. Note: Not all fonts contain glyph names, and if a font
 * contains glyph names, they are not always reliable.
 *
 * An encoding file is essentially a list of 256 names of glyphs that
 * TeX wishes to use from a certain font. If the font contains more
 * than 256 glyphs, TeX is still limited to use at most 256 glyphs. If
 * more glyphs are required, TeX can probably use the same font under
 * a different name and with a different encoding ---the map file
 * (fontMap.h) can probably see to that.
 *
 * Summing up: this class contains 256 glyph names read from an
 * encoding file during the construction of this class.
 *
 * @author Stefan Kebekus   <kebekus@kde.org>
 *
 **/

class fontEncoding
{
public:
    // The constructor takes the name of an encoding file, such as
    // '8r.enc', locate the file on the hard disk using the 'kpsewhich'
    // command, reads it in and parses it. If the file cannot be
    // located, opened or parsed, errors are printed using the kError()
    // channel, and the array glyphNameVector will contain empty
    // strings.
    explicit fontEncoding(const QString &encName);

    // Full name of the encoding, as read from the encoding file
    QString encodingFullName;

    // List of 256 glyph names. The name can be '.notdef' to indicate
    // that a certain position is left open, or empty, if the encoding
    // file did not contain 256 characters or could not be properly read
    QString glyphNameVector[256];

    // Returns 'true' if the encoding file was found and could
    // successfully be loaded.
    bool isValid()
    {
        return _isValid;
    }

private:
    // Set by the constructor to 'true', if the encoding file was found
    // and could be loaded successfully.
    bool _isValid;
};

#endif
