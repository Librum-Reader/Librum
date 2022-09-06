/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "helperxmlhandler_epubcontainer.h"

bool HelperXmlHandler_EpubContainer::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts)
{
    if (qName == QLatin1String("rootfile")) {
        int idx = atts.index(QLatin1String("full-path"));

        if (idx == -1) {
            return false;
        }

        contentPath = atts.value(idx);
    }

    return true;
}
