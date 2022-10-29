// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
//
// Extracted from:
// Class: documentRenderer
//
// Abstract Widget for displaying document types
// Needs to be implemented from the actual parts
// using kviewshell
// Part of KViewshell - A generic interface for document viewers.
//
// SPDX-FileCopyrightText: 2004-2005 Wilfried Huss
// SPDX-FileCopyrightText: 2004-2006 Stefan Kebekus.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "dviRenderer.h"

SimplePageSize dviRenderer::sizeOfPage(const PageNumber page)
{
#if !defined(QT_NO_THREAD)
    // Wait for all access to this DocumentRenderer to finish
    // QMutexLocker locker(&mutex);
#endif

    if(!page.isValid())
    {
        return SimplePageSize();
    }
    if(page > totalPages())
    {
        return SimplePageSize();
    }
    const quint16 pageNumber = static_cast<quint16>(page);
    if(pageNumber > pageSizes.size())
    {
        return SimplePageSize();
    }

    return pageSizes[pageNumber - 1];
}

Anchor dviRenderer::findAnchor(const QString& locallink)
{
    QMap<QString, Anchor>::Iterator it = anchorList.find(locallink);
    if(it != anchorList.end())
    {
        return *it;
    }
    else
    {
        return Anchor();
    }
}

PageNumber dviRenderer::totalPages() const
{
    return PageNumber(numPages);
}
