#include "book_controller.hpp"
#include <QUuid>
#include "bookmark.hpp"
#include "bookmarks_model.hpp"
#include "fz_utils.hpp"
#include "highlight.hpp"
#include "i_book_service.hpp"
#include "library_book_getter.hpp"
#include "search_options.hpp"

using namespace application::core;
using application::utility::LibraryBookGetter;
using domain::entities::Bookmark;
using domain::entities::Highlight;

namespace adapters::controllers
{

BookController::BookController(application::IBookService* bookService,
                               application::ILibraryService* libraryService) :
    m_bookService(bookService),
    m_libraryService(libraryService)
{
    connect(m_bookService, &application::IBookService::goToPosition, this,
            &BookController::goToPosition);

    connect(m_bookService, &application::IBookService::highlightText, this,
            [this](int pageNumber, mupdf::FzQuad quad)
            {
                auto rect = utils::fzQuadToQRectF(quad);
                QPointF left(rect.left(), rect.center().y());
                QPointF right(rect.right(), rect.center().y());

                emit selectText(pageNumber, left, right);
            });

    connect(m_bookService, &application::IBookService::noSearchHitsFound, this,
            &IBookController::noSearchHitsFound);
}

bool BookController::setUp(QString uuid)
{
    m_bookService->setUp(
        std::make_unique<LibraryBookGetter>(m_libraryService, QUuid(uuid)));

    m_bookmarksModel = std::make_unique<data_models::BookmarksModel>(
        QUuid(uuid), m_bookService);
    m_bookmarksProxyModel.setSourceModel(m_bookmarksModel.get());


    // Connections to bookmarks model
    connect(m_bookService, &application::IBookService::bookmarkInsertionStarted,
            m_bookmarksModel.get(),
            &data_models::BookmarksModel::startInsertingBookmark);

    connect(m_bookService, &application::IBookService::bookmarkInsertionEnded,
            m_bookmarksModel.get(),
            &data_models::BookmarksModel::endInsertingBookmark);

    connect(m_bookService, &application::IBookService::bookmarkDeletionStarted,
            m_bookmarksModel.get(),
            &data_models::BookmarksModel::startDeletingBookmark);

    connect(m_bookService, &application::IBookService::bookmarkDeletionStarted,
            m_bookmarksModel.get(),
            &data_models::BookmarksModel::endDeletingBookmark);

    connect(m_bookService, &application::IBookService::bookmarkNameChanged,
            m_bookmarksModel.get(),
            &data_models::BookmarksModel::bookmarkNameChanged);

    return true;
}

mupdf::FzDocument* BookController::getFzDocument()
{
    return m_bookService->getFzDocument();
}

void BookController::search(const QString& text)
{
    m_bookService->search(text, m_searchOptions);
}

void BookController::clearSearch()
{
    m_bookService->clearSearch();
}

void BookController::goToNextSearchHit()
{
    m_bookService->goToNextSearchHit();
}

void BookController::goToPreviousSearchHit()
{
    m_bookService->goToPreviousSearchHit();
}

const QList<Highlight>& BookController::getHighlights() const
{
    return m_bookService->getHighlights();
}

void BookController::addHighlight(const Highlight& highlight)
{
    m_bookService->addHighlight(highlight);
}

void BookController::removeHighlight(const QUuid& uuid)
{
    m_bookService->removeHighlight(uuid);
}

void BookController::changeHighlightColor(const QUuid& uuid,
                                          const QColor& color)
{
    m_bookService->changeHighlightColor(uuid, color);
}

const Highlight* BookController::getHighlightAtPoint(const QPointF& point,
                                                     int page) const
{
    auto restoredPoint = utils::restoreQPoint(point, getZoom());

    return m_bookService->getHighlightAtPoint(restoredPoint, page);
}

const QList<Bookmark>& BookController::getBookmark() const
{
    return m_bookService->getBookmarks();
}

QString BookController::addBookmark(const QString& name, int pageNumber,
                                    float yOffset)
{
    Bookmark bookmark(name, pageNumber, yOffset);
    m_bookService->addBookmark(bookmark);

    return bookmark.getUuid().toString(QUuid::WithoutBraces);
}

void BookController::renameBookmark(const QString& uuid, const QString& newName)
{
    m_bookService->renameBookmark(QUuid(uuid), newName);
}

void BookController::removeBookmark(const QString& uuid)
{
    m_bookService->removeBookmark(QUuid(uuid));
}

void BookController::goToBookmark(const QString& uuid)
{
    for(auto& bookmark : m_bookService->getBookmarks())
    {
        if(bookmark.getUuid() == QUuid(uuid))
        {
            emit goToPosition(bookmark.getPageNumber(), bookmark.getYOffset());
            break;
        }
    }
}

void BookController::followLink(const char* uri)
{
    return m_bookService->followLink(uri);
}

QString BookController::getFilePath() const
{
    return m_bookService->getFilePath();
}

int BookController::getPageCount() const
{
    return m_bookService->getPageCount();
}

void BookController::setCurrentPage(int newCurrentPage)
{
    m_bookService->setCurrentPage(newCurrentPage);
    m_searchOptions.currentPage = m_bookService->getCurrentPage();

    emit currentPageChanged(newCurrentPage);
}

int BookController::getCurrentPage() const
{
    return m_bookService->getCurrentPage();
}

float BookController::getZoom() const
{
    return m_bookService->getZoom();
}

void BookController::setZoom(float newZoom)
{
    m_bookService->setZoom(newZoom);
    emit zoomChanged(newZoom);
}

bool BookController::getSearchWholeWords() const
{
    return m_searchOptions.wholeWords;
}

void BookController::setSearchWholeWords(bool newSearchWholeWords)
{
    m_searchOptions.wholeWords = newSearchWholeWords;
    emit searchWholeWordsChanged();
}

bool BookController::getSearchCaseSensitive() const
{
    return m_searchOptions.caseSensitive;
}

void BookController::setSearchCaseSensitive(bool newCaseSensitive)
{
    m_searchOptions.caseSensitive = newCaseSensitive;
    emit searchCaseSensitiveChanged();
}

bool BookController::getSearchFromStart() const
{
    return m_searchOptions.fromStart;
}

void BookController::setSearchFromStart(bool newSearchFromStart)
{
    m_searchOptions.fromStart = newSearchFromStart;
    emit searchFromStartChanged();
}

QString BookController::getColorTheme()
{
    return m_bookService->getColorTheme();
}

void BookController::setColorTheme(const QString& colorTheme)
{
    m_bookService->setColorTheme(colorTheme);
    emit colorThemeChanged(colorTheme);
}

FilteredTOCModel* BookController::getTableOfContents()
{
    return m_bookService->getTableOfContents();
}

data_models::BookmarksProxyModel* BookController::getBookmarksModel()
{
    return &m_bookmarksProxyModel;
}

}  // namespace adapters::controllers
