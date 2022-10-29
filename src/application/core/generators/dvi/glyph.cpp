// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-

/* glyph.cpp
 *
 * part of kdvi, a dvi-previewer for the KDE desktop environment
 *
 * written by Stefan Kebekus, originally based on code by Paul Vojta
 * and a large number of co-authors */

#include "glyph.h"
#include <config.h>
#include "debug_dvi.h"

bitmap::bitmap()
{
    bits = nullptr;
}

bitmap::~bitmap()
{
    delete[] bits;
}

glyph::glyph()
{
#ifdef DEBUG_GLYPH
    qCDebug(OkularDviDebug) << "glyph::glyph()";
#endif

    addr = 0;
    x = 0;
    y = 0;
    dvi_advance_in_units_of_design_size_by_2e20 = 0;
}

glyph::~glyph()
{
}
