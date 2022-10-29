/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    Based on code written by Bill Janssen 2002

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMAGE_H
#define IMAGE_H

#include "unpluck.h"

class QImage;

bool TranscribePalmImageToJPEG(unsigned char* image_bytes_in, QImage& image);
bool TranscribeMultiImageRecord(plkr_Document* doc, QImage& image,
                                unsigned char* bytes);

#endif
