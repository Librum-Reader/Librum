// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
#include <config.h>

#include "TeXFontDefinition.h"
#include "debug_dvi.h"
#include "dviRenderer.h"
#ifdef HAVE_FREETYPE
#include "TeXFont_PFB.h"
#endif
#include "TeXFont_PK.h"
#include "TeXFont_TFM.h"
#include "debug_dvi.h"
#include "xdvi.h"

#include <KLocalizedString>

#include <QFile>
#include <QLoggingCategory>

extern const int MFResolutions[];

#define PK_PRE 247
#define PK_ID 89
#define PK_MAGIC (PK_PRE << 8) + PK_ID
#define GF_PRE 247
#define GF_ID 131
#define GF_MAGIC (GF_PRE << 8) + GF_ID
#define VF_PRE 247
#define VF_ID_BYTE 202
#define VF_MAGIC (VF_PRE << 8) + VF_ID_BYTE

// #define DEBUG_FONT

TeXFontDefinition::TeXFontDefinition(const QString &nfontname, double _displayResolution_in_dpi, quint32 chk, qint32 _scaled_size_in_DVI_units, class fontPool *pool, double _enlargement)
{
#ifdef DEBUG_FONT
    qCDebug(OkularDviDebug) << "TeXFontDefinition::TeXFontDefinition(...); fontname=" << nfontname << ", enlargement=" << _enlargement;
#endif

    enlargement = _enlargement;
    font_pool = pool;
    fontname = nfontname;
    font = nullptr;
    displayResolution_in_dpi = _displayResolution_in_dpi;
    checksum = chk;
    flags = TeXFontDefinition::FONT_IN_USE;
    file = nullptr;
    filename.clear();
    scaled_size_in_DVI_units = _scaled_size_in_DVI_units;

    macrotable = nullptr;

    // By default, this font contains only empty characters. After the
    // font has been loaded, this function pointer will be replaced by
    // another one.
    set_char_p = &dviRenderer::set_empty_char;
}

TeXFontDefinition::~TeXFontDefinition()
{
#ifdef DEBUG_FONT
    qCDebug(OkularDviDebug) << "discarding font " << fontname << " at " << (int)(enlargement * MFResolutions[font_pool->getMetafontMode()] + 0.5) << " dpi";
#endif

    if (font != nullptr) {
        delete font;
        font = nullptr;
    }
    if (macrotable != nullptr) {
        delete[] macrotable;
        macrotable = nullptr;
    }

    if (flags & FONT_LOADED) {
        if (file != nullptr) {
            fclose(file);
            file = nullptr;
        }
        if (flags & FONT_VIRTUAL) {
            vf_table.clear();
        }
    }
}

void TeXFontDefinition::fontNameReceiver(const QString &fname)
{
#ifdef DEBUG_FONT
    qCDebug(OkularDviDebug) << "void TeXFontDefinition::fontNameReceiver( " << fname << " )";
#endif

    flags |= TeXFontDefinition::FONT_LOADED;
    filename = fname;
#ifdef HAVE_FREETYPE
    fullFontName.clear();
    fullEncodingName.clear();
#endif

    file = fopen(QFile::encodeName(filename).constData(), "r");
    // Check if the file could be opened. If not, try to find the file
    // in the DVI file's directory. If that works, modify the filename
    // accordingly and go on.
    if (file == nullptr) {
        QString filename_test(font_pool->getExtraSearchPath() + QLatin1Char('/') + filename);
        file = fopen(QFile::encodeName(filename_test).constData(), "r");
        if (file == nullptr) {
            qCCritical(OkularDviDebug) << i18n("Cannot find font %1, file %2.", fontname, filename);
            return;
        } else {
            filename = filename_test;
        }
    }

    set_char_p = &dviRenderer::set_char;
    int magic = two(file);

    if (fname.endsWith(QLatin1String("pk"))) {
        if (magic == PK_MAGIC) {
            fclose(file);
            file = nullptr;
            font = new TeXFont_PK(this);
            set_char_p = &dviRenderer::set_char;
            if ((checksum != 0) && (checksum != font->checksum)) {
                qCWarning(OkularDviDebug) << i18n("Checksum mismatch for font file %1", filename);
            }
            fontType = TEX_PK;
            return;
        }
    }

    if (fname.endsWith(QLatin1String(".vf"))) {
        if (magic == VF_MAGIC) {
            read_VF_index();
            set_char_p = &dviRenderer::set_vf_char;
            fontType = TEX_VIRTUAL;
            return;
        }
    }

    if (fname.endsWith(QLatin1String(".tfm"))) {
        fclose(file);
        file = nullptr;
        font = new TeXFont_TFM(this);
        set_char_p = &dviRenderer::set_char;
        fontType = TEX_FONTMETRIC;
        return;
    }

    // None of these known types? Then it should be one of the font
    // formats that are handled by the FreeType library
    fclose(file);
    file = nullptr;
#ifdef HAVE_FREETYPE
    // Find the encoding for that font
    const QString &enc = font_pool->fontsByTeXName.findEncoding(fontname);

    if (enc.isEmpty() == false) {
#ifdef DEBUG_FONT
        qCDebug(OkularDviDebug) << "Font " << fontname << " uses encoding " << enc;
#endif
        font = new TeXFont_PFB(this, font_pool->encodingPool.findByName(enc), font_pool->fontsByTeXName.findSlant(fontname));
    } else {
#ifdef DEBUG_FONT
        qCDebug(OkularDviDebug) << "Font " << fontname << " does not have an encoding.";
#endif
        font = new TeXFont_PFB(this);
    }

    set_char_p = &dviRenderer::set_char;
    fontType = FREETYPE;
    return;
#else
    // If we don't have the FreeType library, we should never have
    // reached this point. Complain, and leave this font blank
    qCCritical(OkularDviDebug) << i18n("Cannot recognize format for font file %1", filename);
#endif
}

void TeXFontDefinition::reset()
{
    if (font != nullptr) {
        delete font;
        font = nullptr;
    }

    if (macrotable != nullptr) {
        delete[] macrotable;
        macrotable = nullptr;
    }

    if (flags & FONT_LOADED) {
        if (file != nullptr) {
            fclose(file);
            file = nullptr;
        }
        if (flags & FONT_VIRTUAL) {
            vf_table.clear();
        }
    }

    filename.clear();
    flags = TeXFontDefinition::FONT_IN_USE;
    set_char_p = &dviRenderer::set_empty_char;
}

void TeXFontDefinition::setDisplayResolution(double _displayResolution_in_dpi)
{
    displayResolution_in_dpi = _displayResolution_in_dpi;
    if (font != nullptr) {
        font->setDisplayResolution();
    }
}

/** mark_as_used marks the font, and all the fonts it refers to, as
    used, i.e. their FONT_IN_USE-flag is set. */

void TeXFontDefinition::mark_as_used()
{
#ifdef DEBUG_FONT
    qCDebug(OkularDviDebug) << "TeXFontDefinition::mark_as_used()";
#endif

    if (flags & TeXFontDefinition::FONT_IN_USE) {
        return;
    }

    flags |= TeXFontDefinition::FONT_IN_USE;

    // For virtual fonts, also go through the list of referred fonts
    if (flags & TeXFontDefinition::FONT_VIRTUAL) {
        QHashIterator<int, TeXFontDefinition *> it(vf_table);
        while (it.hasNext()) {
            it.next();
            it.value()->flags |= TeXFontDefinition::FONT_IN_USE;
        }
    }
}

macro::macro()
{
    pos = nullptr;                                   /* address of first byte of macro */
    end = nullptr;                                   /* address of last+1 byte */
    dvi_advance_in_units_of_design_size_by_2e20 = 0; /* DVI units to move reference point */
    free_me = false;
}

macro::~macro()
{
    if ((pos != nullptr) && (free_me == true)) {
        delete[] pos;
    }
}
