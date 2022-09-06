/*
    SPDX-FileCopyrightText: 2005 Piotr Szymanski <niedakh@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textpage.h"
#include "textpage_p.h"

#include <QDebug>

#include "area.h"
#include "debug_p.h"
#include "misc.h"
#include "page.h"
#include "page_p.h"

#include <cstring>

#include <QVarLengthArray>
#include <QtAlgorithms>

using namespace Okular;

class SearchPoint
{
public:
    SearchPoint()
        : offset_begin(-1)
        , offset_end(-1)
    {
    }

    /** The TinyTextEntity containing the first character of the match. */
    TextList::ConstIterator it_begin;

    /** The TinyTextEntity containing the last character of the match. */
    TextList::ConstIterator it_end;

    /** The index of the first character of the match in (*it_begin)->text().
     *  Satisfies 0 <= offset_begin < (*it_begin)->text().length().
     */
    int offset_begin;

    /** One plus the index of the last character of the match in (*it_end)->text().
     *  Satisfies 0 < offset_end <= (*it_end)->text().length().
     */
    int offset_end;
};

/* text comparison functions */

static bool CaseInsensitiveCmpFn(const QStringRef &from, const QStringRef &to)
{
#ifdef DEBUG_TEXTPAGE
    qDebug(OkularCoreDebug) << from << ":" << to << "(case insensitive)";
#endif
    return from.compare(to, Qt::CaseInsensitive) == 0;
}

static bool CaseSensitiveCmpFn(const QStringRef &from, const QStringRef &to)
{
#ifdef DEBUG_TEXTPAGE
    qDebug(OkularCoreDebug) << from << ":" << to << "(case sensitive)";
#endif
    return from.compare(to, Qt::CaseSensitive) == 0;
}

/**
 * Returns true iff segments [@p left1, @p right1] and [@p left2, @p right2] on the real line
 * overlap within @p threshold percent, i. e. iff the ratio of the length of the
 * intersection of the segments to the length of the shortest of the two input segments
 * is not smaller than the threshold.
 */
static bool segmentsOverlap(double left1, double right1, double left2, double right2, int threshold)
{
    // check if one consumes another fully (speed optimization)

    if (left1 <= left2 && right1 >= right2) {
        return true;
    }

    if (left1 >= left2 && right1 <= right2) {
        return true;
    }

    // check if there is overlap above threshold
    if (right2 >= left1 && right1 >= left2) {
        double overlap = (right2 >= right1) ? right1 - left2 : right2 - left1;

        double length1 = right1 - left1, length2 = right2 - left2;

        return overlap * 100 >= threshold * qMin(length1, length2);
    }

    return false;
}

static bool doesConsumeY(const QRect first, const QRect second, int threshold)
{
    return segmentsOverlap(first.top(), first.bottom(), second.top(), second.bottom(), threshold);
}

static bool doesConsumeY(const NormalizedRect &first, const NormalizedRect &second, int threshold)
{
    return segmentsOverlap(first.top, first.bottom, second.top, second.bottom, threshold);
}

/*
  Rationale behind TinyTextEntity:

  instead of storing directly a QString for the text of an entity,
  we store the UTF-16 data and their length. This way, we save about
  4 int's wrt a QString, and we can create a new string from that
  raw data (that's the only penalty of that).
  Even better, if the string we need to store has at most
  MaxStaticChars characters, then we store those in place of the QChar*
  that would be used (with new[] + free[]) for the data.
 */
class TinyTextEntity
{
    static const int MaxStaticChars = sizeof(void *) / sizeof(QChar);

public:
    TinyTextEntity(const QString &text, const NormalizedRect &rect)
        : area(rect)
    {
        Q_ASSERT_X(!text.isEmpty(), "TinyTextEntity", "empty string");
        Q_ASSERT_X(sizeof(d) == sizeof(void *), "TinyTextEntity", "internal storage is wider than QChar*, fix it!");
        length = text.length();
        switch (length) {
#if QT_POINTER_SIZE >= 8
        case 4:
            d.qc[3] = text.at(3).unicode();
            // fall through
        case 3:
            d.qc[2] = text.at(2).unicode();
#endif
            // fall through
        case 2:
            d.qc[1] = text.at(1).unicode();
            // fall through
        case 1:
            d.qc[0] = text.at(0).unicode();
            break;
        default:
            d.data = new QChar[length];
            std::memcpy(d.data, text.constData(), length * sizeof(QChar));
        }
    }

    ~TinyTextEntity()
    {
        if (length > MaxStaticChars) {
            delete[] d.data;
        }
    }

    inline QString text() const
    {
        return length <= MaxStaticChars ? QString::fromRawData((const QChar *)&d.qc[0], length) : QString::fromRawData(d.data, length);
    }

    inline NormalizedRect transformedArea(const QTransform &matrix) const
    {
        NormalizedRect transformed_area = area;
        transformed_area.transform(matrix);
        return transformed_area;
    }

    NormalizedRect area;

private:
    Q_DISABLE_COPY(TinyTextEntity)

    union {
        QChar *data;
        ushort qc[MaxStaticChars];
    } d;
    int length;
};

TextEntity::TextEntity(const QString &text, NormalizedRect *area)
    : m_text(text)
    , m_area(area)
    , d(nullptr)
{
}

TextEntity::~TextEntity()
{
    delete m_area;
}

QString TextEntity::text() const
{
    return m_text;
}

NormalizedRect *TextEntity::area() const
{
    return m_area;
}

NormalizedRect TextEntity::transformedArea(const QTransform &matrix) const
{
    NormalizedRect transformed_area = *m_area;
    transformed_area.transform(matrix);
    return transformed_area;
}

TextPagePrivate::TextPagePrivate()
    : m_page(nullptr)
{
}

TextPagePrivate::~TextPagePrivate()
{
    qDeleteAll(m_searchPoints);
    qDeleteAll(m_words);
}

TextPage::TextPage()
    : d(new TextPagePrivate())
{
}

TextPage::TextPage(const TextEntity::List &words)
    : d(new TextPagePrivate())
{
    TextEntity::List::ConstIterator it = words.constBegin(), itEnd = words.constEnd();
    for (; it != itEnd; ++it) {
        TextEntity *e = *it;
        if (!e->text().isEmpty()) {
            d->m_words.append(new TinyTextEntity(e->text(), *e->area()));
        }
        delete e;
    }
}

TextPage::~TextPage()
{
    delete d;
}

void TextPage::append(const QString &text, NormalizedRect *area)
{
    if (!text.isEmpty()) {
        if (!d->m_words.isEmpty()) {
            TinyTextEntity *lastEntity = d->m_words.last();
            const QString concatText = lastEntity->text() + text.normalized(QString::NormalizationForm_KC);
            if (concatText != concatText.normalized(QString::NormalizationForm_KC)) {
                // If this happens it means that the new text + old one have combined, for example A and ◌̊  form Å
                NormalizedRect newArea = *area | lastEntity->area;
                delete area;
                delete lastEntity;
                d->m_words.removeLast();
                d->m_words.append(new TinyTextEntity(concatText.normalized(QString::NormalizationForm_KC), newArea));
                return;
            }
        }

        d->m_words.append(new TinyTextEntity(text.normalized(QString::NormalizationForm_KC), *area));
    }
    delete area;
}

struct WordWithCharacters {
    WordWithCharacters(TinyTextEntity *w, const TextList &c)
        : word(w)
        , characters(c)
    {
    }

    inline QString text() const
    {
        return word->text();
    }

    inline const NormalizedRect &area() const
    {
        return word->area;
    }

    TinyTextEntity *word;
    TextList characters;
};
typedef QList<WordWithCharacters> WordsWithCharacters;

/**
 * We will divide the whole page in some regions depending on the horizontal and
 * vertical spacing among different regions. Each region will have an area and an
 * associated WordsWithCharacters in sorted order.
 */
class RegionText
{
public:
    RegionText() {};

    RegionText(const WordsWithCharacters &wordsWithCharacters, const QRect area)
        : m_region_wordWithCharacters(wordsWithCharacters)
        , m_area(area)
    {
    }

    inline QString string() const
    {
        QString res;
        for (const WordWithCharacters &word : m_region_wordWithCharacters) {
            res += word.text();
        }
        return res;
    }

    inline WordsWithCharacters text() const
    {
        return m_region_wordWithCharacters;
    }

    inline QRect area() const
    {
        return m_area;
    }

    inline void setArea(const QRect area)
    {
        m_area = area;
    }

    inline void setText(const WordsWithCharacters &wordsWithCharacters)
    {
        m_region_wordWithCharacters = wordsWithCharacters;
    }

private:
    WordsWithCharacters m_region_wordWithCharacters;
    QRect m_area;
};

RegularAreaRect *TextPage::textArea(TextSelection *sel) const
{
    if (d->m_words.isEmpty()) {
        return new RegularAreaRect();
    }

    /**
        It works like this:
        There are two cursors, we need to select all the text between them. The coordinates are normalised, leftTop is (0,0)
        rightBottom is (1,1), so for cursors start (sx,sy) and end (ex,ey) we start with finding text rectangles under those
        points, if not we search for the first that is to the right to it in the same baseline, if none found, then we search
        for the first rectangle with a baseline under the cursor, having two points that are the best rectangles to both
        of the cursors: (rx,ry)x(tx,ty) for start and (ux,uy)x(vx,vy) for end, we do a
        1. (rx,ry)x(1,ty)
        2. (0,ty)x(1,uy)
        3. (0,uy)x(vx,vy)

        To find the closest rectangle to cursor (cx,cy) we search for a rectangle that either contains the cursor
        or that has a left border >= cx and bottom border >= cy.
    */
    RegularAreaRect *ret = new RegularAreaRect;

    PagePrivate *pagePrivate = PagePrivate::get(d->m_page);
    const QTransform matrix = pagePrivate ? pagePrivate->rotationMatrix() : QTransform();
    const double scaleX = d->m_page->width();
    const double scaleY = d->m_page->height();

    NormalizedPoint startC = sel->start();
    NormalizedPoint endC = sel->end();
    NormalizedPoint temp;

    // if startPoint is right to endPoint swap them
    if (startC.x > endC.x) {
        temp = startC;
        startC = endC;
        endC = temp;
    }

    // minX,maxX,minY,maxY gives the bounding rectangle coordinates of the document
    const NormalizedRect boundingRect = d->m_page->boundingBox();
    const QRect content = boundingRect.geometry(scaleX, scaleY);
    const double minX = content.left();
    const double maxX = content.right();
    const double minY = content.top();
    const double maxY = content.bottom();

    /**
     * We will now find out the TinyTextEntity for the startRectangle and TinyTextEntity for
     * the endRectangle. We have four cases:
     *
     * Case 1(a): both startpoint and endpoint are out of the bounding Rectangle and at one side, so the rectangle made of start
     * and endPoint are outof the bounding rect (do not intersect)
     *
     * Case 1(b): both startpoint and endpoint are out of bounding rect, but they are in different side, so is their rectangle
     *
     * Case 2(a): find the rectangle which contains start and endpoint and having some
     * TextEntity
     *
     * Case 2(b): if 2(a) fails (if startPoint and endPoint both are unchanged), then we check whether there is any
     * TextEntity within the rect made by startPoint and endPoint
     *
     * Case 3: Now, we may have two type of selection.
     * 1. startpoint is left-top of start_end and endpoint is right-bottom
     * 2. startpoint is left-bottom of start_end and endpoint is top-right
     *
     * Also, as 2(b) is passed, we might have it,itEnd or both unchanged, but the fact is that we have
     * text within them. so, we need to search for the best suitable textposition for start and end.
     *
     * Case 3(a): We search the nearest rectangle consisting of some
     * TinyTextEntity right to or bottom of the startPoint for selection 01.
     * And, for selection 02, we have to search for right and top
     *
     * Case 3(b): For endpoint, we have to find the point top of or left to
     * endpoint if we have selection 01.
     * Otherwise, the search will be left and bottom
     */

    // we know that startC.x > endC.x, we need to decide which is top and which is bottom
    const NormalizedRect start_end = (startC.y < endC.y) ? NormalizedRect(startC.x, startC.y, endC.x, endC.y) : NormalizedRect(startC.x, endC.y, endC.x, startC.y);

    // Case 1(a)
    if (!boundingRect.intersects(start_end)) {
        return ret;
    } else {
        // case 1(b)
        /**
            note that, after swapping of start and end, we know that,
            start is always left to end. but, we cannot say start is
            positioned upper than end.
        **/
        // if start is left to content rect take it to content rect boundary
        if (startC.x * scaleX < minX) {
            startC.x = minX / scaleX;
        }
        if (endC.x * scaleX > maxX) {
            endC.x = maxX / scaleX;
        }

        // if start is top to end (selection type 01)
        if (startC.y * scaleY < minY) {
            startC.y = minY / scaleY;
        }
        if (endC.y * scaleY > maxY) {
            endC.y = maxY / scaleY;
        }

        // if start is bottom to end (selection type 02)
        if (startC.y * scaleY > maxY) {
            startC.y = maxY / scaleY;
        }
        if (endC.y * scaleY < minY) {
            endC.y = minY / scaleY;
        }
    }

    TextList::ConstIterator it = d->m_words.constBegin(), itEnd = d->m_words.constEnd();
    TextList::ConstIterator start = it, end = itEnd, tmpIt = it; //, tmpItEnd = itEnd;
    const MergeSide side = d->m_page ? (MergeSide)d->m_page->totalOrientation() : MergeRight;

    NormalizedRect tmp;
    // case 2(a)
    for (; it != itEnd; ++it) {
        tmp = (*it)->area;
        if (tmp.contains(startC.x, startC.y)) {
            start = it;
        }
        if (tmp.contains(endC.x, endC.y)) {
            end = it;
        }
    }

    // case 2(b)
    it = tmpIt;
    if (start == it && end == itEnd) {
        for (; it != itEnd; ++it) {
            // is there any text rectangle within the start_end rect
            tmp = (*it)->area;
            if (start_end.intersects(tmp)) {
                break;
            }
        }

        // we have searched every text entities, but none is within the rectangle created by start and end
        // so, no selection should be done
        if (it == itEnd) {
            return ret;
        }
    }
    it = tmpIt;
    bool selection_two_start = false;

    // case 3.a
    if (start == it) {
        bool flagV = false;
        NormalizedRect rect;

        // selection type 01
        if (startC.y <= endC.y) {
            for (; it != itEnd; ++it) {
                rect = (*it)->area;
                rect.isBottom(startC) ? flagV = false : flagV = true;

                if (flagV && rect.isRight(startC)) {
                    start = it;
                    break;
                }
            }
        }

        // selection type 02
        else {
            selection_two_start = true;
            int distance = scaleX + scaleY + 100;
            int count = 0;

            for (; it != itEnd; ++it) {
                rect = (*it)->area;

                if (rect.isBottomOrLevel(startC) && rect.isRight(startC)) {
                    count++;
                    QRect entRect = rect.geometry(scaleX, scaleY);
                    int xdist, ydist;
                    xdist = entRect.center().x() - startC.x * scaleX;
                    ydist = entRect.center().y() - startC.y * scaleY;

                    // make them positive
                    if (xdist < 0) {
                        xdist = -xdist;
                    }
                    if (ydist < 0) {
                        ydist = -ydist;
                    }

                    if ((xdist + ydist) < distance) {
                        distance = xdist + ydist;
                        start = it;
                    }
                }
            }
        }
    }

    // case 3.b
    if (end == itEnd) {
        it = tmpIt;
        itEnd = itEnd - 1;

        bool flagV = false;
        NormalizedRect rect;

        if (startC.y <= endC.y) {
            for (; itEnd >= it; itEnd--) {
                rect = (*itEnd)->area;
                rect.isTop(endC) ? flagV = false : flagV = true;

                if (flagV && rect.isLeft(endC)) {
                    end = itEnd;
                    break;
                }
            }
        }

        else {
            int distance = scaleX + scaleY + 100;
            for (; itEnd >= it; itEnd--) {
                rect = (*itEnd)->area;

                if (rect.isTopOrLevel(endC) && rect.isLeft(endC)) {
                    QRect entRect = rect.geometry(scaleX, scaleY);
                    int xdist, ydist;
                    xdist = entRect.center().x() - endC.x * scaleX;
                    ydist = entRect.center().y() - endC.y * scaleY;

                    // make them positive
                    if (xdist < 0) {
                        xdist = -xdist;
                    }
                    if (ydist < 0) {
                        ydist = -ydist;
                    }

                    if ((xdist + ydist) < distance) {
                        distance = xdist + ydist;
                        end = itEnd;
                    }
                }
            }
        }
    }

    /* if start and end in selection 02 are in the same column, and we
     start at an empty space we have to remove the selection of last
     character
    */
    if (selection_two_start) {
        if (start > end) {
            start = start - 1;
        }
    }

    // if start is less than end swap them
    if (start > end) {
        it = start;
        start = end;
        end = it;
    }

    // removes the possibility of crash, in case none of 1 to 3 is true
    if (end == d->m_words.constEnd()) {
        end--;
    }

    for (; start <= end; start++) {
        ret->appendShape((*start)->transformedArea(matrix), side);
    }

    return ret;
}

RegularAreaRect *TextPage::findText(int searchID, const QString &query, SearchDirection direct, Qt::CaseSensitivity caseSensitivity, const RegularAreaRect *area)
{
    SearchDirection dir = direct;
    // invalid search request
    if (d->m_words.isEmpty() || query.isEmpty() || (area && area->isNull())) {
        return nullptr;
    }
    TextList::ConstIterator start;
    int start_offset = 0;
    TextList::ConstIterator end;
    const QMap<int, SearchPoint *>::const_iterator sIt = d->m_searchPoints.constFind(searchID);
    if (sIt == d->m_searchPoints.constEnd()) {
        // if no previous run of this search is found, then set it to start
        // from the beginning (respecting the search direction)
        if (dir == NextResult) {
            dir = FromTop;
        } else if (dir == PreviousResult) {
            dir = FromBottom;
        }
    }
    bool forward = true;
    switch (dir) {
    case FromTop:
        start = d->m_words.constBegin();
        start_offset = 0;
        end = d->m_words.constEnd();
        break;
    case FromBottom:
        start = d->m_words.constEnd();
        start_offset = 0;
        end = d->m_words.constBegin();
        forward = false;
        break;
    case NextResult:
        start = (*sIt)->it_end;
        start_offset = (*sIt)->offset_end;
        end = d->m_words.constEnd();
        break;
    case PreviousResult:
        start = (*sIt)->it_begin;
        start_offset = (*sIt)->offset_begin;
        end = d->m_words.constBegin();
        forward = false;
        break;
    };
    RegularAreaRect *ret = nullptr;
    const TextComparisonFunction cmpFn = caseSensitivity == Qt::CaseSensitive ? CaseSensitiveCmpFn : CaseInsensitiveCmpFn;
    if (forward) {
        ret = d->findTextInternalForward(searchID, query, cmpFn, start, start_offset, end);
    } else {
        ret = d->findTextInternalBackward(searchID, query, cmpFn, start, start_offset, end);
    }
    return ret;
}

// hyphenated '-' must be at the end of a word, so hyphenation means
// we have a '-' just followed by a '\n' character
// check if the string contains a '-' character
// if the '-' is the last entry
static int stringLengthAdaptedWithHyphen(const QString &str, const TextList::ConstIterator &it, const TextList::ConstIterator &textListEnd)
{
    const int len = str.length();

    // hyphenated '-' must be at the end of a word, so hyphenation means
    // we have a '-' just followed by a '\n' character
    // check if the string contains a '-' character
    // if the '-' is the last entry
    if (str.endsWith(QLatin1Char('-'))) {
        // validity chek of it + 1
        if ((it + 1) != textListEnd) {
            // 1. if the next character is '\n'
            const QString &lookahedStr = (*(it + 1))->text();
            if (lookahedStr.startsWith(QLatin1Char('\n'))) {
                return len - 1;
            }

            // 2. if the next word is in a different line or not
            const NormalizedRect &hyphenArea = (*it)->area;
            const NormalizedRect &lookaheadArea = (*(it + 1))->area;

            // lookahead to check whether both the '-' rect and next character rect overlap
            if (!doesConsumeY(hyphenArea, lookaheadArea, 70)) {
                return len - 1;
            }
        }
    }
    // else if it is the second last entry - for example in pdf format
    else if (str.endsWith(QLatin1String("-\n"))) {
        return len - 2;
    }

    return len;
}

RegularAreaRect *TextPagePrivate::searchPointToArea(const SearchPoint *sp)
{
    PagePrivate *pagePrivate = PagePrivate::get(m_page);
    const QTransform matrix = pagePrivate ? pagePrivate->rotationMatrix() : QTransform();
    RegularAreaRect *ret = new RegularAreaRect;

    for (TextList::ConstIterator it = sp->it_begin;; it++) {
        const TinyTextEntity *curEntity = *it;
        ret->append(curEntity->transformedArea(matrix));

        if (it == sp->it_end) {
            break;
        }
    }

    ret->simplify();
    return ret;
}

RegularAreaRect *TextPagePrivate::findTextInternalForward(int searchID, const QString &_query, TextComparisonFunction comparer, const TextList::ConstIterator &start, int start_offset, const TextList::ConstIterator &end)
{
    // normalize query search all unicode (including glyphs)
    const QString query = _query.normalized(QString::NormalizationForm_KC);

    // j is the current position in our query
    // queryLeft is the length of the query we have left to match
    int j = 0, queryLeft = query.length();

    TextList::ConstIterator it = start;
    int offset = start_offset;

    TextList::ConstIterator it_begin = TextList::ConstIterator();
    int offset_begin = 0; // dummy initial value to suppress compiler warnings

    while (it != end) {
        const TinyTextEntity *curEntity = *it;
        const QString &str = curEntity->text();
        const int strLen = str.length();
        const int adjustedLen = stringLengthAdaptedWithHyphen(str, it, m_words.constEnd());
        // adjustedLen <= strLen

        if (offset >= strLen) {
            it++;
            offset = 0;
            continue;
        }

        if (it_begin == TextList::ConstIterator()) {
            it_begin = it;
            offset_begin = offset;
        }

        // Let the user write the hyphen or not when searching for text
        int matchedLen = -1;
        for (int matchingLen = strLen; matchingLen >= adjustedLen; matchingLen--) {
            // we have equal (or less than) area of the query left as the length of the current
            // entity
            const int min = qMin(queryLeft, matchingLen - offset);
            if (comparer(str.midRef(offset, min), query.midRef(j, min))) {
                matchedLen = min;
                break;
            }
        }

        if (matchedLen == -1) {
            // we have not matched
            // this means we do not have a complete match
            // we need to get back to query start
            // and continue the search from this place
#ifdef DEBUG_TEXTPAGE
            qCDebug(OkularCoreDebug) << "\tnot matched";
#endif
            j = 0;
            queryLeft = query.length();
            it = it_begin;
            offset = offset_begin + 1;
            it_begin = TextList::ConstIterator();
        } else {
            // we have a match
            // move the current position in the query
            // to the position after the length of this string
            // we matched
            // subtract the length of the current entity from
            // the left length of the query
#ifdef DEBUG_TEXTPAGE
            qCDebug(OkularCoreDebug) << "\tmatched" << matchedLen;
#endif
            j += matchedLen;
            queryLeft -= matchedLen;

            if (queryLeft == 0) {
                // save or update the search point for the current searchID
                QMap<int, SearchPoint *>::iterator sIt = m_searchPoints.find(searchID);
                if (sIt == m_searchPoints.end()) {
                    sIt = m_searchPoints.insert(searchID, new SearchPoint);
                }
                SearchPoint *sp = *sIt;
                sp->it_begin = it_begin;
                sp->it_end = it;
                sp->offset_begin = offset_begin;
                sp->offset_end = offset + matchedLen;
                return searchPointToArea(sp);
            }

            it++;
            offset = 0;
        }
    }
    // end of loop - it means that we've ended the textentities

    const QMap<int, SearchPoint *>::iterator sIt = m_searchPoints.find(searchID);
    if (sIt != m_searchPoints.end()) {
        SearchPoint *sp = *sIt;
        m_searchPoints.erase(sIt);
        delete sp;
    }
    return nullptr;
}

RegularAreaRect *TextPagePrivate::findTextInternalBackward(int searchID, const QString &_query, TextComparisonFunction comparer, const TextList::ConstIterator &start, int start_offset, const TextList::ConstIterator &end)
{
    // normalize query to search all unicode (including glyphs)
    const QString query = _query.normalized(QString::NormalizationForm_KC);

    // j is the current position in our query
    // len is the length of the string in TextEntity
    // queryLeft is the length of the query we have left
    int j = query.length(), queryLeft = query.length();

    TextList::ConstIterator it = start;
    int offset = start_offset;

    TextList::ConstIterator it_begin = TextList::ConstIterator();
    int offset_begin = 0; // dummy initial value to suppress compiler warnings

    while (true) {
        if (offset <= 0) {
            if (it == end) {
                break;
            }
            it--;
        }

        const TinyTextEntity *curEntity = *it;
        const QString &str = curEntity->text();
        const int strLen = str.length();
        const int adjustedLen = stringLengthAdaptedWithHyphen(str, it, m_words.constEnd());
        // adjustedLen <= strLen

        if (offset <= 0) {
            offset = strLen;
        }

        if (it_begin == TextList::ConstIterator()) {
            it_begin = it;
            offset_begin = offset;
        }

        // Let the user write the hyphen or not when searching for text
        int matchedLen = -1;
        // we have equal (or less than) area of the query left as the length of the current
        // entity
        for (int matchingLen = strLen; matchingLen >= adjustedLen; matchingLen--) {
            const int hyphenOffset = (strLen - matchingLen);
            const int min = qMin(queryLeft + hyphenOffset, offset);
            if (comparer(str.midRef(offset - min, min - hyphenOffset), query.midRef(j - min + hyphenOffset, min - hyphenOffset))) {
                matchedLen = min - hyphenOffset;
                break;
            }
        }

        if (matchedLen == -1) {
            // we have not matched
            // this means we do not have a complete match
            // we need to get back to query start
            // and continue the search from this place
#ifdef DEBUG_TEXTPAGE
            qCDebug(OkularCoreDebug) << "\tnot matched";
#endif

            j = query.length();
            queryLeft = query.length();
            it = it_begin;
            offset = offset_begin - 1;
            it_begin = TextList::ConstIterator();
        } else {
            // we have a match
            // move the current position in the query
            // to the position after the length of this string
            // we matched
            // subtract the length of the current entity from
            // the left length of the query
#ifdef DEBUG_TEXTPAGE
            qCDebug(OkularCoreDebug) << "\tmatched";
#endif
            j -= matchedLen;
            queryLeft -= matchedLen;

            if (queryLeft == 0) {
                // save or update the search point for the current searchID
                QMap<int, SearchPoint *>::iterator sIt = m_searchPoints.find(searchID);
                if (sIt == m_searchPoints.end()) {
                    sIt = m_searchPoints.insert(searchID, new SearchPoint);
                }
                SearchPoint *sp = *sIt;
                sp->it_begin = it;
                sp->it_end = it_begin;
                sp->offset_begin = offset - matchedLen;
                sp->offset_end = offset_begin;
                return searchPointToArea(sp);
            }

            offset = 0;
        }
    }
    // end of loop - it means that we've ended the textentities

    const QMap<int, SearchPoint *>::iterator sIt = m_searchPoints.find(searchID);
    if (sIt != m_searchPoints.end()) {
        SearchPoint *sp = *sIt;
        m_searchPoints.erase(sIt);
        delete sp;
    }
    return nullptr;
}

QString TextPage::text(const RegularAreaRect *area) const
{
    return text(area, AnyPixelTextAreaInclusionBehaviour);
}

QString TextPage::text(const RegularAreaRect *area, TextAreaInclusionBehaviour b) const
{
    if (area && area->isNull()) {
        return QString();
    }

    TextList::ConstIterator it = d->m_words.constBegin(), itEnd = d->m_words.constEnd();
    QString ret;
    if (area) {
        for (; it != itEnd; ++it) {
            if (b == AnyPixelTextAreaInclusionBehaviour) {
                if (area->intersects((*it)->area)) {
                    ret += (*it)->text();
                }
            } else {
                NormalizedPoint center = (*it)->area.center();
                if (area->contains(center.x, center.y)) {
                    ret += (*it)->text();
                }
            }
        }
    } else {
        for (; it != itEnd; ++it) {
            ret += (*it)->text();
        }
    }
    return ret;
}

static bool compareTinyTextEntityX(const WordWithCharacters &first, const WordWithCharacters &second)
{
    QRect firstArea = first.area().roundedGeometry(1000, 1000);
    QRect secondArea = second.area().roundedGeometry(1000, 1000);

    return firstArea.left() < secondArea.left();
}

static bool compareTinyTextEntityY(const WordWithCharacters &first, const WordWithCharacters &second)
{
    const QRect firstArea = first.area().roundedGeometry(1000, 1000);
    const QRect secondArea = second.area().roundedGeometry(1000, 1000);

    return firstArea.top() < secondArea.top();
}

/**
 * Sets a new world list. Deleting the contents of the old one
 */
void TextPagePrivate::setWordList(const TextList &list)
{
    qDeleteAll(m_words);
    m_words = list;
}

/**
 * Remove all the spaces in between texts. It will make all the generators
 * same, whether they save spaces(like pdf) or not(like djvu).
 */
static void removeSpace(TextList *words)
{
    TextList::Iterator it = words->begin();
    const QString str(QLatin1Char(' '));

    while (it != words->end()) {
        if ((*it)->text() == str) {
            it = words->erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * We will read the TinyTextEntity from characters and try to create words from there.
 * Note: characters might be already characters for some generators, but we will keep
 * the nomenclature characters for the generator produced data. The resulting
 * WordsWithCharacters memory has to be managed by the caller, both the
 * WordWithCharacters::word and WordWithCharacters::characters contents
 */
static WordsWithCharacters makeWordFromCharacters(const TextList &characters, int pageWidth, int pageHeight)
{
    /**
     * We will traverse characters and try to create words from the TinyTextEntities in it.
     * We will search TinyTextEntity blocks and merge them until we get a
     * space between two consecutive TinyTextEntities. When we get a space
     * we can take it as a end of word. Then we store the word as a TinyTextEntity
     * and keep it in newList.

     * We create a RegionText named regionWord that contains the word and the characters associated with it and
     * a rectangle area of the element in newList.

     */
    WordsWithCharacters wordsWithCharacters;

    TextList::ConstIterator it = characters.begin(), itEnd = characters.end(), tmpIt;
    int newLeft, newRight, newTop, newBottom;
    int index = 0;

    for (; it != itEnd; it++) {
        QString textString = (*it)->text();
        QString newString;
        QRect lineArea = (*it)->area.roundedGeometry(pageWidth, pageHeight), elementArea;
        TextList wordCharacters;
        tmpIt = it;
        int space = 0;

        while (!space) {
            if (!textString.isEmpty()) {
                newString.append(textString);

                // when textString is the start of the word
                if (tmpIt == it) {
                    NormalizedRect newRect(lineArea, pageWidth, pageHeight);
                    wordCharacters.append(new TinyTextEntity(textString.normalized(QString::NormalizationForm_KC), newRect));
                } else {
                    NormalizedRect newRect(elementArea, pageWidth, pageHeight);
                    wordCharacters.append(new TinyTextEntity(textString.normalized(QString::NormalizationForm_KC), newRect));
                }
            }

            ++it;

            /*
             we must have to put this line before the if condition of it==itEnd
             otherwise the last character can be missed
             */
            if (it == itEnd) {
                break;
            }
            elementArea = (*it)->area.roundedGeometry(pageWidth, pageHeight);
            if (!doesConsumeY(elementArea, lineArea, 60)) {
                --it;
                break;
            }

            const int text_y1 = elementArea.top(), text_x1 = elementArea.left(), text_y2 = elementArea.y() + elementArea.height(), text_x2 = elementArea.x() + elementArea.width();
            const int line_y1 = lineArea.top(), line_x1 = lineArea.left(), line_y2 = lineArea.y() + lineArea.height(), line_x2 = lineArea.x() + lineArea.width();

            space = elementArea.left() - lineArea.right();

            if (space != 0) {
                it--;
                break;
            }

            newLeft = text_x1 < line_x1 ? text_x1 : line_x1;
            newRight = line_x2 > text_x2 ? line_x2 : text_x2;
            newTop = text_y1 > line_y1 ? line_y1 : text_y1;
            newBottom = text_y2 > line_y2 ? text_y2 : line_y2;

            lineArea.setLeft(newLeft);
            lineArea.setTop(newTop);
            lineArea.setWidth(newRight - newLeft);
            lineArea.setHeight(newBottom - newTop);

            textString = (*it)->text();
        }

        // if newString is not empty, save it
        if (!newString.isEmpty()) {
            const NormalizedRect newRect(lineArea, pageWidth, pageHeight);
            TinyTextEntity *word = new TinyTextEntity(newString.normalized(QString::NormalizationForm_KC), newRect);
            wordsWithCharacters.append(WordWithCharacters(word, wordCharacters));

            index++;
        }

        if (it == itEnd) {
            break;
        }
    }

    return wordsWithCharacters;
}

/**
 * Create Lines from the words and sort them
 */
QList<QPair<WordsWithCharacters, QRect>> makeAndSortLines(const WordsWithCharacters &wordsTmp, int pageWidth, int pageHeight)
{
    /**
     * We cannot assume that the generator will give us texts in the right order.
     * We can only assume that we will get texts in the page and their bounding
     * rectangle. The texts can be character, word, half-word anything.
     * So, we need to:
     **
     * 1. Sort rectangles/boxes containing texts by y0(top)
     * 2. Create textline where there is y overlap between TinyTextEntity 's
     * 3. Within each line sort the TinyTextEntity 's by x0(left)
     */

    QList<QPair<WordsWithCharacters, QRect>> lines;

    /*
     Make a new copy of the TextList in the words, so that the wordsTmp and lines do
     not contain same pointers for all the TinyTextEntity.
     */
    QList<WordWithCharacters> words = wordsTmp;

    // Step 1
    std::sort(words.begin(), words.end(), compareTinyTextEntityY);

    // Step 2
    QList<WordWithCharacters>::Iterator it = words.begin(), itEnd = words.end();

    // for every non-space texts(characters/words) in the textList
    for (; it != itEnd; it++) {
        const QRect elementArea = (*it).area().roundedGeometry(pageWidth, pageHeight);
        bool found = false;

        for (QPair<WordsWithCharacters, QRect> &linesI : lines) {
            /* the line area which will be expanded
               line_rects is only necessary to preserve the topmin and bottommax of all
               the texts in the line, left and right is not necessary at all
            */
            QRect &lineArea = linesI.second;
            const int text_y1 = elementArea.top(), text_y2 = elementArea.top() + elementArea.height(), text_x1 = elementArea.left(), text_x2 = elementArea.left() + elementArea.width();
            const int line_y1 = lineArea.top(), line_y2 = lineArea.top() + lineArea.height(), line_x1 = lineArea.left(), line_x2 = lineArea.left() + lineArea.width();

            /*
               if the new text and the line has y overlapping parts of more than 70%,
               the text will be added to this line
             */
            if (doesConsumeY(elementArea, lineArea, 70)) {
                WordsWithCharacters &line = linesI.first;
                line.append(*it);

                const int newLeft = line_x1 < text_x1 ? line_x1 : text_x1;
                const int newRight = line_x2 > text_x2 ? line_x2 : text_x2;
                const int newTop = line_y1 < text_y1 ? line_y1 : text_y1;
                const int newBottom = text_y2 > line_y2 ? text_y2 : line_y2;

                lineArea = QRect(newLeft, newTop, newRight - newLeft, newBottom - newTop);
                found = true;
            }

            if (found) {
                break;
            }
        }

        /* when we have found a new line create a new TextList containing
           only one element and append it to the lines
         */
        if (!found) {
            WordsWithCharacters tmp;
            tmp.append((*it));
            lines.append(QPair<WordsWithCharacters, QRect>(tmp, elementArea));
        }
    }

    // Step 3
    for (QPair<WordsWithCharacters, QRect> &line : lines) {
        WordsWithCharacters &list = line.first;
        std::sort(list.begin(), list.end(), compareTinyTextEntityX);
    }

    return lines;
}

/**
 * Calculate Statistical information from the lines we made previously
 */
static void calculateStatisticalInformation(const QList<WordWithCharacters> &words, int pageWidth, int pageHeight, int *word_spacing, int *line_spacing, int *col_spacing)
{
    /**
     * For the region, defined by line_rects and lines
     * 1. Make line statistical analysis to find the line spacing
     * 2. Make character statistical analysis to differentiate between
     *   word spacing and column spacing.
     */

    /**
     * Step 0
     */
    const QList<QPair<WordsWithCharacters, QRect>> sortedLines = makeAndSortLines(words, pageWidth, pageHeight);

    /**
     * Step 1
     */
    QMap<int, int> line_space_stat;
    for (int i = 0; i < sortedLines.length(); i++) {
        const QRect rectUpper = sortedLines.at(i).second;

        if (i + 1 == sortedLines.length()) {
            break;
        }
        const QRect rectLower = sortedLines.at(i + 1).second;

        int linespace = rectLower.top() - (rectUpper.top() + rectUpper.height());
        if (linespace < 0) {
            linespace = -linespace;
        }

        if (line_space_stat.contains(linespace)) {
            line_space_stat[linespace]++;
        } else {
            line_space_stat[linespace] = 1;
        }
    }

    *line_spacing = 0;
    int weighted_count = 0;
    QMapIterator<int, int> iterate_linespace(line_space_stat);

    while (iterate_linespace.hasNext()) {
        iterate_linespace.next();
        *line_spacing += iterate_linespace.value() * iterate_linespace.key();
        weighted_count += iterate_linespace.value();
    }
    if (*line_spacing != 0) {
        *line_spacing = (int)((double)*line_spacing / (double)weighted_count + 0.5);
    }

    /**
     * Step 2
     */
    // We would like to use QMap instead of QHash as it will keep the keys sorted
    QMap<int, int> hor_space_stat;
    QMap<int, int> col_space_stat;
    QList<QList<QRect>> space_rects;
    QVector<QRect> max_hor_space_rects;

    // Space in every line
    for (const QPair<WordsWithCharacters, QRect> &sortedLine : sortedLines) {
        const WordsWithCharacters list = sortedLine.first;
        QList<QRect> line_space_rects;
        int maxSpace = 0, minSpace = pageWidth;

        // for every TinyTextEntity element in the line
        WordsWithCharacters::ConstIterator it = list.begin(), itEnd = list.end();
        QRect max_area1, max_area2;
        QString before_max, after_max;

        // for every line
        for (; it != itEnd; it++) {
            const QRect area1 = (*it).area().roundedGeometry(pageWidth, pageHeight);
            if (it + 1 == itEnd) {
                break;
            }

            const QRect area2 = (*(it + 1)).area().roundedGeometry(pageWidth, pageHeight);
            int space = area2.left() - area1.right();

            if (space > maxSpace) {
                max_area1 = area1;
                max_area2 = area2;
                maxSpace = space;
                before_max = (*it).text();
                after_max = (*(it + 1)).text();
            }

            if (space < minSpace && space != 0) {
                minSpace = space;
            }

            // if we found a real space, whose length is not zero and also less than the pageWidth
            if (space != 0 && space != pageWidth) {
                // increase the count of the space amount
                if (hor_space_stat.contains(space)) {
                    hor_space_stat[space]++;
                } else {
                    hor_space_stat[space] = 1;
                }

                int left, right, top, bottom;

                left = area1.right();
                right = area2.left();

                top = area2.top() < area1.top() ? area2.top() : area1.top();
                bottom = area2.bottom() > area1.bottom() ? area2.bottom() : area1.bottom();

                QRect rect(left, top, right - left, bottom - top);
                line_space_rects.append(rect);
            }
        }

        space_rects.append(line_space_rects);

        if (hor_space_stat.contains(maxSpace)) {
            if (hor_space_stat[maxSpace] != 1) {
                hor_space_stat[maxSpace]--;
            } else {
                hor_space_stat.remove(maxSpace);
            }
        }

        if (maxSpace != 0) {
            if (col_space_stat.contains(maxSpace)) {
                col_space_stat[maxSpace]++;
            } else {
                col_space_stat[maxSpace] = 1;
            }

            // store the max rect of each line
            const int left = max_area1.right();
            const int right = max_area2.left();
            const int top = (max_area1.top() > max_area2.top()) ? max_area2.top() : max_area1.top();
            const int bottom = (max_area1.bottom() < max_area2.bottom()) ? max_area2.bottom() : max_area1.bottom();

            const QRect rect(left, top, right - left, bottom - top);
            max_hor_space_rects.append(rect);
        } else {
            max_hor_space_rects.append(QRect(0, 0, 0, 0));
        }
    }

    // All the between word space counts are in hor_space_stat
    *word_spacing = 0;
    weighted_count = 0;
    QMapIterator<int, int> iterate(hor_space_stat);

    while (iterate.hasNext()) {
        iterate.next();

        if (iterate.key() > 0) {
            *word_spacing += iterate.value() * iterate.key();
            weighted_count += iterate.value();
        }
    }
    if (weighted_count) {
        *word_spacing = (int)((double)*word_spacing / (double)weighted_count + 0.5);
    }

    *col_spacing = 0;
    QMapIterator<int, int> iterate_col(col_space_stat);

    while (iterate_col.hasNext()) {
        iterate_col.next();
        if (iterate_col.value() > *col_spacing) {
            *col_spacing = iterate_col.value();
        }
    }
    *col_spacing = col_space_stat.key(*col_spacing);

    // if there is just one line in a region, there is no point in dividing it
    if (sortedLines.length() == 1) {
        *word_spacing = *col_spacing;
    }
}

/**
 * Implements the XY Cut algorithm for textpage segmentation
 * The resulting RegionTextList will contain RegionText whose WordsWithCharacters::word and
 * WordsWithCharacters::characters are reused from wordsWithCharacters (i.e. no new nor delete happens in this function)
 */
static RegionTextList XYCutForBoundingBoxes(const QList<WordWithCharacters> &wordsWithCharacters, int pageWidth, int pageHeight)
{
    RegionTextList tree;
    QRect contentRect(0, 0, pageWidth, pageHeight);
    const RegionText root(wordsWithCharacters, contentRect);

    // start the tree with the root, it is our only region at the start
    tree.push_back(root);

    int i = 0;

    // while traversing the tree has not been ended
    while (i < tree.length()) {
        const RegionText node = tree.at(i);
        QRect regionRect = node.area();

        /**
         * 1. calculation of projection profiles
         */
        // allocate the size of proj profiles and initialize with 0
        int size_proj_y = node.area().height();
        int size_proj_x = node.area().width();
        // dynamic memory allocation
        QVarLengthArray<int> proj_on_xaxis(size_proj_x);
        QVarLengthArray<int> proj_on_yaxis(size_proj_y);

        for (int j = 0; j < size_proj_y; ++j) {
            proj_on_yaxis[j] = 0;
        }
        for (int j = 0; j < size_proj_x; ++j) {
            proj_on_xaxis[j] = 0;
        }

        const QList<WordWithCharacters> list = node.text();

        // Calculate tcx and tcy locally for each new region
        int word_spacing, line_spacing, column_spacing;
        calculateStatisticalInformation(list, pageWidth, pageHeight, &word_spacing, &line_spacing, &column_spacing);

        const int tcx = word_spacing * 2;
        const int tcy = line_spacing * 2;

        int maxX = 0, maxY = 0;
        int avgX = 0;
        int count;

        // for every text in the region
        for (const WordWithCharacters &wwc : list) {
            TinyTextEntity *ent = wwc.word;
            const QRect entRect = ent->area.geometry(pageWidth, pageHeight);

            // calculate vertical projection profile proj_on_xaxis1
            for (int k = entRect.left(); k <= entRect.left() + entRect.width(); ++k) {
                if ((k - regionRect.left()) < size_proj_x && (k - regionRect.left()) >= 0) {
                    proj_on_xaxis[k - regionRect.left()] += entRect.height();
                }
            }

            // calculate horizontal projection profile in the same way
            for (int k = entRect.top(); k <= entRect.top() + entRect.height(); ++k) {
                if ((k - regionRect.top()) < size_proj_y && (k - regionRect.top()) >= 0) {
                    proj_on_yaxis[k - regionRect.top()] += entRect.width();
                }
            }
        }

        for (int j = 0; j < size_proj_y; ++j) {
            if (proj_on_yaxis[j] > maxY) {
                maxY = proj_on_yaxis[j];
            }
        }

        avgX = count = 0;
        for (int j = 0; j < size_proj_x; ++j) {
            if (proj_on_xaxis[j] > maxX) {
                maxX = proj_on_xaxis[j];
            }
            if (proj_on_xaxis[j]) {
                count++;
                avgX += proj_on_xaxis[j];
            }
        }
        if (count) {
            avgX /= count;
        }

        /**
         * 2. Cleanup Boundary White Spaces and removal of noise
         */
        int xbegin = 0, xend = size_proj_x - 1;
        int ybegin = 0, yend = size_proj_y - 1;
        while (xbegin < size_proj_x && proj_on_xaxis[xbegin] <= 0) {
            xbegin++;
        }
        while (xend >= 0 && proj_on_xaxis[xend] <= 0) {
            xend--;
        }
        while (ybegin < size_proj_y && proj_on_yaxis[ybegin] <= 0) {
            ybegin++;
        }
        while (yend >= 0 && proj_on_yaxis[yend] <= 0) {
            yend--;
        }

        // update the regionRect
        int old_left = regionRect.left(), old_top = regionRect.top();
        regionRect.setLeft(old_left + xbegin);
        regionRect.setRight(old_left + xend);
        regionRect.setTop(old_top + ybegin);
        regionRect.setBottom(old_top + yend);

        int tnx = (int)((double)avgX * 10.0 / 100.0 + 0.5), tny = 0;
        for (int j = 0; j < size_proj_x; ++j) {
            proj_on_xaxis[j] -= tnx;
        }
        for (int j = 0; j < size_proj_y; ++j) {
            proj_on_yaxis[j] -= tny;
        }

        /**
         * 3. Find the Widest gap
         */
        int gap_hor = -1, pos_hor = -1;
        int begin = -1, end = -1;

        // find all hor_gaps and find the maximum between them
        for (int j = 1; j < size_proj_y; ++j) {
            // transition from white to black
            if (begin >= 0 && proj_on_yaxis[j - 1] <= 0 && proj_on_yaxis[j] > 0) {
                end = j;
            }

            // transition from black to white
            if (proj_on_yaxis[j - 1] > 0 && proj_on_yaxis[j] <= 0) {
                begin = j;
            }

            if (begin > 0 && end > 0 && end - begin > gap_hor) {
                gap_hor = end - begin;
                pos_hor = (end + begin) / 2;
                begin = -1;
                end = -1;
            }
        }

        begin = -1, end = -1;
        int gap_ver = -1, pos_ver = -1;

        // find all the ver_gaps and find the maximum between them
        for (int j = 1; j < size_proj_x; ++j) {
            // transition from white to black
            if (begin >= 0 && proj_on_xaxis[j - 1] <= 0 && proj_on_xaxis[j] > 0) {
                end = j;
            }

            // transition from black to white
            if (proj_on_xaxis[j - 1] > 0 && proj_on_xaxis[j] <= 0) {
                begin = j;
            }

            if (begin > 0 && end > 0 && end - begin > gap_ver) {
                gap_ver = end - begin;
                pos_ver = (end + begin) / 2;
                begin = -1;
                end = -1;
            }
        }

        int cut_pos_x = pos_ver, cut_pos_y = pos_hor;
        int gap_x = gap_ver, gap_y = gap_hor;

        /**
         * 4. Cut the region and make nodes (left,right) or (up,down)
         */
        bool cut_hor = false, cut_ver = false;

        // For horizontal cut
        const int topHeight = cut_pos_y - (regionRect.top() - old_top);
        const QRect topRect(regionRect.left(), regionRect.top(), regionRect.width(), topHeight);
        const QRect bottomRect(regionRect.left(), regionRect.top() + topHeight, regionRect.width(), regionRect.height() - topHeight);

        // For vertical Cut
        const int leftWidth = cut_pos_x - (regionRect.left() - old_left);
        const QRect leftRect(regionRect.left(), regionRect.top(), leftWidth, regionRect.height());
        const QRect rightRect(regionRect.left() + leftWidth, regionRect.top(), regionRect.width() - leftWidth, regionRect.height());

        if (gap_y >= gap_x && gap_y >= tcy) {
            cut_hor = true;
        } else if (gap_y >= gap_x && gap_y <= tcy && gap_x >= tcx) {
            cut_ver = true;
        } else if (gap_x >= gap_y && gap_x >= tcx) {
            cut_ver = true;
        } else if (gap_x >= gap_y && gap_x <= tcx && gap_y >= tcy) {
            cut_hor = true;
        } else {
            // no cut possible
            // we can now update the node rectangle with the shrinked rectangle
            RegionText tmpNode = tree.at(i);
            tmpNode.setArea(regionRect);
            tree.replace(i, tmpNode);
            i++;
            continue;
        }

        WordsWithCharacters list1, list2;

        // horizontal cut, topRect and bottomRect
        if (cut_hor) {
            for (const WordWithCharacters &word : list) {
                const QRect wordRect = word.area().geometry(pageWidth, pageHeight);

                if (topRect.intersects(wordRect)) {
                    list1.append(word);
                } else {
                    list2.append(word);
                }
            }

            RegionText node1(list1, topRect);
            RegionText node2(list2, bottomRect);

            tree.replace(i, node1);
            tree.insert(i + 1, node2);
        }

        // vertical cut, leftRect and rightRect
        else if (cut_ver) {
            for (const WordWithCharacters &word : list) {
                const QRect wordRect = word.area().geometry(pageWidth, pageHeight);

                if (leftRect.intersects(wordRect)) {
                    list1.append(word);
                } else {
                    list2.append(word);
                }
            }

            RegionText node1(list1, leftRect);
            RegionText node2(list2, rightRect);

            tree.replace(i, node1);
            tree.insert(i + 1, node2);
        }
    }

    return tree;
}

/**
 * Add spaces in between words in a line. It reuses the pointers passed in tree and might add new ones. You will need to take care of deleting them if needed
 */
WordsWithCharacters addNecessarySpace(RegionTextList tree, int pageWidth, int pageHeight)
{
    /**
     * 1. Call makeAndSortLines before adding spaces in between words in a line
     * 2. Now add spaces between every two words in a line
     * 3. Finally, extract all the space separated texts from each region and return it
     */

    // Only change the texts under RegionTexts, not the area
    for (RegionText &tmpRegion : tree) {
        // Step 01
        QList<QPair<WordsWithCharacters, QRect>> sortedLines = makeAndSortLines(tmpRegion.text(), pageWidth, pageHeight);

        // Step 02
        for (QPair<WordsWithCharacters, QRect> &sortedLine : sortedLines) {
            WordsWithCharacters &list = sortedLine.first;
            for (int k = 0; k < list.length(); k++) {
                const QRect area1 = list.at(k).area().roundedGeometry(pageWidth, pageHeight);
                if (k + 1 >= list.length()) {
                    break;
                }

                const QRect area2 = list.at(k + 1).area().roundedGeometry(pageWidth, pageHeight);
                const int space = area2.left() - area1.right();

                if (space != 0) {
                    // Make a TinyTextEntity of string space and push it between it and it+1
                    const int left = area1.right();
                    const int right = area2.left();
                    const int top = area2.top() < area1.top() ? area2.top() : area1.top();
                    const int bottom = area2.bottom() > area1.bottom() ? area2.bottom() : area1.bottom();

                    const QString spaceStr(QStringLiteral(" "));
                    const QRect rect(QPoint(left, top), QPoint(right, bottom));
                    const NormalizedRect entRect(rect, pageWidth, pageHeight);
                    TinyTextEntity *ent1 = new TinyTextEntity(spaceStr, entRect);
                    TinyTextEntity *ent2 = new TinyTextEntity(spaceStr, entRect);
                    WordWithCharacters word(ent1, QList<TinyTextEntity *>() << ent2);

                    list.insert(k + 1, word);

                    // Skip the space
                    k++;
                }
            }
        }

        WordsWithCharacters tmpList;
        for (const QPair<WordsWithCharacters, QRect> &sortedLine : qAsConst(sortedLines)) {
            tmpList += sortedLine.first;
        }
        tmpRegion.setText(tmpList);
    }

    // Step 03
    WordsWithCharacters tmp;
    for (const RegionText &tmpRegion : qAsConst(tree)) {
        tmp += tmpRegion.text();
    }
    return tmp;
}

/**
 * Correct the textOrder, all layout recognition works here
 */
void TextPagePrivate::correctTextOrder()
{
    // m_page->width() and m_page->height() are in pixels at
    // 100% zoom level, and thus depend on display DPI.
    // To avoid Okular failing on lowDPI displays,
    // we scale pageWidth and pageHeight so their sum equals 2000.
    const double scalingFactor = 2000.0 / (m_page->width() + m_page->height());
    const int pageWidth = (int)(scalingFactor * m_page->width());
    const int pageHeight = (int)(scalingFactor * m_page->height());

    TextList characters = m_words;

    /**
     * Remove spaces from the text
     */
    removeSpace(&characters);

    /**
     * Construct words from characters
     */
    const QList<WordWithCharacters> wordsWithCharacters = makeWordFromCharacters(characters, pageWidth, pageHeight);

    /**
     * Make a XY Cut tree for segmentation of the texts
     */
    const RegionTextList tree = XYCutForBoundingBoxes(wordsWithCharacters, pageWidth, pageHeight);

    /**
     * Add spaces to the word
     */
    const WordsWithCharacters listWithWordsAndSpaces = addNecessarySpace(tree, pageWidth, pageHeight);

    /**
     * Break the words into characters
     */
    TextList listOfCharacters;
    for (const WordWithCharacters &word : listWithWordsAndSpaces) {
        delete word.word;
        listOfCharacters.append(word.characters);
    }
    setWordList(listOfCharacters);
}

TextEntity::List TextPage::words(const RegularAreaRect *area, TextAreaInclusionBehaviour b) const
{
    if (area && area->isNull()) {
        return TextEntity::List();
    }

    TextEntity::List ret;
    if (area) {
        for (const TinyTextEntity *te : qAsConst(d->m_words)) {
            if (b == AnyPixelTextAreaInclusionBehaviour) {
                if (area->intersects(te->area)) {
                    ret.append(new TextEntity(te->text(), new Okular::NormalizedRect(te->area)));
                }
            } else {
                const NormalizedPoint center = te->area.center();
                if (area->contains(center.x, center.y)) {
                    ret.append(new TextEntity(te->text(), new Okular::NormalizedRect(te->area)));
                }
            }
        }
    } else {
        for (const TinyTextEntity *te : qAsConst(d->m_words)) {
            ret.append(new TextEntity(te->text(), new Okular::NormalizedRect(te->area)));
        }
    }
    return ret;
}

RegularAreaRect *TextPage::wordAt(const NormalizedPoint &p, QString *word) const
{
    TextList::ConstIterator itBegin = d->m_words.constBegin(), itEnd = d->m_words.constEnd();
    TextList::ConstIterator it = itBegin;
    TextList::ConstIterator posIt = itEnd;
    for (; it != itEnd; ++it) {
        if ((*it)->area.contains(p.x, p.y)) {
            posIt = it;
            break;
        }
    }
    QString text;
    if (posIt != itEnd) {
        if ((*posIt)->text().simplified().isEmpty()) {
            return nullptr;
        }
        // Find the first TinyTextEntity of the word
        while (posIt != itBegin) {
            --posIt;
            const QString itText = (*posIt)->text();
            if (itText.right(1).at(0).isSpace()) {
                if (itText.endsWith(QLatin1String("-\n"))) {
                    // Is an hyphenated word
                    // continue searching the start of the word back
                    continue;
                }

                if (itText == QLatin1String("\n") && posIt != itBegin) {
                    --posIt;
                    if ((*posIt)->text().endsWith(QLatin1String("-"))) {
                        // Is an hyphenated word
                        // continue searching the start of the word back
                        continue;
                    }
                    ++posIt;
                }

                ++posIt;
                break;
            }
        }
        RegularAreaRect *ret = new RegularAreaRect();
        for (; posIt != itEnd; ++posIt) {
            const QString itText = (*posIt)->text();
            if (itText.simplified().isEmpty()) {
                break;
            }

            ret->appendShape((*posIt)->area);
            text += (*posIt)->text();
            if (itText.right(1).at(0).isSpace()) {
                if (!text.endsWith(QLatin1String("-\n"))) {
                    break;
                }
            }
        }

        if (word) {
            *word = text;
        }
        return ret;
    } else {
        return nullptr;
    }
}
