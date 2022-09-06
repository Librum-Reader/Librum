/*
    SPDX-FileCopyrightText: 2005 Piotr Szymanski <niedakh@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTPAGE_H_
#define _OKULAR_TEXTPAGE_H_

#include <QList>
#include <QString>

#include "global.h"
#include "okularcore_export.h"

class QTransform;

namespace Okular
{
class NormalizedPoint;
class NormalizedRect;
class Page;
class PagePrivate;
class TextPagePrivate;
class TextSelection;
class RegularAreaRect;

/*! @class TextEntity
 * @short Represents a piece of text on a TextPage, containing its textual representation and its bounding box.
 *
 * To enable searching and text selection, a generator can give information about the textual
 * content of a Page using a TextPage.
 * A TextPage is created using TextEntity objects.
 * A TextEntity can represent a single character/glyph, a word, a line, or even the whole page.
 *
 * Ideally, every single glyph is represented by its own TextEntity.
 * If the textual representation of a graphical glyph contains more than one character,
 * the TextEntity must contain the whole string which represents the glyph.
 *
 * When the Generator has created the TextPage, and it is added to a Page,
 * the text entities are reordered to words, lines, and paragraphs, to optimize search and text selection.
 * This way, the Generator does not need to care about the logical order of lines or paragraphs.
 *
 * @par Text Selection/Highlighting
 * A TextEntity is the smallest piece of text, which the user can select, or which can be highlighted.
 * That is, if the TextEntity represents a word, only the whole word can be selected.
 * It would not be possible to select a single glyph of the word, because its bounding box is not known.
 *
 * @see TextPage, Generator
 */
class OKULARCORE_EXPORT TextEntity
{
public:
    typedef QList<TextEntity *> List;

    /**
     * Creates a new text entity with the given @p text and the
     * given @p area.
     */
    TextEntity(const QString &text, NormalizedRect *area);

    /**
     * Destroys the text entity.
     */
    ~TextEntity();

    /**
     * Returns the text of the text entity.
     */
    QString text() const;

    /**
     * Returns the bounding area of the text entity.
     */
    NormalizedRect *area() const;

    /**
     * Returns the transformed area of the text entity.
     */
    NormalizedRect transformedArea(const QTransform &matrix) const;

private:
    QString m_text;
    NormalizedRect *m_area;

    class Private;
    const Private *d;

    Q_DISABLE_COPY(TextEntity)
};

/**
 * @short Represents the textual information of a Page. Makes search and text selection possible.
 *
 * A Generator with text support should add a TextPage to every Page.
 * For every piece of text, a TextEntity is added, holding the string representation and the bounding box.
 *
 * Ideally, every TextEntity describes only one glyph.
 * A "glyph" is one character in the graphical representation, but the textual representation may consist of multiple characters (like diacritic modifiers).
 *
 * When the TextPage is added to the Page, the TextEntitys are restructured to optimize text selection.
 *
 * @see TextEntity
 */
class OKULARCORE_EXPORT TextPage
{
    /// @cond PRIVATE
    friend class Page;
    friend class PagePrivate;
    /// @endcond

public:
    /**
     * Defines the behaviour of adding characters to text() result
     * @since 0.10 (KDE 4.4)
     */
    enum TextAreaInclusionBehaviour {
        AnyPixelTextAreaInclusionBehaviour,    ///< A character is included into text() result if any pixel of his bounding box is in the given area
        CentralPixelTextAreaInclusionBehaviour ///< A character is included into text() result if the central pixel of his bounding box is in the given area
    };

    /**
     * Creates a new text page.
     */
    TextPage();

    /**
     * Creates a new text page with the given @p words.
     */
    explicit TextPage(const TextEntity::List &words);

    /**
     * Destroys the text page.
     */
    ~TextPage();

    /**
     * Appends the given @p text with the given @p area as new
     * @ref TextEntity to the page.
     */
    void append(const QString &text, NormalizedRect *area);

    /**
     * Returns the bounding rect of the text which matches the following criteria
     * or 0 if the search is not successful.
     *
     * @param searchID An unique id for this search.
     * @param query The search text.
     * @param direction The direction of the search (@ref SearchDirection)
     * @param caseSensitivity If Qt::CaseSensitive, the search is case sensitive; otherwise
     *                        the search is case insensitive.
     * @param area If null the search starts at the beginning of the page, otherwise
     *                 right/below the coordinates of the given rect.
     */
    RegularAreaRect *findText(int searchID, const QString &query, SearchDirection direction, Qt::CaseSensitivity caseSensitivity, const RegularAreaRect *area);

    /**
     * Text extraction function. Looks for text in the given @p area.
     *
     * @return
     * - If @p area points to a valid null area, a null string.
     * - If @p area is nullptr, the whole page text as a single string.
     * - Otherwise, the text which is included by @p area, as a single string.
     * Uses AnyPixelTextAreaInclusionBehaviour
     */
    QString text(const RegularAreaRect *area = nullptr) const;

    /**
     * Text extraction function. Looks for text in the given @p area.
     *
     * @return
     * - If @p area points to a valid null area, a null string.
     * - If @p area is nullptr, the whole page text as a single string.
     * - Otherwise, the text which is included by @p area, as a single string.
     * @since 0.10 (KDE 4.4)
     */
    QString text(const RegularAreaRect *area, TextAreaInclusionBehaviour b) const;

    /**
     * Text entity extraction function. Similar to text() but returns
     * the words including their bounding rectangles. Note that
     * ownership of the contents of the returned list belongs to the
     * caller.
     * @since 0.14 (KDE 4.8)
     */
    TextEntity::List words(const RegularAreaRect *area, TextAreaInclusionBehaviour b) const;

    /**
     * Returns the area and text of the word at the given point
     * Note that ownership of the returned area belongs to the caller.
     * @since 0.15 (KDE 4.9)
     */
    RegularAreaRect *wordAt(const NormalizedPoint &p, QString *word = nullptr) const;

    /**
     * Returns the rectangular area of the given @p selection.
     */
    RegularAreaRect *textArea(TextSelection *selection) const;

private:
    TextPagePrivate *const d;

    Q_DISABLE_COPY(TextPage)
};

}

#endif
