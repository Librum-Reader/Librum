// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// TeXFont_TFM.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include "TeXFont_TFM.h"
#include "debug_dvi.h"

#include <QDataStream>
#include <QFile>
#include <QLoggingCategory>

//#define DEBUG_TFM

TeXFont_TFM::TeXFont_TFM(TeXFontDefinition *parent)
    : TeXFont(parent)
{
#ifdef DEBUG_TFM
    qCDebug(OkularDviDebug) << "TeXFont_TFM::TeXFont_TFM( parent=" << parent << " )";
#endif

    QFile file(parent->filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qCCritical(OkularDviDebug) << "TeXFont_TFM::TeXFont_TFM(): Could not read TFM file";
        return;
    }
    QDataStream stream(&file);

    // Data from the very beginning of the TFM file, as specified in
    // "The DVI Driver Standard, Level 0", section D.2.1
    quint16 lf, lh, bc, ec, nw, nh, nd;
    stream >> lf >> lh >> bc >> ec >> nw >> nh >> nd;
#ifdef DEBUG_TFM
    qCDebug(OkularDviDebug) << "lf= " << lf << "lh= " << lh << "\nbc= " << bc << "\nec= " << ec << "\nnw= " << nw << "\nnh= " << nh << "\nnd= " << nd;
#endif
    if ((bc > ec) || (ec >= TeXFontDefinition::max_num_of_chars_in_font)) {
        qCCritical(OkularDviDebug) << "TeXFont_TFM::TeXFont_TFM( filename=" << parent->filename << " ): The font has an invalid bc and ec entries.";
        file.close();
        return;
    }

    // Data from the HEADER section of the TFM data.
    file.seek(24);
    stream >> checksum >> design_size_in_TeX_points.value;
#ifdef DEBUG_TFM
    qCDebug(OkularDviDebug) << "checksum    = " << checksum << "design_size = " << design_size_in_TeX_points.toDouble() << " TeX Points\n"
                            << "            = " << design_size_in_TeX_points.toDouble() * 254.0 / 7227.0 << " cm";
#endif

    // Width table
    fix_word widthTable_in_units_of_design_size[TeXFontDefinition::max_num_of_chars_in_font];
    for (fix_word &fw : widthTable_in_units_of_design_size) {
        fw.value = 0;
    }

    file.seek(24 + 4 * lh + 4 * (ec - bc));
    for (unsigned int i = 0; i < nw; i++) {
        stream >> widthTable_in_units_of_design_size[i].value;
        // Some characters, which are used as parts of glyphs, have width
        // 0 --the real width is calculated in a lig_kern program and
        // depends on the preceding character. We cannot calculate the
        // real width here and take 0.4 times the design size as an
        // approximation.
        if (widthTable_in_units_of_design_size[i].value == 0) {
            widthTable_in_units_of_design_size[i].fromDouble(0.4);
        }
    }

    // Height table
    fix_word heightTable_in_units_of_design_size[16];
    for (fix_word &fw : heightTable_in_units_of_design_size) {
        fw.value = 0;
    }
    for (unsigned int i = 0; i < nh; i++) {
        stream >> heightTable_in_units_of_design_size[i].value;
    }

    // Char-Info table
    file.seek(24 + 4 * lh);
    for (unsigned int characterCode = bc; characterCode < ec; characterCode++) {
        glyph *g = glyphtable + characterCode;

        quint8 byte;
        stream >> byte;
        if (byte >= nw) {
            qCCritical(OkularDviDebug) << "TeXFont_TFM::TeXFont_TFM( filename=" << parent->filename << " ): The font has an invalid Char-Info table.";
        } else {
            characterWidth_in_units_of_design_size[characterCode] = widthTable_in_units_of_design_size[byte];
            g->dvi_advance_in_units_of_design_size_by_2e20 = widthTable_in_units_of_design_size[byte].value;
        }

        stream >> byte;
        byte = byte >> 4;
        if (byte >= nh) {
            qCCritical(OkularDviDebug) << "TeXFont_TFM::TeXFont_TFM( filename=" << parent->filename << " ): The font has an invalid Char-Info table.";
        } else {
            characterHeight_in_units_of_design_size[characterCode] = heightTable_in_units_of_design_size[byte];
        }

        stream >> byte;
        stream >> byte;
    }
    file.close();
}

TeXFont_TFM::~TeXFont_TFM()
{
}

glyph *TeXFont_TFM::getGlyph(quint16 characterCode, bool generateCharacterPixmap, const QColor &color)
{
#ifdef DEBUG_TFM
    qCDebug(OkularDviDebug) << "TeXFont_TFM::getGlyph( ch=" << ch << ", generateCharacterPixmap=" << generateCharacterPixmap << " )";
#endif

    // Paranoia checks
    if (characterCode >= TeXFontDefinition::max_num_of_chars_in_font) {
        qCCritical(OkularDviDebug) << "TeXFont_TFM::getGlyph(): Argument is too big.";
        return glyphtable;
    }

    // This is the address of the glyph that will be returned.
    class glyph *g = glyphtable + characterCode;

    if ((generateCharacterPixmap == true) && ((g->shrunkenCharacter.isNull()) || (color != g->color))) {
        g->color = color;
        quint16 pixelWidth = (quint16)(parent->displayResolution_in_dpi * design_size_in_TeX_points.toDouble() * characterWidth_in_units_of_design_size[characterCode].toDouble() * 100.0 / 7227.0 + 0.5);
        quint16 pixelHeight = (quint16)(parent->displayResolution_in_dpi * design_size_in_TeX_points.toDouble() * characterHeight_in_units_of_design_size[characterCode].toDouble() * 100.0 / 7227.0 + 0.5);

        // Just make sure that weird TFM files never lead to giant
        // pixmaps that eat all system memory...
        if (pixelWidth > 50) {
            pixelWidth = 50;
        }
        if (pixelHeight > 50) {
            pixelHeight = 50;
        }

        g->shrunkenCharacter = QImage(pixelWidth, pixelHeight, QImage::Format_RGB32);
        g->shrunkenCharacter.fill(color.rgba());
        g->x2 = 0;
        g->y2 = pixelHeight;
    }

    return g;
}
