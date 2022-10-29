/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "bookmarkmanager.h"

// qt/kde includes
#include <KBookmarkAction>
#include <KBookmarkManager>
#include <KBookmarkMenu>
#include <QDebug>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHash>
#include <QSet>
#include <QStandardPaths>
#include <QUrl>

// local includes
#include "document_p.h"
#include "observer.h"

using namespace Okular;

#define foreachObserver(cmd)                            \
    {                                                   \
        QSet<DocumentObserver*>::const_iterator         \
            it = d->document->m_observers.constBegin(), \
            end = d->document->m_observers.constEnd();  \
        for(; it != end; ++it)                          \
        {                                               \
            (*it)->cmd;                                 \
        }                                               \
    }

#define foreachObserverD(cmd)                        \
    {                                                \
        QSet<DocumentObserver*>::const_iterator      \
            it = document->m_observers.constBegin(), \
            end = document->m_observers.constEnd();  \
        for(; it != end; ++it)                       \
        {                                            \
            (*it)->cmd;                              \
        }                                            \
    }

class OkularBookmarkAction : public KBookmarkAction
{
    Q_OBJECT

public:
    OkularBookmarkAction(const Okular::DocumentViewport& vp,
                         const KBookmark& bk, KBookmarkOwner* owner,
                         QObject* parent) :
        KBookmarkAction(bk, owner, parent)
    {
        if(vp.isValid())
        {
            setText(QString::number(vp.pageNumber + 1) + QStringLiteral(" - ") +
                    text());
        }
        setProperty("pageNumber", vp.pageNumber + 1);
        setProperty("htmlRef", bk.url().fragment(QUrl::FullyDecoded));
    }

    inline int pageNumber() const
    {
        return property("pageNumber").toInt();
    }

    inline QString htmlRef() const
    {
        return property("htmlRef").toString();
    }
};

static inline bool documentViewportFuzzyCompare(const DocumentViewport& vp1,
                                                const DocumentViewport& vp2)
{
    bool equal = vp1.isValid() && vp2.isValid() &&
                 (vp1.pageNumber == vp2.pageNumber) &&
                 (vp1.rePos.pos == vp2.rePos.pos);

    if(!equal)
    {
        return false;
    }

    if(qAbs(vp1.rePos.normalizedX - vp2.rePos.normalizedX) >= 0.000001)
    {
        return false;
    }

    if(qAbs(vp1.rePos.normalizedY - vp2.rePos.normalizedY) >= 0.000001)
    {
        return false;
    }

    return true;
}

static inline bool bookmarkLessThan(const KBookmark& b1, const KBookmark& b2)
{
    DocumentViewport vp1(b1.url().fragment(QUrl::FullyDecoded));
    DocumentViewport vp2(b2.url().fragment(QUrl::FullyDecoded));

    return vp1 < vp2;
}

static inline bool okularBookmarkActionLessThan(QAction* a1, QAction* a2)
{
    DocumentViewport vp1(static_cast<OkularBookmarkAction*>(a1)->htmlRef());
    DocumentViewport vp2(static_cast<OkularBookmarkAction*>(a2)->htmlRef());

    return vp1 < vp2;
}

static QUrl mostCanonicalUrl(const QUrl& url)
{
    if(!url.isLocalFile())
    {
        return url;
    }

    const QFileInfo fi(url.toLocalFile());
    return QUrl::fromLocalFile(fi.canonicalFilePath());
}

class BookmarkManager::Private : public KBookmarkOwner
{
public:
    explicit Private(BookmarkManager* qq) :
        KBookmarkOwner(),
        q(qq),
        document(nullptr),
        manager(nullptr)
    {
    }

    ~Private() override
    {
        knownFiles.clear();
        // no need to delete the manager, it's automatically done by
        // KBookmarkManager delete manager;
    }

    Private(const Private&) = delete;
    Private& operator=(const Private&) = delete;

    QUrl currentUrl() const override;
    QString currentTitle() const override;
    bool enableOption(BookmarkOption option) const override;
    void openBookmark(const KBookmark& bm, Qt::MouseButtons,
                      Qt::KeyboardModifiers) override;

    QHash<QUrl, QString>::iterator bookmarkFind(
        const QUrl& url, bool doCreate, KBookmarkGroup* result = nullptr);

    // slots
    void _o_changed(const QString& groupAddress, const QString& caller);

    BookmarkManager* q;
    QUrl url;
    QHash<int, int> urlBookmarks;
    DocumentPrivate* document;
    QString file;
    KBookmarkManager* manager;
    QHash<QUrl, QString> knownFiles;
};

static inline QUrl urlForGroup(const KBookmark& group)
{
    if(group.url().isValid())
    {
        return group.url();
    }
    else
    {
        return QUrl::fromUserInput(group.fullText());
    }
}

BookmarkManager::BookmarkManager(DocumentPrivate* document) :
    QObject(document->m_parent),
    d(new Private(this))
{
    setObjectName(QStringLiteral("Okular::BookmarkManager"));

    d->document = document;

    d->file =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
        QStringLiteral("/okular/bookmarks.xml");

    d->manager =
        KBookmarkManager::managerForFile(d->file, QStringLiteral("okular"));
    d->manager->setEditorOptions(QGuiApplication::applicationDisplayName(),
                                 false);
    d->manager->setUpdate(true);
    connect(d->manager, &KBookmarkManager::changed, this,
            [this](const QString& groupAddress, const QString& caller)
            {
                d->_o_changed(groupAddress, caller);
            });
}

BookmarkManager::~BookmarkManager()
{
    delete d;
}

// BEGIN Reimplementations from KBookmarkOwner
QUrl BookmarkManager::Private::currentUrl() const
{
    return url;
}

QString BookmarkManager::Private::currentTitle() const
{
    return url.toDisplayString();
}

bool BookmarkManager::Private::enableOption(BookmarkOption option) const
{
    Q_UNUSED(option)
    return false;
}

void BookmarkManager::Private::openBookmark(const KBookmark& bm,
                                            Qt::MouseButtons,
                                            Qt::KeyboardModifiers)
{
    Q_EMIT q->openUrl(bm.url());
}

// END Reimplementations from KBookmarkOwner

void BookmarkManager::Private::_o_changed(const QString& groupAddress,
                                          const QString& caller)
{
    Q_UNUSED(caller);
    if(groupAddress.isEmpty())
    {
        return;
    }

    QUrl referurl;
    // first, try to find the bookmark group whom change notification was just
    // received
    QHash<QUrl, QString>::iterator it = knownFiles.begin(),
                                   itEnd = knownFiles.end();
    for(; it != itEnd; ++it)
    {
        if(it.value() == groupAddress)
        {
            referurl = it.key();
            knownFiles.erase(it);
            break;
        }
    }
    if(!referurl.isValid())
    {
        const KBookmark bm = manager->findByAddress(groupAddress);
        // better be safe than sorry
        if(bm.isNull())
        {
            return;
        }
        Q_ASSERT(bm.isGroup());
        referurl = urlForGroup(bm);
    }
    Q_ASSERT(referurl.isValid());
    Q_EMIT q->bookmarksChanged(referurl);
    // case for the url representing the current document
    // (this might happen if the same document is open in another place;
    // in such case, make really sure to be in sync)
    if(referurl == url)
    {
        // save the old bookmarks for the current url
        const QHash<int, int> oldUrlBookmarks = urlBookmarks;
        // set the same url again, so we reload the information we have about it
        q->setUrl(referurl);
        // then notify the observers about the changes in the bookmarks
        for(int i = 0; i < qMax(oldUrlBookmarks.size(), urlBookmarks.size());
            i++)
        {
            bool oldContains =
                oldUrlBookmarks.contains(i) && oldUrlBookmarks[i] > 0;
            bool curContains = urlBookmarks.contains(i) && urlBookmarks[i] > 0;

            if(oldContains != curContains)
            {
                foreachObserverD(
                    notifyPageChanged(i, DocumentObserver::Bookmark));
            }
            else if(oldContains && oldUrlBookmarks[i] != urlBookmarks[i])
            {
                foreachObserverD(
                    notifyPageChanged(i, DocumentObserver::Bookmark));
            }
        }
    }
    Q_EMIT q->saved();
}

QList<QUrl> BookmarkManager::files() const
{
    QList<QUrl> ret;
    KBookmarkGroup group = d->manager->root();
    for(KBookmark bm = group.first(); !bm.isNull(); bm = group.next(bm))
    {
        if(bm.isSeparator() || !bm.isGroup())
        {
            continue;
        }

        ret.append(urlForGroup(bm));
    }
    return ret;
}

KBookmark::List BookmarkManager::bookmarks(const QUrl& documentUrl) const
{
    const QUrl url = mostCanonicalUrl(documentUrl);
    KBookmark::List ret;
    KBookmarkGroup group = d->manager->root();
    for(KBookmark bm = group.first(); !bm.isNull(); bm = group.next(bm))
    {
        if(!bm.isGroup() || urlForGroup(bm) != url)
        {
            continue;
        }

        KBookmarkGroup group = bm.toGroup();
        for(KBookmark b = group.first(); !b.isNull(); b = group.next(b))
        {
            if(b.isSeparator() || b.isGroup())
            {
                continue;
            }

            ret.append(b);
        }
        break;
    }

    return ret;
}

KBookmark::List BookmarkManager::bookmarks() const
{
    return bookmarks(d->url);
}

KBookmark::List BookmarkManager::bookmarks(int page) const
{
    const KBookmark::List bmarks = bookmarks();
    KBookmark::List ret;
    for(const KBookmark& bm : bmarks)
    {
        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(vp.isValid() && vp.pageNumber == page)
        {
            ret.append(bm);
        }
    }

    return ret;
}

KBookmark BookmarkManager::bookmark(int page) const
{
    const KBookmark::List bmarks = bookmarks();
    for(const KBookmark& bm : bmarks)
    {
        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(vp.isValid() && vp.pageNumber == page)
        {
            return bm;
        }
    }
    return KBookmark();
}

KBookmark BookmarkManager::bookmark(const DocumentViewport& viewport) const
{
    if(!viewport.isValid() || !isBookmarked(viewport.pageNumber))
    {
        return KBookmark();
    }

    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(d->url, false, &thebg);
    if(it == d->knownFiles.end())
    {
        return KBookmark();
    }

    for(KBookmark bm = thebg.first(); !bm.isNull(); bm = thebg.next(bm))
    {
        if(bm.isSeparator() || bm.isGroup())
        {
            continue;
        }

        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(documentViewportFuzzyCompare(vp, viewport))
        {
            return bm;
        }
    }

    return KBookmark();
}

void BookmarkManager::save() const
{
    d->manager->emitChanged();
    Q_EMIT const_cast<BookmarkManager*>(this)->saved();
}

QHash<QUrl, QString>::iterator BookmarkManager::Private::bookmarkFind(
    const QUrl& url, bool doCreate, KBookmarkGroup* result)
{
    QHash<QUrl, QString>::iterator it = knownFiles.find(url);
    if(it == knownFiles.end())
    {
        // if the url we want to add a new entry for is not in the hash of the
        // known files, then first try to find the file among the top-level
        // "folder" names
        bool found = false;
        KBookmarkGroup root = manager->root();
        for(KBookmark bm = root.first(); !found && !bm.isNull();
            bm = root.next(bm))
        {
            if(bm.isSeparator() || !bm.isGroup())
            {
                continue;
            }

            QUrl tmpurl(urlForGroup(bm));
            if(tmpurl == url)
            {
                // got it! place it the hash of known files
                KBookmarkGroup bg = bm.toGroup();
                it = knownFiles.insert(url, bg.address());
                found = true;
                if(result)
                {
                    *result = bg;
                }
                break;
            }
        }
        if(!found && doCreate)
        {
            // folder not found :(
            // then, in a single step create a new folder and add it in our
            // cache :)
            QString purl =
                url.isLocalFile() ? url.toLocalFile() : url.toDisplayString();
            KBookmarkGroup newbg = root.createNewFolder(purl);
            newbg.setUrl(url);
            it = knownFiles.insert(url, newbg.address());
            if(result)
            {
                *result = newbg;
            }
        }
    }
    else if(result)
    {
        const KBookmark bm = manager->findByAddress(it.value());
        Q_ASSERT(bm.isGroup());
        *result = bm.toGroup();
    }
    return it;
}

void BookmarkManager::addBookmark(int page)
{
    if(page >= 0 && page < (int)d->document->m_pagesVector.count())
    {
        if(setPageBookmark(page))
            foreachObserver(
                notifyPageChanged(page, DocumentObserver::Bookmark));
    }
}

void BookmarkManager::addBookmark(const DocumentViewport& vp)
{
    addBookmark(d->url, vp);
}

bool BookmarkManager::addBookmark(const QUrl& documentUrl,
                                  const Okular::DocumentViewport& vp,
                                  const QString& title)
{
    if(!documentUrl.isValid() || !vp.isValid())
    {
        return false;
    }

    if(vp.pageNumber < 0 || vp.pageNumber >= d->document->m_pagesVector.count())
    {
        return false;
    }

    const QUrl referurl = mostCanonicalUrl(documentUrl);

    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(referurl, true, &thebg);
    Q_ASSERT(it != d->knownFiles.end());

    int count = 0;  // Number of bookmarks in the current page
    bool found = false;
    // Check if the bookmark already exists
    for(KBookmark bm = thebg.first(); !found && !bm.isNull();
        bm = thebg.next(bm))
    {
        if(bm.isSeparator() || bm.isGroup())
        {
            continue;
        }

        DocumentViewport bmViewport(bm.url().fragment(QUrl::FullyDecoded));
        if(bmViewport.isValid() && bmViewport.pageNumber == vp.pageNumber)
        {
            ++count;

            if(documentViewportFuzzyCompare(bmViewport, vp))
            {
                found = true;
            }
        }
    }

    if(found)
    {
        return false;
    }

    QString newtitle;
    if(title.isEmpty())
    {
        // if we have no title specified for the new bookmark, then give it the
        // name '#p' where p is the page number where the bookmark is located.
        // if there's more than one bookmark per page, give the name '#p-n'
        // where n is the index of this bookmark among the ones of its page.
        if(count > 0)
        {
            newtitle =
                QStringLiteral("#%1-%2").arg(vp.pageNumber + 1).arg(count);
        }
        else
        {
            newtitle = QStringLiteral("#%1").arg(vp.pageNumber + 1);
        }
    }
    else
    {
        newtitle = title;
    }

    QUrl newurl = referurl;
    newurl.setFragment(vp.toString(), QUrl::DecodedMode);
    thebg.addBookmark(newtitle, newurl, QString());
    if(referurl == d->document->m_url)
    {
        d->urlBookmarks[vp.pageNumber]++;
        foreachObserver(
            notifyPageChanged(vp.pageNumber, DocumentObserver::Bookmark));
    }
    d->manager->emitChanged(thebg);
    return true;
}

void BookmarkManager::removeBookmark(int page)
{
    if(page >= 0 && page < (int)d->document->m_pagesVector.count())
    {
        if(removePageBookmark(page))
            foreachObserver(
                notifyPageChanged(page, DocumentObserver::Bookmark));
    }
}

void BookmarkManager::removeBookmark(const DocumentViewport& vp)
{
    int page = vp.pageNumber;
    if(page >= 0 && page < d->document->m_pagesVector.count())
    {
        removeBookmark(d->url, bookmark(vp));
    }
}

void BookmarkManager::renameBookmark(KBookmark* bm, const QString& newName)
{
    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(d->url, false, &thebg);
    Q_ASSERT(it != d->knownFiles.end());
    if(it == d->knownFiles.end())
    {
        return;
    }

    bm->setFullText(newName);
    d->manager->emitChanged(thebg);
}

void BookmarkManager::renameBookmark(const QUrl& documentUrl,
                                     const QString& newName)
{
    if(!documentUrl.isValid())
    {
        return;
    }

    const QUrl referurl = mostCanonicalUrl(documentUrl);

    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it =
        d->bookmarkFind(referurl, false, &thebg);
    Q_ASSERT(it != d->knownFiles.end());
    if(it == d->knownFiles.end())
    {
        return;
    }

    thebg.setFullText(newName);
    d->manager->emitChanged(thebg);
}

QString BookmarkManager::titleForUrl(const QUrl& documentUrl) const
{
    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it =
        d->bookmarkFind(mostCanonicalUrl(documentUrl), false, &thebg);
    Q_ASSERT(it != d->knownFiles.end());

    return thebg.fullText();
}

int BookmarkManager::removeBookmark(const QUrl& documentUrl,
                                    const KBookmark& bm)
{
    if(!documentUrl.isValid() || bm.isNull() || bm.isGroup() ||
       bm.isSeparator())
    {
        return -1;
    }

    DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
    if(!vp.isValid())
    {
        return -1;
    }

    const QUrl referurl = mostCanonicalUrl(documentUrl);

    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it =
        d->bookmarkFind(referurl, false, &thebg);
    if(it == d->knownFiles.end())
    {
        return -1;
    }

    thebg.deleteBookmark(bm);

    if(referurl == d->document->m_url)
    {
        d->urlBookmarks[vp.pageNumber]--;
        foreachObserver(
            notifyPageChanged(vp.pageNumber, DocumentObserver::Bookmark));
    }
    d->manager->emitChanged(thebg);

    return vp.pageNumber;
}

void BookmarkManager::removeBookmarks(const QUrl& documentUrl,
                                      const KBookmark::List& list)
{
    if(!documentUrl.isValid() || list.isEmpty())
    {
        return;
    }

    const QUrl referurl = mostCanonicalUrl(documentUrl);

    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it =
        d->bookmarkFind(referurl, false, &thebg);
    if(it == d->knownFiles.end())
    {
        return;
    }

    const QHash<int, int> oldUrlBookmarks = d->urlBookmarks;
    bool deletedAny = false;
    for(const KBookmark& bm : list)
    {
        if(bm.parentGroup() == thebg)
        {
            thebg.deleteBookmark(bm);
            deletedAny = true;

            DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
            if(referurl == d->document->m_url)
            {
                d->urlBookmarks[vp.pageNumber]--;
            }
        }
    }

    if(referurl == d->document->m_url)
    {
        for(int i = 0; i < qMax(oldUrlBookmarks.size(), d->urlBookmarks.size());
            i++)
        {
            bool oldContains =
                oldUrlBookmarks.contains(i) && oldUrlBookmarks[i] > 0;
            bool curContains =
                d->urlBookmarks.contains(i) && d->urlBookmarks[i] > 0;

            if(oldContains != curContains)
            {
                foreachObserver(
                    notifyPageChanged(i, DocumentObserver::Bookmark));
            }
            else if(oldContains && oldUrlBookmarks[i] != d->urlBookmarks[i])
            {
                foreachObserver(
                    notifyPageChanged(i, DocumentObserver::Bookmark));
            }
        }
    }
    if(deletedAny)
    {
        d->manager->emitChanged(thebg);
    }
}

QList<QAction*> BookmarkManager::actionsForUrl(const QUrl& documentUrl) const
{
    const QUrl url = mostCanonicalUrl(documentUrl);
    QList<QAction*> ret;
    KBookmarkGroup group = d->manager->root();
    for(KBookmark bm = group.first(); !bm.isNull(); bm = group.next(bm))
    {
        if(!bm.isGroup() || urlForGroup(bm) != url)
        {
            continue;
        }

        KBookmarkGroup group = bm.toGroup();
        for(KBookmark b = group.first(); !b.isNull(); b = group.next(b))
        {
            if(b.isSeparator() || b.isGroup())
            {
                continue;
            }

            ret.append(new OkularBookmarkAction(
                DocumentViewport(b.url().fragment(QUrl::FullyDecoded)), b, d,
                nullptr));
        }
        break;
    }
    std::sort(ret.begin(), ret.end(), okularBookmarkActionLessThan);
    return ret;
}

void BookmarkManager::setUrl(const QUrl& url)
{
    d->url = mostCanonicalUrl(url);
    d->urlBookmarks.clear();
    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(d->url, false, &thebg);
    if(it != d->knownFiles.end())
    {
        for(KBookmark bm = thebg.first(); !bm.isNull(); bm = thebg.next(bm))
        {
            if(bm.isSeparator() || bm.isGroup())
            {
                continue;
            }

            DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
            if(!vp.isValid())
            {
                continue;
            }

            d->urlBookmarks[vp.pageNumber]++;
        }
    }
}

bool BookmarkManager::setPageBookmark(int page)
{
    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(d->url, true, &thebg);
    Q_ASSERT(it != d->knownFiles.end());

    bool found = false;
    bool added = false;
    for(KBookmark bm = thebg.first(); !found && !bm.isNull();
        bm = thebg.next(bm))
    {
        if(bm.isSeparator() || bm.isGroup())
        {
            continue;
        }

        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(vp.isValid() && vp.pageNumber == page)
        {
            found = true;
        }
    }
    if(!found)
    {
        d->urlBookmarks[page]++;
        DocumentViewport vp;
        vp.pageNumber = page;
        QUrl newurl = d->url;
        newurl.setFragment(vp.toString(), QUrl::DecodedMode);
        thebg.addBookmark(
            QLatin1String("#") + QString::number(vp.pageNumber + 1), newurl,
            QString());
        added = true;
        d->manager->emitChanged(thebg);
    }
    return added;
}

bool BookmarkManager::removePageBookmark(int page)
{
    KBookmarkGroup thebg;
    QHash<QUrl, QString>::iterator it = d->bookmarkFind(d->url, false, &thebg);
    if(it == d->knownFiles.end())
    {
        return false;
    }

    bool found = false;
    for(KBookmark bm = thebg.first(); !found && !bm.isNull();
        bm = thebg.next(bm))
    {
        if(bm.isSeparator() || bm.isGroup())
        {
            continue;
        }

        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(vp.isValid() && vp.pageNumber == page)
        {
            found = true;
            thebg.deleteBookmark(bm);
            d->urlBookmarks[page]--;
            d->manager->emitChanged(thebg);
        }
    }
    return found;
}

bool BookmarkManager::isBookmarked(int page) const
{
    return d->urlBookmarks.contains(page) && d->urlBookmarks[page] > 0;
}

bool BookmarkManager::isBookmarked(const DocumentViewport& viewport) const
{
    KBookmark bm = bookmark(viewport);

    return !bm.isNull();
}

KBookmark BookmarkManager::nextBookmark(const DocumentViewport& viewport) const
{
    KBookmark::List bmarks = bookmarks();
    std::sort(bmarks.begin(), bmarks.end(), bookmarkLessThan);

    KBookmark bookmark;
    for(const KBookmark& bm : qAsConst(bmarks))
    {
        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(viewport < vp)
        {
            bookmark = bm;
            break;
        }
    }

    return bookmark;
}

KBookmark BookmarkManager::previousBookmark(
    const DocumentViewport& viewport) const
{
    KBookmark::List bmarks = bookmarks();
    std::sort(bmarks.begin(), bmarks.end(), bookmarkLessThan);

    KBookmark bookmark;
    for(KBookmark::List::const_iterator it = bmarks.constEnd();
        it != bmarks.constBegin(); --it)
    {
        KBookmark bm = *(it - 1);
        DocumentViewport vp(bm.url().fragment(QUrl::FullyDecoded));
        if(vp < viewport)
        {
            bookmark = bm;
            break;
        }
    }

    return bookmark;
}

#undef foreachObserver
#undef foreachObserverD

#include "bookmarkmanager.moc"

/* kate: replace-tabs on; indent-width 4; */
