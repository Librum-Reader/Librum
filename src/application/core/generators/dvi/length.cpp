// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// units.cpp
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-FileCopyrightText: 2006 Wilfried Huss
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include "debug_dvi.h"
#include "length.h"

class unitOfDistance
{
public:
    float mmPerUnit;
    const char *name;
};

static const unitOfDistance distanceUnitTable[] = {
    // Metric units
    {1.0f, "mm"},
    {1.0f, "millimeter"},
    {10.0f, "cm"},
    {10.0f, "centimeter"},
    {float(100.0 * 10.0), "m"},
    {float(100.0 * 10.0), "meter"},

    // Imperial units
    {25.4f, "in"},
    {25.4f, "inch"},

    // Typographical units
    {float(2540.0 / 7227.0), "pt"}, // TeX points. 7227points = 254cm
    {float(25.4 / 72.0), "bp"},     // big points, 1/72 inch as used in Postscript
    {float(25.4 / 6.0), "pc"},      // pica = 1/6 inch
    {float(25.4 / 6.0), "pica"},
    {float(25.4 * 0.0148), "dd"}, // didot points = 0.0148 inches
    {float(25.4 * 0.0148), "didot"},
    {float(25.4 * 0.178), "cc"}, // cicero points = 0.178 inches
    {float(25.4 * 0.178), "cicero"},

    {0.0f, nullptr},
};

float Length::convertToMM(const QString &distance, bool *ok)
{
    float MMperUnit = 0.0;
    int unitPos = 0; // position of the unit in the string

    // Check for various known units, and store the beginning position
    // of the unit in 'unitPos', so that distance[0..unitPos] will hold
    // the value. Store the number of mm per unit in 'MMperUnit'.
    for (int i = 0; MMperUnit == 0.0 && distanceUnitTable[i].name != nullptr; i++) {
        unitPos = distance.lastIndexOf(QString::fromLocal8Bit(distanceUnitTable[i].name));
        if (unitPos != -1) {
            MMperUnit = distanceUnitTable[i].mmPerUnit;
        }
    }

    // If no unit has been found -> error message, set *ok to false and
    // return 0.0.
    if (MMperUnit == 0.0) {
        qCCritical(OkularDviShellDebug) << "distance::convertToMM: no known unit found in the string '" << distance << "'.";
        if (ok) {
            *ok = false;
        }
        return 0.0;
    }

    QString val = distance.left(unitPos).simplified();
    return MMperUnit * val.toFloat(ok);
}
