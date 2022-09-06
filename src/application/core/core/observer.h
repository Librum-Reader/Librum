/*
    SPDX-FileCopyrightText: 2005 Enrico Ros <eros.kde@email.it>
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_DOCUMENTOBSERVER_H_
#define _OKULAR_DOCUMENTOBSERVER_H_

#include <QVector>

#include "okularcore_export.h"

namespace Okular
{
class Page;

/**
 * @short Base class for objects being notified when something changes.
 *
 * Inherit this class and call Document->addObserver( yourClass ) to get
 * notified of asynchronous events (new pixmap generated, or changed, etc..).
 */
class OKULARCORE_EXPORT DocumentObserver
{
public:
    DocumentObserver();
    /**
     * Destroys the document observer.
     */
    virtual ~DocumentObserver();

    DocumentObserver(const DocumentObserver &) = delete;
    DocumentObserver &operator=(const DocumentObserver &) = delete;

    /**
     * Flags that can be sent from the document to all observers to
     * inform them about the type of object that has been changed.
     */
    enum ChangedFlags {
        Pixmap = 1,        ///< Pixmaps has been changed
        Bookmark = 2,      ///< Bookmarks has been changed
        Highlights = 4,    ///< Highlighting information has been changed
        TextSelection = 8, ///< Text selection has been changed
        Annotations = 16,  ///< Annotations have been changed
        BoundingBox = 32,  ///< Bounding boxes have been changed
        NeedSaveAs = 64    ///< Set when "Save" is needed or annotation/form changes will be lost @since 0.15 (KDE 4.9) @deprecated
    };

    /**
     * ...
     */
    enum SetupFlags {
        DocumentChanged = 1,   ///< The document is a new document.
        NewLayoutForPages = 2, ///< All the pages have
        UrlChanged = 4         ///< The URL has changed @since 1.3
    };

    /**
     * This method is called whenever the document is initialized or reconstructed.
     *
     * @param pages The vector of pages of the document.
     * @param setupFlags the flags with the information about the setup
     */
    virtual void notifySetup(const QVector<Okular::Page *> &pages, int setupFlags);

    /**
     * This method is called whenever the viewport has been changed.
     *
     * @param smoothMove If true, the move shall be animated.
     */
    virtual void notifyViewportChanged(bool smoothMove);

    /**
     * This method is called whenever the content on @p page described by the
     * passed @p flags has been changed.
     */
    virtual void notifyPageChanged(int page, int flags);

    /**
     * This method is called whenever the content described by the passed @p flags
     * has been cleared.
     */
    virtual void notifyContentsCleared(int flags);

    /**
     * This method is called whenever the visible rects have been changed.
     */
    virtual void notifyVisibleRectsChanged();

    /**
     * This method is called whenever the zoom of the document has been changed.
     */
    virtual void notifyZoom(int factor);

    /**
     * Returns whether the observer agrees that all pixmaps for the given
     * @p page can be unloaded to improve memory usage.
     *
     * Returns true per default.
     */
    virtual bool canUnloadPixmap(int page) const;

    /**
     * This method is called after the current page of the document has been entered.
     *
     * @param previous The number of the previous page (is @c -1 for the initial page change).
     * @param current The number of the current page.
     *
     * @since 0.16 (KDE 4.10)
     */
    virtual void notifyCurrentPageChanged(int previous, int current);

private:
    class Private;
    const Private *d;
};

}

#endif
