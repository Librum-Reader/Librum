/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "ebook.h"
#include "ebook_chm.h"
#include "ebook_epub.h"

EBook::EBook()
{
}

EBook::~EBook()
{
}

EBook *EBook::loadFile(const QString &archiveName)
{
    EBook_CHM *cbook = new EBook_CHM();

    if (cbook->load(archiveName)) {
        return cbook;
    }

    delete cbook;

    EBook_EPUB *ebook = new EBook_EPUB();

    if (ebook->load(archiveName)) {
        return ebook;
    }

    delete ebook;
    return nullptr;
}
