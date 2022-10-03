/*
    SPDX-FileCopyrightText: 2004 Enrico Ros <eros.kde@email.it>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PAGE_H_
#define _OKULAR_PAGE_H_

#include "area.h"
#include "global.h"
#include "okularcore_export.h"
#include "textpage.h"

class QPixmap;

class PagePainter;

namespace Okular
{
class Annotation;
class Document;
class DocumentObserver;
class DocumentPrivate;
class FormField;
class PagePrivate;
class PageTransition;
class SourceReference;
class TextSelection;
class Tile;

/**
 * @short Collector for all the data belonging to a page.
 *
 * The Page class contains pixmaps (referenced using observers id as key),
 * a search page (a class used internally for retrieving text), rect classes
 * (that describe links or other active areas in the current page) and more.
 *
 * All coordinates are normalized to the page, so {x,y} are valid in [0,1]
 * range as long as NormalizedRect components.
 *
 * Note: The class takes ownership of all objects.
 */
class OKULARCORE_EXPORT Page
{
public:
    /**
     * An action to be executed when particular events happen.
     */
    enum PageAction {
        Opening, ///< An action to be executed when the page is "opened".
        Closing  ///< An action to be executed when the page is "closed".
    };

    /**
     * Creates a new page.
     *
     * @param pageNumber The number of the page in the document.
     * @param width The width of the page.
     * @param height The height of the page.
     * @param orientation The orientation of the page
     */
    Page(uint pageNumber, double width, double height, Rotation orientation);

    /**
     * Destroys the page.
     */
    ~Page();

    /**
     * Returns the number of the page in the document.
     */
    int number() const;

    /**
     * Returns the orientation of the page as defined by the document.
     */
    Rotation orientation() const;

    /**
     * Returns the rotation of the page as defined by the user.
     */
    Rotation rotation() const;

    /**
     * Returns the total orientation which is the original orientation plus
     * the user defined rotation.
     */
    Rotation totalOrientation() const;

    /**
     * Returns the width of the page.
     */
    double width() const;

    /**
     * Returns the height of the page.
     */
    double height() const;

    /**
     * Returns the ration (height / width) of the page.
     */
    double ratio() const;

    /**
     * Returns the bounding box of the page content in normalized [0,1] coordinates,
     * in terms of the upright orientation (Rotation0).
     * If it has not been computed yet, returns the full page (i.e., (0, 0, 1, 1)).
     * Note that the bounding box may be null if the page is blank.
     *
     * @since 0.7 (KDE 4.1)
     */
    NormalizedRect boundingBox() const;

    /**
     * Returns whether the bounding box of the page has been computed.
     * Note that even if the bounding box is computed, it may be null if the page is blank.
     *
     * @since 0.7 (KDE 4.1)
     */
    bool isBoundingBoxKnown() const;

    /**
     * Sets the bounding box of the page content in normalized [0,1] coordinates,
     * in terms of the upright orientation (Rotation0).
     * (This does not inform the document's observers, call Document::SetPageBoundingBox
     * instead if you want that.)
     *
     * @since 0.7 (KDE 4.1)
     */
    void setBoundingBox(const NormalizedRect &bbox);

    /**
     * Returns whether the page of size @p width x @p height has a @p pixmap
     * in the region given by @p rect for the given @p observer
     * If there is a partially rendered pixmap the answer is false.
     */
    bool hasPixmap(DocumentObserver *observer, int width = -1, int height = -1, const NormalizedRect &rect = NormalizedRect()) const;

    /**
     * Returns whether the page provides a text page (@ref TextPage).
     */
    bool hasTextPage() const;

    /**
     * Returns whether the page has an object rect which includes the point (@p x, @p y)
     * at scale (@p xScale, @p yScale).
     */
    bool hasObjectRect(double x, double y, double xScale, double yScale) const;

    /**
     * Returns whether the page provides highlighting for the observer with the
     * given @p id.
     */
    bool hasHighlights(int id = -1) const;

    /**
     * Returns whether the page provides a transition effect.
     */
    bool hasTransition() const;

    /**
     * Returns whether the page provides annotations.
     */
    bool hasAnnotations() const;

    /**
     * Returns the bounding rect of the text which matches the following criteria
     * or 0 if the search is not successful.
     *
     * @param id An unique id for this search.
     * @param text The search text.
     * @param direction The direction of the search (@ref SearchDirection)
     * @param caseSensitivity If Qt::CaseSensitive, the search is case sensitive; otherwise
     *                        the search is case insensitive.
     * @param lastRect If 0 (default) the search starts at the beginning of the page, otherwise
     *                 right/below the coordinates of the given rect.
     */
    RegularAreaRect *findText(int id, const QString &text, SearchDirection direction, Qt::CaseSensitivity caseSensitivity, const RegularAreaRect *lastRect = nullptr) const;

    /**
     * Returns the page text (or part of it).
     * @see TextPage::text()
     */
    QString text(const RegularAreaRect *area = nullptr) const;

    /**
     * Returns the page text (or part of it).
     * @see TextPage::text()
     * @since 0.10 (KDE 4.4)
     */
    QString text(const RegularAreaRect *area, TextPage::TextAreaInclusionBehaviour b) const;

    /**
     * Returns the page text (or part of it) including the bounding
     * rectangles. Note that ownership of the contents of the returned
     * list belongs to the caller.
     * @see TextPage::words()
     * @since 0.14 (KDE 4.8)
     */
    TextEntity::List words(const RegularAreaRect *area, TextPage::TextAreaInclusionBehaviour b) const;

    /**
     * Returns the area and text of the word at the given point
     * Note that ownership of the returned area belongs to the caller.
     * @see TextPage::wordAt()
     * @since 0.15 (KDE 4.9)
     */
    RegularAreaRect *wordAt(const NormalizedPoint &p, QString *word = nullptr) const;

    /**
     * Returns the rectangular area of the given @p selection.
     */
    RegularAreaRect *textArea(TextSelection *selection) const;

    /**
     * Returns the object rect of the given @p type which is at point (@p x, @p y) at scale (@p xScale, @p yScale).
     */
    const ObjectRect *objectRect(ObjectRect::ObjectType type, double x, double y, double xScale, double yScale) const;

    /**
     * Returns all object rects of the given @p type which are at point (@p x, @p y) at scale (@p xScale, @p yScale).
     * @since 0.16 (KDE 4.10)
     */
    QList<const ObjectRect *> objectRects(ObjectRect::ObjectType type, double x, double y, double xScale, double yScale) const;

    /**
     * Returns the object rect of the given @p type which is nearest to the point (@p x, @p y) at scale (@p xScale, @p yScale).
     *
     * @since 0.8.2 (KDE 4.2.2)
     */
    const ObjectRect *nearestObjectRect(ObjectRect::ObjectType type, double x, double y, double xScale, double yScale, double *distance) const;

    /**
     * Returns the transition effect of the page or 0 if no transition
     * effect is set (see hasTransition()).
     */
    const PageTransition *transition() const;

    /**
     * Returns the list of annotations of the page.
     */
    QList<Annotation *> annotations() const;

    /**
     * Returns the annotation with the given unique name.
     * @since 1.3
     */
    Annotation *annotation(const QString &uniqueName) const;

    /**
     * Returns the @ref Action object which is associated with the given page @p action
     * or 0 if no page action is set.
     */
    const Action *pageAction(PageAction action) const;

    /**
     * Returns the list of FormField of the page.
     */
    QList<FormField *> formFields() const;

    /**
     * Sets the region described by @p rect with @p pixmap for the
     * given @p observer.
     * If @p rect is not set (default) the @p pixmap is set to the entire
     * page.
     */
    void setPixmap(DocumentObserver *observer, QPixmap *pixmap, const NormalizedRect &rect = NormalizedRect());

    /**
     * Sets the @p text page.
     */
    void setTextPage(TextPage *text);

    /**
     * Sets the list of object @p rects of the page.
     */
    void setObjectRects(const QList<ObjectRect *> &rects);

    /**
     * Gets the list of object rects of the page.
     *
     * @since 22.04
     */
    const QList<ObjectRect *> &objectRects() const;

    /**
     * Sets the list of source reference objects @p rects.
     */
    void setSourceReferences(const QList<SourceRefObjectRect *> &rects);

    /**
     * Sets the duration of the page to @p seconds when displayed in presentation mode.
     *
     * Setting a negative number disables the duration.
     */
    void setDuration(double seconds);

    /**
     * Returns the duration in seconds of the page when displayed in presentation mode.
     *
     * A negative number means that no time is set.
     */
    double duration() const;

    /**
     * Sets the labels for the page to @p label .
     */
    void setLabel(const QString &label);

    /**
     * Returns the label of the page, or a null string if not set.
     */
    QString label() const;

    /**
     * Returns the current text selection.
     */
    const RegularAreaRect *textSelection() const;

    /**
     * Returns the color of the current text selection, or an invalid color
     * if no text selection has been set.
     */
    QColor textSelectionColor() const;

    /**
     * Adds a new @p annotation to the page.
     */
    void addAnnotation(Annotation *annotation);

    /**
     * Removes the @p annotation from the page.
     */
    bool removeAnnotation(Annotation *annotation);

    /**
     * Sets the page @p transition effect.
     */
    void setTransition(PageTransition *transition);

    /**
     * Sets the @p link object for the given page @p action.
     */
    void setPageAction(PageAction action, Action *link);

    /**
     * Sets @p fields as list of FormField of the page.
     */
    void setFormFields(const QList<FormField *> &fields);

    /**
     * Deletes the pixmap for the given @p observer
     */
    void deletePixmap(DocumentObserver *observer);

    /**
     * Deletes all pixmaps of the page.
     */
    void deletePixmaps();

    /**
     * Deletes all object rects of the page.
     */
    void deleteRects();

    /**
     * Deletes all source reference objects of the page.
     */
    void deleteSourceReferences();

    /**
     * Deletes all annotations of the page.
     */
    void deleteAnnotations();

    /**
     * Returns whether pixmaps for the tiled observer are handled by a
     * tile manager.
     *
     * @since 0.19 (KDE 4.13)
     */
    bool hasTilesManager(const DocumentObserver *observer) const;
    
    /**
     * Returns the pixmap matching the given parameters
     * @author david_librum
    */
    const QPixmap* getPixmap(DocumentObserver* observer, int width, int height) const;

    /**
     * Returns a list of all tiles intersecting with @p rect.
     *
     * The list contains only tiles with a pixmap
     *
     * @since 0.19 (KDE 4.13)
     */
    QList<Tile> tilesAt(const DocumentObserver *observer, const NormalizedRect &rect) const;

private:
    PagePrivate *d;
    /// @cond PRIVATE
    friend class PagePrivate;
    friend class Document;
    friend class DocumentPrivate;
    friend class PixmapRequestPrivate;

    /**
     * To improve performance PagePainter accesses the following
     * member variables directly.
     */
    friend class ::PagePainter;
    /// @endcond

    const QPixmap *_o_nearestPixmap(DocumentObserver *, int, int) const;

    QList<ObjectRect *> m_rects;
    QList<HighlightAreaRect *> m_highlights;
    QList<Annotation *> m_annotations;

    Q_DISABLE_COPY(Page)
};

}

#endif
