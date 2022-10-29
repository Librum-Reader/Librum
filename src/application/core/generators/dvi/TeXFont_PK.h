// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-

#ifndef _TEXFONT_PK_H
#define _TEXFONT_PK_H

#include "TeXFont.h"

class glyph;

class TeXFont_PK : public TeXFont
{
public:
    explicit TeXFont_PK(TeXFontDefinition* parent);
    ~TeXFont_PK() override;

    glyph* getGlyph(quint16 character, bool generateCharacterPixmap = false,
                    const QColor& color = Qt::black) override;

private:
    // open font file or NULL
    FILE* file;

    struct bitmap*
        characterBitmaps[TeXFontDefinition::max_num_of_chars_in_font];

    // For use by PK-decryption routines. I don't understand what these
    // are good for -- Stefan Kebekus
    int PK_flag_byte;
    unsigned int PK_input_byte;
    int PK_bitpos;
    int PK_dyn_f;
    int PK_repeat_count;

    // PK-internal routines which were taken from xdvi. Again, I do not
    // really know what they are good for -- Stefan Kebekus
    inline void read_PK_char(unsigned int ch);
    inline int PK_get_nyb(FILE* fp);
    inline int PK_packed_num(FILE* fp);
    inline void read_PK_index();
    inline void PK_skip_specials();
};

#endif
