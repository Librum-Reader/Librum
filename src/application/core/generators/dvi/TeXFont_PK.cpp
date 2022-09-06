// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
/*
 * SPDX-FileCopyrightText: 1994 Paul Vojta. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * NOTE:
 *        xdvi is based on prior work as noted in the modification history, below.
 */

/*
 * DVI previewer for X.
 *
 * Eric Cooper, CMU, September 1985.
 *
 * Code derived from dvi-imagen.c.
 *
 * Modification history:
 * 1/1986        Modified for X.10        --Bob Scheifler, MIT LCS.
 * 7/1988        Modified for X.11        --Mark Eichin, MIT
 * 12/1988        Added 'R' option, toolkit, magnifying glass
 *                                        --Paul Vojta, UC Berkeley.
 * 2/1989        Added tpic support       --Jeffrey Lee, U of Toronto
 * 4/1989        Modified for System V    --Donald Richardson, Clarkson Univ.
 * 3/1990        Added VMS support        --Scott Allendorf, U of Iowa
 * 7/1990        Added reflection mode    --Michael Pak, Hebrew U of Jerusalem
 * 1/1992        Added greyscale code     --Till Brychcy, Techn. Univ. Muenchen
 *                                          and Lee Hetherington, MIT
 * 4/1994        Added DPS support, bounding box
 *                                        --Ricardo Telichevesky
 *                                          and Luis Miguel Silveira, MIT RLE.
 */

#include <config.h>

#include "TeXFont_PK.h"
#include "debug_dvi.h"
#include "fontpool.h"
#include "xdvi.h"

#include <KLocalizedString>

#include <QFile>
#include <QImage>

#include <cmath>
#include <math.h>

//#define DEBUG_PK

#define PK_PRE 247
#define PK_ID 89
#define PK_MAGIC (PK_PRE << 8) + PK_ID

extern void oops(const QString &message);

TeXFont_PK::TeXFont_PK(TeXFontDefinition *parent)
    : TeXFont(parent)
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::TeXFont_PK( parent=" << parent << ")";
#endif

    for (auto &characterBitmap : characterBitmaps) {
        characterBitmap = nullptr;
    }
    file = fopen(QFile::encodeName(parent->filename).constData(), "r");
    if (file == nullptr) {
        qCCritical(OkularDviDebug) << i18n("Cannot open font file %1.", parent->filename);
    }
#ifdef DEBUG_PK
    else
        qCDebug(OkularDviDebug) << "TeXFont_PK::TeXFont_PK(): file opened successfully";
#endif

    read_PK_index();

#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::TeXFont_PK() ended";
#endif
}

TeXFont_PK::~TeXFont_PK()
{
    //@@@ Release bitmaps
    for (bitmap *characterBitmap : characterBitmaps) {
        delete characterBitmap;
    }

    if (file != nullptr) {
        fclose(file);
        file = nullptr;
    }
}

glyph *TeXFont_PK::getGlyph(quint16 ch, bool generateCharacterPixmap, const QColor &color)
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::getGlyph( ch=" << ch << ", generateCharacterPixmap=" << generateCharacterPixmap << " )";
#endif

    // Paranoia checks
    if (ch >= TeXFontDefinition::max_num_of_chars_in_font) {
        qCCritical(OkularDviDebug) << "TeXFont_PK::getGlyph(): Argument is too big.";
        return glyphtable;
    }

    // This is the address of the glyph that will be returned.
    class glyph *g = glyphtable + ch;

    // Check if the glyph is loaded. If not, load it now.
    if (characterBitmaps[ch] == nullptr) {
        // If the character is not defined in the PK file, mark the
        // character as missing, and print an error message
        if (g->addr == 0) {
            qCCritical(OkularDviDebug) << i18n("TexFont_PK::operator[]: Character %1 not defined in font %2", ch, parent->filename);
            g->addr = -1;
            return g;
        }

        // If the character has already been marked as missing, just
        // return a pointer to the glyph (which will then be empty)
        if (g->addr == -1) {
            return g;
        }

        // Otherwise, try to load the character
        fseek(file, g->addr, 0);
        read_PK_char(ch);
        // Check if the character could be loaded. If not, mark the
        // character as 'missing', and return a pointer.
        if (characterBitmaps[ch]->bits == nullptr) {
            g->addr = -1;
            return g;
        }
    }

    // At this point, g points to a properly loaded character. Generate
    // a smoothly scaled QPixmap if the user asks for it.
    if ((generateCharacterPixmap == true) && ((g->shrunkenCharacter.isNull()) || (color != g->color)) && (characterBitmaps[ch]->w != 0)) {
        g->color = color;
        double shrinkFactor = 1200 / parent->displayResolution_in_dpi;

        // All is fine? Then we rescale the bitmap in order to produce the
        // required pixmap.  Rescaling a character, however, is an art
        // that requires some explanation...
        //
        // If we would just divide the size of the character and the
        // coordinates by the shrink factor, then the result would look
        // quite ugly: due to the inevitable rounding errors in the
        // integer arithmetic, the characters would be displaced by up to
        // a pixel. That doesn't sound much, but on low-resolution
        // devices, such as a notebook screen, the effect would be a
        // "dancing line" of characters, which looks really bad.

        // Calculate the coordinates of the hot point in the shrunken
        // bitmap. For simplicity, let us consider the x-coordinate
        // first. In principle, the hot point should have an x-coordinate
        // of (g->x/shrinkFactor). That, however, will generally NOT be an
        // integral number. The cure is to translate the source image
        // somewhat, so that the x-coordinate of the hot point falls onto
        // the round-up of this number, i.e.
        g->x2 = (int)ceil(g->x / shrinkFactor);

        // Translating and scaling then means that the pixel in the scaled
        // image which covers the range [x,x+1) corresponds to the range
        // [x*shrinkFactor+srcXTrans, (x+1)*shrinkFactor+srcXTrans), where
        // srcXTrans is the following NEGATIVE number
        double srcXTrans = shrinkFactor * (g->x / shrinkFactor - ceil(g->x / shrinkFactor));

        // How big will the shrunken bitmap then become? If shrunk_width
        // denotes that width of the scaled image, and
        // characterBitmaps[ch]->w the width of the original image, we
        // need to make sure that the following inequality holds:
        //
        // shrunk_width*shrinkFactor+srcXTrans >= characterBitmaps[ch]->w
        //
        // in other words,
        int shrunk_width = (int)ceil((characterBitmaps[ch]->w - srcXTrans) / shrinkFactor);

        // Now do the same for the y-coordinate
        g->y2 = (int)ceil(g->y / shrinkFactor);
        double srcYTrans = shrinkFactor * (g->y / shrinkFactor - ceil(g->y / shrinkFactor));
        int shrunk_height = (int)ceil((characterBitmaps[ch]->h - srcYTrans) / shrinkFactor);

        // Turn the image into 8 bit
        QByteArray translated(characterBitmaps[ch]->w * characterBitmaps[ch]->h, '\0');
        quint8 *data = (quint8 *)translated.data();
        for (int x = 0; x < characterBitmaps[ch]->w; x++) {
            for (int y = 0; y < characterBitmaps[ch]->h; y++) {
                quint8 bit = *(characterBitmaps[ch]->bits + characterBitmaps[ch]->bytes_wide * y + (x >> 3));
                bit = bit >> (x & 7);
                bit = bit & 1;
                data[characterBitmaps[ch]->w * y + x] = bit;
            }
        }

        // Now shrink the image. We shrink the X-direction first
        QByteArray xshrunk(shrunk_width * characterBitmaps[ch]->h, '\0');
        quint8 *xdata = (quint8 *)xshrunk.data();

        // Do the shrinking. The pixel (x,y) that we want to calculate
        // corresponds to the line segment from
        //
        // [shrinkFactor*x+srcXTrans, shrinkFactor*(x+1)+srcXTrans)
        //
        // The trouble is, these numbers are in general no integers.

        for (int y = 0; y < characterBitmaps[ch]->h; y++) {
            for (int x = 0; x < shrunk_width; x++) {
                quint32 value = 0;
                double destStartX = shrinkFactor * x + srcXTrans;
                double destEndX = shrinkFactor * (x + 1) + srcXTrans;
                for (int srcX = (int)ceil(destStartX); srcX < floor(destEndX); srcX++) {
                    if ((srcX >= 0) && (srcX < characterBitmaps[ch]->w)) {
                        value += data[characterBitmaps[ch]->w * y + srcX] * 255;
                    }
                }

                if (destStartX >= 0.0) {
                    value += (quint32)(255.0 * (ceil(destStartX) - destStartX) * data[characterBitmaps[ch]->w * y + (int)floor(destStartX)]);
                }
                if (floor(destEndX) < characterBitmaps[ch]->w) {
                    value += (quint32)(255.0 * (destEndX - floor(destEndX)) * data[characterBitmaps[ch]->w * y + (int)floor(destEndX)]);
                }

                xdata[shrunk_width * y + x] = (int)(value / shrinkFactor + 0.5);
            }
        }

        // Now shrink the Y-direction
        QByteArray xyshrunk(shrunk_width * shrunk_height, '\0');
        quint8 *xydata = (quint8 *)xyshrunk.data();
        for (int x = 0; x < shrunk_width; x++) {
            for (int y = 0; y < shrunk_height; y++) {
                quint32 value = 0;
                double destStartY = shrinkFactor * y + srcYTrans;
                double destEndY = shrinkFactor * (y + 1) + srcYTrans;
                for (int srcY = (int)ceil(destStartY); srcY < floor(destEndY); srcY++) {
                    if ((srcY >= 0) && (srcY < characterBitmaps[ch]->h)) {
                        value += xdata[shrunk_width * srcY + x];
                    }
                }

                if (destStartY >= 0.0) {
                    value += (quint32)((ceil(destStartY) - destStartY) * xdata[shrunk_width * (int)floor(destStartY) + x]);
                }
                if (floor(destEndY) < characterBitmaps[ch]->h) {
                    value += (quint32)((destEndY - floor(destEndY)) * xdata[shrunk_width * (int)floor(destEndY) + x]);
                }

                xydata[shrunk_width * y + x] = (int)(value / shrinkFactor);
            }
        }

        QImage im32(shrunk_width, shrunk_height, QImage::Format_ARGB32);
        // Do QPixmaps fully support the alpha channel? If yes, we use
        // that. Otherwise, use other routines as a fallback
        if (parent->font_pool->QPixmapSupportsAlpha) {
            // If the alpha channel is properly supported, we set the
            // character glyph to a colored rectangle, and define the
            // character outline only using the alpha channel. That ensures
            // good quality rendering for overlapping characters.
            im32.fill(qRgb(color.red(), color.green(), color.blue()));
            for (int y = 0; y < shrunk_height; y++) {
                quint8 *destScanLine = (quint8 *)im32.scanLine(y);
                for (int col = 0; col < shrunk_width; col++) {
                    destScanLine[4 * col + 3] = xydata[shrunk_width * y + col];
                }
            }
        } else {
            // If the alpha channel is not supported... QT seems to turn the
            // alpha channel into a crude bitmap which is used to mask the
            // resulting QPixmap. In this case, we define the character
            // outline using the image data, and use the alpha channel only
            // to store "maximally opaque" or "completely transparent"
            // values. When characters are rendered, overlapping characters
            // are no longer correctly drawn, but quality is still
            // sufficient for most purposes. One notable exception is output
            // from the gftodvi program, which will be partially unreadable.
            quint16 rInv = 0xFF - color.red();
            quint16 gInv = 0xFF - color.green();
            quint16 bInv = 0xFF - color.blue();

            quint8 *srcScanLine = xydata;
            for (int y = 0; y < shrunk_height; y++) {
                unsigned int *destScanLine = reinterpret_cast<unsigned int *>(im32.scanLine(y));
                for (int col = 0; col < shrunk_width; col++) {
                    quint16 data = *srcScanLine;
                    // The value stored in "data" now has the following meaning:
                    // data = 0 -> white; data = 0xff -> use "color"
                    *destScanLine = qRgba(0xFF - (rInv * data + 0x7F) / 0xFF, 0xFF - (gInv * data + 0x7F) / 0xFF, 0xFF - (bInv * data + 0x7F) / 0xFF, (data > 0x03) ? 0xff : 0x00);
                    destScanLine++;
                    srcScanLine++;
                }
            }
        }

        g->shrunkenCharacter = im32;
    }
    return g;
}

#define ADD(a, b) (reinterpret_cast<quint32 *>(((char *)a) + b))
#define SUB(a, b) (reinterpret_cast<quint32 *>(((char *)a) - b))

// This table is used for changing the bit order in a byte. The
// expression bitflp[byte] takes a byte in big endian and gives the
// little endian equivalent of that.
static const uchar bitflip[256] = {0,   128, 64,  192, 32,  160, 96,  224, 16,  144, 80,  208, 48,  176, 112, 240, 8,   136, 72,  200, 40,  168, 104, 232, 24,  152, 88,  216, 56,  184, 120, 248, 4,   132, 68,  196, 36,
                                   164, 100, 228, 20,  148, 84,  212, 52,  180, 116, 244, 12,  140, 76,  204, 44,  172, 108, 236, 28,  156, 92,  220, 60,  188, 124, 252, 2,   130, 66,  194, 34,  162, 98,  226, 18,  146,
                                   82,  210, 50,  178, 114, 242, 10,  138, 74,  202, 42,  170, 106, 234, 26,  154, 90,  218, 58,  186, 122, 250, 6,   134, 70,  198, 38,  166, 102, 230, 22,  150, 86,  214, 54,  182, 118,
                                   246, 14,  142, 78,  206, 46,  174, 110, 238, 30,  158, 94,  222, 62,  190, 126, 254, 1,   129, 65,  193, 33,  161, 97,  225, 17,  145, 81,  209, 49,  177, 113, 241, 9,   137, 73,  201,
                                   41,  169, 105, 233, 25,  153, 89,  217, 57,  185, 121, 249, 5,   133, 69,  197, 37,  165, 101, 229, 21,  149, 85,  213, 53,  181, 117, 245, 13,  141, 77,  205, 45,  173, 109, 237, 29,
                                   157, 93,  221, 61,  189, 125, 253, 3,   131, 67,  195, 35,  163, 99,  227, 19,  147, 83,  211, 51,  179, 115, 243, 11,  139, 75,  203, 43,  171, 107, 235, 27,  155, 91,  219, 59,  187,
                                   123, 251, 7,   135, 71,  199, 39,  167, 103, 231, 23,  151, 87,  215, 55,  183, 119, 247, 15,  143, 79,  207, 47,  175, 111, 239, 31,  159, 95,  223, 63,  191, 127, 255};

static const quint32 bit_masks[33] = {0x0,     0x1,     0x3,     0x7,     0xf,      0x1f,     0x3f,     0x7f,     0xff,      0x1ff,     0x3ff,     0x7ff,     0xfff,      0x1fff,     0x3fff,     0x7fff,    0xffff,
                                      0x1ffff, 0x3ffff, 0x7ffff, 0xfffff, 0x1fffff, 0x3fffff, 0x7fffff, 0xffffff, 0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff};

#define PK_ID 89
#define PK_CMD_START 240
#define PK_X1 240
#define PK_X2 241
#define PK_X3 242
#define PK_X4 243
#define PK_Y 244
#define PK_POST 245
#define PK_NOOP 246
#define PK_PRE 247

int TeXFont_PK::PK_get_nyb(FILE *fp)
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "PK_get_nyb";
#endif

    unsigned temp;
    if (PK_bitpos < 0) {
        PK_input_byte = one(fp);
        PK_bitpos = 4;
    }
    temp = PK_input_byte >> PK_bitpos;
    PK_bitpos -= 4;
    return (temp & 0xf);
}

int TeXFont_PK::PK_packed_num(FILE *fp)
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "PK_packed_num";
#endif

    int i, j;

    if ((i = PK_get_nyb(fp)) == 0) {
        do {
            j = PK_get_nyb(fp);
            ++i;
        } while (j == 0);
        while (i > 0) {
            j = (j << 4) | PK_get_nyb(fp);
            --i;
        }
        return (j - 15 + ((13 - PK_dyn_f) << 4) + PK_dyn_f);
    } else {
        if (i <= PK_dyn_f) {
            return i;
        }
        if (i < 14) {
            return (((i - PK_dyn_f - 1) << 4) + PK_get_nyb(fp) + PK_dyn_f + 1);
        }
        if (i == 14) {
            PK_repeat_count = PK_packed_num(fp);
        } else {
            PK_repeat_count = 1;
        }
        return PK_packed_num(fp);
    }
}

void TeXFont_PK::PK_skip_specials()
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::PK_skip_specials() called";
#endif

    int i, j;
    FILE *fp = file;

#ifdef DEBUG_PK
    if (fp == 0)
        qCDebug(OkularDviDebug) << "TeXFont_PK::PK_skip_specials(): file == 0";
#endif

    do {
        PK_flag_byte = one(fp);
        if (PK_flag_byte >= PK_CMD_START) {
            switch (PK_flag_byte) {
            case PK_X1:
            case PK_X2:
            case PK_X3:
            case PK_X4:
                i = 0;
                for (j = PK_CMD_START; j <= PK_flag_byte; ++j) {
                    i = (i << 8) | one(fp);
                }
                while (i--) {
                    (void)one(fp);
                }
                break;
            case PK_Y:
                (void)four(fp);
            case PK_POST:
            case PK_NOOP:
                break;
            default:
                oops(i18n("Unexpected %1 in PK file %2", PK_flag_byte, parent->filename));
                break;
            }
        }
    } while (PK_flag_byte != PK_POST && PK_flag_byte >= PK_CMD_START);

#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::PK_skip_specials() ended";
#endif
}

void TeXFont_PK::read_PK_char(unsigned int ch)
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "read_PK_char";
#endif

    int i, j;
    int n;
    int row_bit_pos;
    bool paint_switch;
    quint32 *cp;
    class glyph *g;
    FILE *fp = file;
    long fpwidth;
    quint32 word = 0;
    int word_weight, bytes_wide;
    int rows_left, h_bit, count;

    g = glyphtable + ch;
    PK_flag_byte = g->x2;
    PK_dyn_f = PK_flag_byte >> 4;
    paint_switch = ((PK_flag_byte & 8) != 0);
    PK_flag_byte &= 0x7;
    if (PK_flag_byte == 7) {
        n = 4;
    } else if (PK_flag_byte > 3) {
        n = 2;
    } else {
        n = 1;
    }

#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "loading pk char " << ch << ", char type " << n;
#endif

    if (characterBitmaps[ch] == nullptr) {
        characterBitmaps[ch] = new bitmap();
    }

    /*
     * now read rest of character preamble
     */
    if (n != 4) {
        fpwidth = num(fp, 3);
    } else {
        fpwidth = sfour(fp);
        (void)four(fp); /* horizontal escapement */
    }
    (void)num(fp, n); /* vertical escapement */
    {
        unsigned long w, h;

        w = num(fp, n);
        h = num(fp, n);
        if (w > 0x7fff || h > 0x7fff) {
            oops(i18n("The character %1 is too large in file %2", ch, parent->filename));
        }
        characterBitmaps[ch]->w = w;
        characterBitmaps[ch]->h = h;
    }
    g->x = snum(fp, n);
    g->y = snum(fp, n);

    g->dvi_advance_in_units_of_design_size_by_2e20 = fpwidth;

    {
        /* width must be multiple of 16 bits for raster_op */
        characterBitmaps[ch]->bytes_wide = ROUNDUP((int)characterBitmaps[ch]->w, 32) * 4;
        unsigned int size = characterBitmaps[ch]->bytes_wide * characterBitmaps[ch]->h;
        characterBitmaps[ch]->bits = new char[size != 0 ? size : 1];
    }

    cp = reinterpret_cast<quint32 *>(characterBitmaps[ch]->bits);

    /*
     * read character data into *cp
     */
    bytes_wide = ROUNDUP((int)characterBitmaps[ch]->w, 32) * 4;
    PK_bitpos = -1;

    // The routines which read the character depend on the bit
    // ordering. In principle, the bit order should be detected at
    // compile time and the proper routing chosen. For the moment, as
    // autoconf is somewhat complicated for the author, we prefer a
    // simpler -even if somewhat slower approach and detect the ordering
    // at runtime. That should of course be changed in the future.

    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
        // Routine for big Endian machines. Applies e.g. to Motorola and
        // (Ultra-)Sparc processors.

#ifdef DEBUG_PK
        qCDebug(OkularDviDebug) << "big Endian byte ordering";
#endif

        if (PK_dyn_f == 14) { /* get raster by bits */
            memset(characterBitmaps[ch]->bits, 0, (int)characterBitmaps[ch]->h * bytes_wide);
            for (i = 0; i < (int)characterBitmaps[ch]->h; i++) { /* get all rows */
                cp = ADD(characterBitmaps[ch]->bits, i * bytes_wide);
                row_bit_pos = 32;
                for (j = 0; j < (int)characterBitmaps[ch]->w; j++) { /* get one row */
                    if (--PK_bitpos < 0) {
                        word = one(fp);
                        PK_bitpos = 7;
                    }
                    if (--row_bit_pos < 0) {
                        cp++;
                        row_bit_pos = 32 - 1;
                    }
                    if (word & (1 << PK_bitpos)) {
                        *cp |= 1 << row_bit_pos;
                    }
                }
            }
        } else { /* get packed raster */
            rows_left = characterBitmaps[ch]->h;
            h_bit = characterBitmaps[ch]->w;
            PK_repeat_count = 0;
            word_weight = 32;
            word = 0;
            while (rows_left > 0) {
                count = PK_packed_num(fp);
                while (count > 0) {
                    if (count < word_weight && count < h_bit) {
                        h_bit -= count;
                        word_weight -= count;
                        if (paint_switch) {
                            word |= bit_masks[count] << word_weight;
                        }
                        count = 0;
                    } else if (count >= h_bit && h_bit <= word_weight) {
                        if (paint_switch) {
                            word |= bit_masks[h_bit] << (word_weight - h_bit);
                        }
                        *cp++ = word;
                        /* "output" row(s) */
                        for (i = PK_repeat_count * bytes_wide / 4; i > 0; --i) {
                            *cp = *SUB(cp, bytes_wide);
                            ++cp;
                        }
                        rows_left -= PK_repeat_count + 1;
                        PK_repeat_count = 0;
                        word = 0;
                        word_weight = 32;
                        count -= h_bit;
                        h_bit = characterBitmaps[ch]->w;
                    } else {
                        if (paint_switch) {
                            word |= bit_masks[word_weight];
                        }
                        *cp++ = word;
                        word = 0;
                        count -= word_weight;
                        h_bit -= word_weight;
                        word_weight = 32;
                    }
                }
                paint_switch = 1 - paint_switch;
            }
            if (cp != (reinterpret_cast<quint32 *>(characterBitmaps[ch]->bits + bytes_wide * characterBitmaps[ch]->h))) {
                oops(i18n("Wrong number of bits stored:  char. %1, font %2", ch, parent->filename));
            }
            if (rows_left != 0 || h_bit != characterBitmaps[ch]->w) {
                oops(i18n("Bad pk file (%1), too many bits", parent->filename));
            }
        }

        // The data in the bitmap is now in the processor's bit order,
        // that is, big endian. Since XWindows needs little endian, we
        // need to change the bit order now.
        unsigned char *bitmapData = (unsigned char *)characterBitmaps[ch]->bits;
        unsigned char *endOfData = bitmapData + characterBitmaps[ch]->bytes_wide * characterBitmaps[ch]->h;
        while (bitmapData < endOfData) {
            *bitmapData = bitflip[*bitmapData];
            bitmapData++;
        }

    } else {
        // Routines for small Endian start here. This applies e.g. to
        // Intel and Alpha processors.

#ifdef DEBUG_PK
        qCDebug(OkularDviDebug) << "small Endian byte ordering";
#endif

        if (PK_dyn_f == 14) { /* get raster by bits */
            memset(characterBitmaps[ch]->bits, 0, (int)characterBitmaps[ch]->h * bytes_wide);
            for (i = 0; i < (int)characterBitmaps[ch]->h; i++) { /* get all rows */
                cp = ADD(characterBitmaps[ch]->bits, i * bytes_wide);
                row_bit_pos = -1;
                for (j = 0; j < (int)characterBitmaps[ch]->w; j++) { /* get one row */
                    if (--PK_bitpos < 0) {
                        word = one(fp);
                        PK_bitpos = 7;
                    }
                    if (++row_bit_pos >= 32) {
                        cp++;
                        row_bit_pos = 0;
                    }
                    if (word & (1 << PK_bitpos)) {
                        *cp |= 1 << row_bit_pos;
                    }
                }
            }
        } else { /* get packed raster */
            rows_left = characterBitmaps[ch]->h;
            h_bit = characterBitmaps[ch]->w;
            PK_repeat_count = 0;
            word_weight = 32;
            word = 0;
            while (rows_left > 0) {
                count = PK_packed_num(fp);
                while (count > 0) {
                    if (count < word_weight && count < h_bit) {
                        if (paint_switch) {
                            word |= bit_masks[count] << (32 - word_weight);
                        }
                        h_bit -= count;
                        word_weight -= count;
                        count = 0;
                    } else if (count >= h_bit && h_bit <= word_weight) {
                        if (paint_switch) {
                            word |= bit_masks[h_bit] << (32 - word_weight);
                        }
                        *cp++ = word;
                        /* "output" row(s) */
                        for (i = PK_repeat_count * bytes_wide / 4; i > 0; --i) {
                            *cp = *SUB(cp, bytes_wide);
                            ++cp;
                        }
                        rows_left -= PK_repeat_count + 1;
                        PK_repeat_count = 0;
                        word = 0;
                        word_weight = 32;
                        count -= h_bit;
                        h_bit = characterBitmaps[ch]->w;
                    } else {
                        if (paint_switch) {
                            word |= bit_masks[word_weight] << (32 - word_weight);
                        }
                        *cp++ = word;
                        word = 0;
                        count -= word_weight;
                        h_bit -= word_weight;
                        word_weight = 32;
                    }
                }
                paint_switch = 1 - paint_switch;
            }
            if (cp != (reinterpret_cast<quint32 *>(characterBitmaps[ch]->bits + bytes_wide * characterBitmaps[ch]->h))) {
                oops(i18n("Wrong number of bits stored:  char. %1, font %2", ch, parent->filename));
            }
            if (rows_left != 0 || h_bit != characterBitmaps[ch]->w) {
                oops(i18n("Bad pk file (%1), too many bits", parent->filename));
            }
        }
    } // endif: big or small Endian?
}

void TeXFont_PK::read_PK_index()
{
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::read_PK_index() called";
#endif

    if (file == nullptr) {
        qCCritical(OkularDviDebug) << "TeXFont_PK::read_PK_index(): file == 0";
        return;
    }

    int magic = two(file);
    if (magic != PK_MAGIC) {
        qCCritical(OkularDviDebug) << "TeXFont_PK::read_PK_index(): file is not a PK file";
        return;
    }

    fseek(file, (long)one(file), SEEK_CUR); /* skip comment */
    (void)four(file);                       /* skip design size */

    checksum = four(file);

    int hppp = sfour(file);
    int vppp = sfour(file);
    if (hppp != vppp) {
        qCWarning(OkularDviDebug) << i18n("Font has non-square aspect ratio ") << vppp << ":" << hppp;
    }

    // Read glyph directory (really a whole pass over the file).
    for (;;) {
        int bytes_left, flag_low_bits;
        unsigned int ch;

        PK_skip_specials();
        if (PK_flag_byte == PK_POST) {
            break;
        }
        flag_low_bits = PK_flag_byte & 0x7;
        if (flag_low_bits == 7) {
            bytes_left = four(file);
            ch = four(file);
        } else if (flag_low_bits > 3) {
            bytes_left = ((flag_low_bits - 4) << 16) + two(file);
            ch = one(file);
        } else {
            bytes_left = (flag_low_bits << 8) + one(file);
            ch = one(file);
        }

        glyphtable[ch].addr = ftell(file);
        glyphtable[ch].x2 = PK_flag_byte;
        fseek(file, (long)bytes_left, SEEK_CUR);
#ifdef DEBUG_PK
        qCDebug(OkularDviDebug) << "Scanning pk char " << ch << "at " << glyphtable[ch].addr;
#endif
    }
#ifdef DEBUG_PK
    qCDebug(OkularDviDebug) << "TeXFont_PK::read_PK_index() called";
#endif
}
