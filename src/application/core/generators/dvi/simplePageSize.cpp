// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*- SimplePageSize.cpp
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2002-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include "simplePageSize.h"
#include <config.h>
#include <QLoggingCategory>
#include <QPaintDevice>
#include "debug_dvi.h"

double SimplePageSize::zoomForHeight(quint32 height,
                                     const QPaintDevice& pd) const
{
    if(!isValid())
    {
        qCCritical(OkularDviShellDebug)
            << "SimplePageSize::zoomForHeight() called when paper height was "
               "invalid";
        return 0.1;
    }

    return double(height) / (pd.logicalDpiY() * pageHeight.getLength_in_inch());
}

double SimplePageSize::zoomForWidth(quint32 width, const QPaintDevice& pd) const
{
    if(!isValid())
    {
        qCCritical(OkularDviShellDebug)
            << "SimplePageSize::zoomForWidth() called when paper width was "
               "invalid";
        return 0.1;
    }

    return double(width) / (pd.logicalDpiX() * pageWidth.getLength_in_inch());
}

double SimplePageSize::zoomToFitInto(const SimplePageSize& target) const
{
    if(!isValid() || isSmall() || !target.isValid())
    {
        qCWarning(OkularDviShellDebug) << "SimplePageSize::zoomToFitInto(...) "
                                          "with unsuitable source of target";
        return 1.0;
    }

    double z1 = target.width() / pageWidth;
    double z2 = target.height() / pageHeight;

    return qMin(z1, z2);
}
