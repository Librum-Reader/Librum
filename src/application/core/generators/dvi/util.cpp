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

#include "debug_dvi.h"
#include "xdvi.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <cstdlib>

#include <QLoggingCategory>

/*
 *   General utility routines.
 */

/*
 *   Print error message and quit.
 */

void oops(const QString &message)
{
    qCCritical(OkularDviDebug) << "Fatal Error:" << message;

    KMessageBox::error(nullptr, i18n("Fatal error.\n\n") + message + i18n("\n\n\
This probably means that either you found a bug in Okular,\n\
or that the DVI file, or auxiliary files (such as font files, \n\
or virtual font files) were really badly broken.\n\
Okular will abort after this message. If you believe that you \n\
found a bug, or that Okular should behave better in this situation\n\
please report the problem."));
    exit(1);
}

/*
 * Read size bytes from the FILE fp, constructing them into a
 * signed/unsigned integer.
 */
unsigned long num(FILE *fp, int size)
{
    long x = 0;

    while (size--) {
        x = (x << 8) | one(fp);
    }
    return x;
}

long snum(FILE *fp, int size)
{
    long x;

#ifdef __STDC__
    x = (signed char)getc(fp); // NOLINT(bugprone-signed-char-misuse) This code is decades old, so prefer not to touch it
#else
    x = (unsigned char)getc(fp);
    if (x & 0x80)
        x -= 0x100;
#endif
    while (--size) {
        x = (x << 8) | one(fp);
    }
    return x;
}
