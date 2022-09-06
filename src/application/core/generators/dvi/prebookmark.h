// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
/*
    SPDX-FileCopyrightText: 2005 Stefan Kebekus <kebekus@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _PREBOOKMARK_H_
#define _PREBOOKMARK_H_

/*! \brief Bookmark representation

This class presents a bookmark in a format that is used internally by
the DVI prescan routines.
*/

class PreBookmark
{
public:
    PreBookmark()
    {
        title.clear();
        anchorName.clear();
        noOfChildren = 0;
    }
    PreBookmark(const QString &t, const QString &a, quint16 n)
    {
        title = t;
        anchorName = a;
        noOfChildren = n;
    }

    // Title of the bookmark
    QString title;

    // Name of the anchor
    QString anchorName;

    // Number of subordinate bookmarks
    quint16 noOfChildren;
};

#endif
