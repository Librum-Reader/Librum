// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// TeXFont_TFM.h
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _TEXFONT_TFM_H
#define _TEXFONT_TFM_H

#include "TeXFont.h"

class fix_word
{
public:
    void fromINT32(qint32 val)
    {
        value = val;
    }
    void fromDouble(double val)
    {
        value = (qint32)(val * (1 << 20) + 0.5);
    }
    double toDouble()
    {
        return (double(value)) / (double(1 << 20));
    }

    qint32 value;
};

class TeXFont_TFM : public TeXFont
{
public:
    explicit TeXFont_TFM(TeXFontDefinition *parent);
    ~TeXFont_TFM() override;

    glyph *getGlyph(quint16 character, bool generateCharacterPixmap = false, const QColor &color = Qt::black) override;

private:
    fix_word characterWidth_in_units_of_design_size[256];
    fix_word characterHeight_in_units_of_design_size[256];

    fix_word design_size_in_TeX_points;
};

#endif
