/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_BOOKMARK_MANAGER_H_
#define _OKULAR_BOOKMARK_MANAGER_H_

#include </usr/include/KF5/KBookmarks/KBookmark>

#include "okularcore_export.h"
#include <QObject>
#include <QUrl>

class QAction;

namespace Okular
{
class Document;
class DocumentPrivate;
class DocumentViewport;

/**
 * @brief Bookmarks manager utility.
 *
 * This class is responsible for loading and saving the bookmarks using the
 * proper format, and for working with them (eg querying, adding, removing).
 */
class OKULARCORE_EXPORT BookmarkManager : public QObject
{
    Q_OBJECT

public:
    ~BookmarkManager() override;

    /**
     * Returns the list of documents with bookmarks.
     */
    QList<QUrl> files() const;

    /**
     * Returns the list of bookmarks for the specified @p documentUrl.
     */
    KBookmark::List bookmarks(const QUrl &documentUrl) const;

    /**
     * Returns the list of bookmarks for document
     * @since 0.14 (KDE 4.8)
     */
    KBookmark::List bookmarks() const;

    /**
     * Returns the list of bookmarks for the given page of the document
     * @since 0.15 (KDE 4.9)
     */
    KBookmark::List bookmarks(int page) const;

    /**
     * Returns the bookmark for the given page of the document
     * @since 0.14 (KDE 4.8)
     */
    KBookmark bookmark(int page) const;

    /**
     * Returns the bookmark for the given @p viewport of the document
     * @since 0.15 (KDE 4.9)
     */
    KBookmark bookmark(const DocumentViewport &viewport) const;

    /**
     * Forces to save the list of bookmarks.
     */
    void save() const;

    /**
     * Adds a bookmark for the given @p page.
     */
    void addBookmark(int page);

    /**
     * Adds a bookmark for the given viewport @p vp
     * @since 0.15 (KDE 4.9)
     */
    void addBookmark(const DocumentViewport &vp);

    /**
     * Adds a new bookmark for the @p documentUrl at the specified viewport @p vp,
     * with an optional @p title.
     *
     * If no @p title is specified, then \em \#n will be used.
     */
    bool addBookmark(const QUrl &documentUrl, const Okular::DocumentViewport &vp, const QString &title = QString());

    /**
     * Remove a bookmark for the given @p page.
     */
    void removeBookmark(int page);

    /**
     * Remove a bookmark for the given viewport @p vp
     * @since 0.15 (KDE 4.9)
     */
    void removeBookmark(const DocumentViewport &vp);

    /**
     * Removes the bookmark @p bm for the @p documentUrl specified.
     */
    int removeBookmark(const QUrl &documentUrl, const KBookmark &bm);

    /**
     * Removes the bookmarks in @p list for the @p documentUrl specified.
     *
     * @note it will remove only the bookmarks which belong to @p documentUrl
     *
     * @since 0.11 (KDE 4.5)
     */
    void removeBookmarks(const QUrl &documentUrl, const KBookmark::List &list);

    /**
     * Returns the bookmark given bookmark of the document
     * @since 0.14 (KDE 4.8)
     */
    void renameBookmark(KBookmark *bm, const QString &newName);

    /**
     * Renames the top-level bookmark for the @p documentUrl specified with
     * the @p newName specified.
     * @since 0.15 (KDE 4.9)
     */
    void renameBookmark(const QUrl &documentUrl, const QString &newName);

    /**
     * Returns title for the @p documentUrl
     * @since 0.15 (KDE 4.9)
     */
    QString titleForUrl(const QUrl &documentUrl) const;

    /**
     * Returns whether the given @p page is bookmarked.
     */
    bool isBookmarked(int page) const;

    /**
     * Return whether the given @p viewport is bookmarked.
     * @since 0.15 (KDE 4.9)
     */
    bool isBookmarked(const DocumentViewport &viewport) const;

    /**
     * Given a @p viewport, returns the next bookmark
     * @since 0.15 (KDE 4.9)
     */
    KBookmark nextBookmark(const DocumentViewport &viewport) const;

    /**
     * Given a @p viewport, returns the previous bookmark
     * @since 0.15 (KDE 4.9)
     */
    KBookmark previousBookmark(const DocumentViewport &viewport) const;

    /**
     * Returns a list of actions for the bookmarks of the specified @p url.
     *
     * @note the actions will have no parents, so you have to delete them
     * yourself
     */
    QList<QAction *> actionsForUrl(const QUrl &documentUrl) const;

Q_SIGNALS:
    /**
     * The bookmark manager is requesting to open the specified @p url.
     */
    void openUrl(const QUrl &url);

    /**
     * This signal is emitted whenever bookmarks have been saved.
     */
    void saved();

    /**
     * The bookmarks for specified @p url were changed.
     *
     * @since 0.7 (KDE 4.1)
     */
    void bookmarksChanged(const QUrl &url);

private:
    class Private;
    Private *const d;
    friend class Private;

    // private interface used by the Document
    friend class Document;
    friend class DocumentPrivate;

    explicit BookmarkManager(DocumentPrivate *document);

    void setUrl(const QUrl &url);
    bool setPageBookmark(int page);
    bool removePageBookmark(int page);

    Q_DISABLE_COPY(BookmarkManager)
};

}

#endif
