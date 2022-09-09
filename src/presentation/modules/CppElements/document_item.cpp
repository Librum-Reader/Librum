#include "document_item.hpp"

#include <QMimeDatabase>
#include <QtQml> // krazy:exclude=includes

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

#include <core/bookmarkmanager.h>
#include <core/document_p.h>
#include <core/page.h>

#include "gui/signatureguiutils.h"
#include "gui/signaturemodel.h"
#include "gui/tocmodel.h"
#include "QDebug"


namespace cpp_elements
{

DocumentItem::DocumentItem(QObject *parent)
    : QObject(parent)
    , m_thumbnailObserver(nullptr)
    , m_pageviewObserver(nullptr)
    , m_searchInProgress(false)
{
    qmlRegisterUncreatableType<TOCModel>("org.kde.okular.private", 1, 0, "TOCModel", QStringLiteral("Do not create objects of this type."));
    qmlRegisterUncreatableType<SignatureModel>("org.kde.okular.private", 1, 0, "SignatureModel", QStringLiteral("Do not create objects of this type."));
    Okular::Settings::instance(QStringLiteral("okularproviderrc"));
    m_document = new Okular::Document(nullptr);
    m_tocModel = new TOCModel(m_document, this);
    m_signaturesModel = new SignatureModel(m_document, this);

    connect(m_document, &Okular::Document::searchFinished, this, &DocumentItem::searchFinished);
    connect(m_document->bookmarkManager(), &Okular::BookmarkManager::bookmarksChanged, this, &DocumentItem::bookmarkedPagesChanged);
    connect(m_document->bookmarkManager(), &Okular::BookmarkManager::bookmarksChanged, this, &DocumentItem::bookmarksChanged);
    connect(m_document, &Okular::Document::error, this, &DocumentItem::error);
    connect(m_document, &Okular::Document::warning, this, &DocumentItem::warning);
    connect(m_document, &Okular::Document::notice, this, &DocumentItem::notice);
}

DocumentItem::~DocumentItem()
{
    delete m_signaturesModel;
    delete m_document;
}

void DocumentItem::setUrl(const QUrl &url)
{
    openUrl(url, {});
}

void DocumentItem::openUrl(const QUrl &url, const QString &password)
{
    qDebug() << "Opening " << url;
    
    m_document->closeDocument();
    // TODO: password
    QMimeDatabase db;

    QUrl realUrl = url; // NOLINT(performance-unnecessary-copy-initialization) because of the ifdef below this can't be const &

#ifdef Q_OS_ANDROID
    realUrl = QUrl(QtAndroid::androidActivity().callObjectMethod("contentUrlToFd", "(Ljava/lang/String;)Ljava/lang/String;", QAndroidJniObject::fromString(url.toString()).object<jstring>()).toString());
#endif

    const QString path = realUrl.isLocalFile() ? realUrl.toLocalFile() : QStringLiteral("-");

    const Okular::Document::OpenResult res = m_document->openDocument(path, realUrl, db.mimeTypeForUrl(realUrl), password);

    m_tocModel->clear();
    m_tocModel->fill(m_document->documentSynopsis());
    m_tocModel->setCurrentViewport(m_document->viewport());

    m_matchingPages.clear();
    for (uint i = 0; i < m_document->pages(); ++i) {
        m_matchingPages << (int)i;
    }
    m_needsPassword = res == Okular::Document::OpenNeedsPassword;
    Q_EMIT matchingPagesChanged();
    Q_EMIT urlChanged();
    Q_EMIT pageCountChanged();
    Q_EMIT openedChanged();
    Q_EMIT needsPasswordChanged();
    Q_EMIT supportsSearchingChanged();
    Q_EMIT windowTitleForDocumentChanged();
    Q_EMIT bookmarkedPagesChanged();

    KMessageWidget::MessageType messageType;
    QString message;
    std::tie(messageType, message) = SignatureGuiUtils::documentSignatureMessageWidgetText(m_document);
    if (!message.isEmpty()) {
        if (messageType == KMessageWidget::Information) {
            Q_EMIT notice(message, -1);
        } else if (messageType == KMessageWidget::Warning) {
            Q_EMIT warning(message, -1);
        } else {
            qWarning() << "Unexpected message type" << messageType;
        }
    }
}

QString DocumentItem::windowTitleForDocument() const
{
    // If 'DocumentTitle' should be used, check if the document has one. If
    // either case is false, use the file name.
    QString title = Okular::Settings::displayDocumentNameOrPath() == Okular::Settings::EnumDisplayDocumentNameOrPath::Path ? m_document->currentDocument().toDisplayString(QUrl::PreferLocalFile) : m_document->currentDocument().fileName();

    if (Okular::Settings::displayDocumentTitle()) {
        const QString docTitle = m_document->metaData(QStringLiteral("DocumentTitle")).toString();

        if (!docTitle.isEmpty() && !docTitle.trimmed().isEmpty()) {
            title = docTitle;
        }
    }

    return title;
}

QUrl DocumentItem::url() const
{
    return m_document->currentDocument();
}

void DocumentItem::setCurrentPage(int page)
{
    m_document->setViewportPage(page);
    m_tocModel->setCurrentViewport(m_document->viewport());
    Q_EMIT currentPageChanged();
}

int DocumentItem::currentPage() const
{
    return m_document->currentPage();
}

bool DocumentItem::isOpened() const
{
    return m_document->isOpened();
}

int DocumentItem::pageCount() const
{
    return m_document->pages();
}

QVariantList DocumentItem::matchingPages() const
{
    return m_matchingPages;
}

TOCModel *DocumentItem::tableOfContents() const
{
    return m_tocModel;
}

SignatureModel *DocumentItem::signaturesModel() const
{
    return m_signaturesModel;
}

QVariantList DocumentItem::bookmarkedPages() const
{
    QList<int> list;
    QSet<int> pages;
    const KBookmark::List bMarks = m_document->bookmarkManager()->bookmarks();
    for (const KBookmark &bookmark : bMarks) {
        Okular::DocumentViewport viewport(bookmark.url().fragment());
        pages << viewport.pageNumber;
    }
    list = pages.values();
    std::sort(list.begin(), list.end());

    QVariantList variantList;
    for (const int page : qAsConst(list)) {
        variantList << page;
    }
    return variantList;
}

QStringList DocumentItem::bookmarks() const
{
    QStringList list;
    const KBookmark::List bMarks = m_document->bookmarkManager()->bookmarks();
    for (const KBookmark &bookmark : bMarks) {
        list << bookmark.url().toString();
    }
    return list;
}

bool DocumentItem::supportsSearching() const
{
    return m_document->supportsSearching();
}

bool DocumentItem::isSearchInProgress() const
{
    return m_searchInProgress;
}

void DocumentItem::searchText(const QString &text)
{
    if (text.isEmpty()) {
        resetSearch();
        return;
    }
    m_document->cancelSearch();
    m_document->resetSearch(PAGEVIEW_SEARCH_ID);
    m_document->searchText(PAGEVIEW_SEARCH_ID, text, true, Qt::CaseInsensitive, Okular::Document::AllDocument, true, QColor(100, 100, 200, 40));

    if (!m_searchInProgress) {
        m_searchInProgress = true;
        Q_EMIT searchInProgressChanged();
    }
}

void DocumentItem::resetSearch()
{
    m_document->resetSearch(PAGEVIEW_SEARCH_ID);
    m_matchingPages.clear();
    for (uint i = 0; i < m_document->pages(); ++i) {
        m_matchingPages << (int)i;
    }
    if (m_searchInProgress) {
        m_searchInProgress = false;
        Q_EMIT searchInProgressChanged();
    }

    Q_EMIT matchingPagesChanged();
}

void DocumentItem::setPassword(const QString &password)
{
    openUrl(m_document->currentDocument(), password);
}

Okular::Document *DocumentItem::document()
{
    return m_document;
}

Observer *DocumentItem::thumbnailObserver()
{
    if (!m_thumbnailObserver) {
        m_thumbnailObserver = new Observer(this);
    }

    return m_thumbnailObserver;
}

Observer *DocumentItem::pageviewObserver()
{
    if (!m_pageviewObserver) {
        m_pageviewObserver = new Observer(this);
    }

    return m_pageviewObserver;
}

void DocumentItem::searchFinished(int id, Okular::Document::SearchStatus endStatus)
{
    Q_UNUSED(endStatus)

    if (id != PAGEVIEW_SEARCH_ID) {
        return;
    }

    m_matchingPages.clear();
    for (uint i = 0; i < m_document->pages(); ++i) {
        if (m_document->page(i)->hasHighlights(id)) {
            m_matchingPages << (int)i;
        }
    }

    if (m_searchInProgress) {
        m_searchInProgress = false;
        Q_EMIT searchInProgressChanged();
    }
    Q_EMIT matchingPagesChanged();
}

// Observer

Observer::Observer(DocumentItem *parent)
    : QObject(parent)
    , m_document(parent)
{
    parent->document()->addObserver(this);
}

Observer::~Observer()
{
}

void Observer::notifyPageChanged(int page, int flags)
{
    Q_EMIT pageChanged(page, flags);
}

} // namespace cpp_elements