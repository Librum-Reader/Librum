// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
/*
 * The layout of a font information block.
 * There is one of these for every loaded font or magnification thereof.
 * Duplicates are eliminated:  this is necessary because of possible recursion
 * in virtual fonts.
 *
 * Also note the strange units.  The design size is in 1/2^20 point
 * units (also called micro-points), and the individual character widths
 * are in the TFM file in 1/2^20 ems units, i.e., relative to the design size.
 *
 * We then change the sizes to SPELL units (unshrunk pixel / 2^16).
 */

#ifndef _FONT_H
#define _FONT_H

#include <QHash>
#include <QString>

class dviRenderer;
class TeXFont;

typedef void (dviRenderer::*set_char_proc)(unsigned int, unsigned int);

// Per character information for virtual fonts

class macro
{
public:
    macro();
    ~macro();

    macro(const macro&) = delete;
    macro& operator=(const macro&) = delete;

    // address of first byte of macro
    unsigned char* pos;
    // address of last+1 byte
    unsigned char* end;
    // DVI units to move reference point
    qint32 dvi_advance_in_units_of_design_size_by_2e20;
    // if memory at pos should be returned on destruction
    bool free_me;
};

class TeXFontDefinition
{
public:
    // Currently, kdvi supports fonts with at most 256 characters to
    // comply with "The DVI Driver Standard, Level 0". If you change
    // this value here, make sure to go through all the source and
    // ensure that character numbers are stored in ints rather than
    // unsigned chars.
    static const unsigned int max_num_of_chars_in_font = 256;

    enum font_flags
    {
        // used for housekeeping
        FONT_IN_USE = 1,
        // if font file has been read
        FONT_LOADED = 2,
        // if font is virtual
        FONT_VIRTUAL = 4,
        // if kpathsea has already tried to find the font name
        FONT_KPSE_NAME = 8
    };

    enum font_type
    {
        TEX_PK,
        TEX_VIRTUAL,
        TEX_FONTMETRIC,
        FREETYPE
    };

    TeXFontDefinition(const QString& nfontname,
                      double _displayResolution_in_dpi, quint32 chk,
                      qint32 _scaled_size_in_DVI_units, class fontPool* pool,
                      double _enlargement);
    ~TeXFontDefinition();

    TeXFontDefinition(const TeXFontDefinition&) = delete;
    TeXFontDefinition& operator=(const TeXFontDefinition&) = delete;

    void reset();
    void fontNameReceiver(const QString&);

    // Members for character fonts
    void setDisplayResolution(double _displayResolution_in_dpi);

    bool isLocated() const
    {
        return ((flags & FONT_KPSE_NAME) != 0);
    }

    void markAsLocated()
    {
        flags |= FONT_KPSE_NAME;
    }

    void mark_as_used();
    // Pointer to the pool that contains this font.
    class fontPool* font_pool;
    // name of font, such as "cmr10"
    QString fontname;
    // flags byte (see values below)
    unsigned char flags;
    double enlargement;
    // Scaled size from the font definition command; in DVI units
    qint32 scaled_size_in_DVI_units;
    // proc used to set char
    set_char_proc set_char_p;

    // Resolution of the display device (resolution will usually be
    // scaled, according to the zoom)
    double displayResolution_in_dpi;

    // open font file or NULL
    FILE* file;
    // name of font file
    QString filename;

    TeXFont* font;
    // used by (loaded) virtual fonts
    macro* macrotable;
    // used by (loaded) virtual fonts, list of fonts used by this vf,
    QHash<int, TeXFontDefinition*> vf_table;
    // accessible by number
    // used by (loaded) virtual fonts, list of fonts used by this vf
    TeXFontDefinition* first_font;

#ifdef HAVE_FREETYPE
    const QString& getFullFontName() const
    {
        return fullFontName;
    }

    const QString& getFullEncodingName() const
    {
        return fullEncodingName;
    }
#endif

    const font_type& getFontType() const
    {
        return fontType;
    };

#ifdef HAVE_FREETYPE
    /** For FREETYPE fonts, which use a map file, this field will
        contain the full name of the font (e.g. 'Computer Modern'). If
        the name does not exist, or cannot be found, this field will be
        QString(). Only subclasses of TeXFont should write into this
        field. */
    QString fullFontName;

    /** For FREETYPE fonts, which use a map file, this field will
        contain the full name of the font encoding (e.g. 'TexBase1'). If
        the encoding name does not exist, or cannot be found, this field
        will be QString(). Only subclasses of TeXFont should write
        into this field. */
    QString fullEncodingName;
#endif

private:
    quint32 checksum;

    font_type fontType;

    // Functions related to virtual fonts
    void read_VF_index();
};

#endif
