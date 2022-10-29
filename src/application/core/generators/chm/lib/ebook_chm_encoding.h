/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef EBOOK_CHM_ENCODING_H
#define EBOOK_CHM_ENCODING_H

#include <QString>

class Ebook_CHM_Encoding
{
public:
    static QString guessByLCID(unsigned short lcid);
};

#endif  // EBOOK_CHM_ENCODING_H
