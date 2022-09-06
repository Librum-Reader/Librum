/*
    SPDX-FileCopyrightText: 2006 Tobias Koenig <tokoe@kde.org>
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTPAGE_P_H_
#define _OKULAR_TEXTPAGE_P_H_

#include <QList>
#include <QMap>
#include <QPair>
#include <QTransform>

class SearchPoint;

/**
 * Memory-optimized storage of a TextEntity. Stores a string and its bounding box.
 *
 * When a generator adds a TextEntity to a TextPage, it is internally stored as TinyTextEntity.
 * TinyTextEntity is also internally used to get the geometry of text selections and highlight areas.
 *
 * @see TextEntity
 */
class TinyTextEntity;
class RegionText;

namespace Okular
{
class PagePrivate;
typedef QList<TinyTextEntity *> TextList;

/**
 * Returns whether the two strings match.
 * Satisfies the condition that if two strings match then their lengths are equal.
 */
typedef bool (*TextComparisonFunction)(const QStringRef &from, const QStringRef &to);

/**
 * A list of RegionText. It keeps a bunch of TextList with their bounding rectangles
 */
typedef QList<RegionText> RegionTextList;

class TextPagePrivate
{
public:
    TextPagePrivate();
    ~TextPagePrivate();

    RegularAreaRect *findTextInternalForward(int searchID, const QString &query, TextComparisonFunction comparer, const TextList::ConstIterator &start, int start_offset, const TextList::ConstIterator &end);
    RegularAreaRect *findTextInternalBackward(int searchID, const QString &query, TextComparisonFunction comparer, const TextList::ConstIterator &start, int start_offset, const TextList::ConstIterator &end);

    /**
     * Copy a TextList to m_words, the pointers of list are adopted
     */
    void setWordList(const TextList &list);

    /**
     * Make necessary modifications in the TextList to make the text order correct, so
     * that textselection works fine
     */
    void correctTextOrder();

    // variables those can be accessed directly from TextPage
    TextList m_words;
    QMap<int, SearchPoint *> m_searchPoints;
    Page *m_page;

private:
    RegularAreaRect *searchPointToArea(const SearchPoint *sp);
};

}

#endif
