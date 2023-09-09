#include "book_service.hpp"
#include <mupdf/classes.h>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <string>
#include "utils/book_searcher.hpp"

using namespace application::core;
using namespace utils;

namespace application::services
{

BookService::BookService(ILibraryService* libraryService,
                         IHighlightStorageManager* highlightStorageManager) :
    m_libraryService(libraryService),
    m_highlightStorageManager(highlightStorageManager)
{
}

void BookService::setUp(QUuid uuid)
{
    // Clean up previous book data first
    m_TOCModel = nullptr;

    m_uuid = uuid;
    auto book = getBook();
    auto stdFilePath = book->getFilePath().toStdString();
    m_fzDocument = std::make_unique<mupdf::FzDocument>(stdFilePath.c_str());

    m_bookSearcher = std::make_unique<BookSearcher>(m_fzDocument.get());
}

mupdf::FzDocument* BookService::getFzDocument()
{
    return m_fzDocument.get();
}

void BookService::search(const QString& text)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_bookSearcher->search(text);
    QApplication::restoreOverrideCursor();

    auto searchHit = m_bookSearcher->firstSearchHit();
    if(searchHit.pageNumber == -1)
        return;

    emit goToPosition(searchHit.pageNumber, searchHit.rect.ul.y);
    emit highlightText(searchHit.pageNumber, searchHit.rect);
}

void BookService::clearSearch()
{
    m_bookSearcher->clearSearch();
}

void BookService::goToNextSearchHit()
{
    auto searchHit = m_bookSearcher->nextSearchHit();
    if(searchHit.pageNumber == -1)
        return;

    emit goToPosition(searchHit.pageNumber, searchHit.rect.ul.y);
    emit highlightText(searchHit.pageNumber, searchHit.rect);
}

void BookService::goToPreviousSearchHit()
{
    auto searchHit = m_bookSearcher->previousSearchHit();
    if(searchHit.pageNumber == -1)
        return;

    emit goToPosition(searchHit.pageNumber, searchHit.rect.ul.y);
    emit highlightText(searchHit.pageNumber, searchHit.rect);
}

const QList<domain::entities::Highlight>& BookService::getHighlights() const
{
    auto book = getBook();
    return book->getHighlights();
}

void BookService::addHighlight(const domain::entities::Highlight& highlight)
{
    auto book = getBook();
    book->addHighlight(highlight);

    m_highlightStorageManager->addHighlight(*book, highlight);
}

void BookService::removeHighlight(const QUuid& uuid)
{
    auto book = getBook();
    book->removeHighlight(uuid);

    m_highlightStorageManager->deleteHighlight(*book, uuid);
}

void BookService::followLink(const char* uri)
{
    if(mupdf::ll_fz_is_external_link(uri))
    {
        QDesktopServices::openUrl(QUrl(uri));
    }
    else
    {
        float yp = 0;
        auto location = m_fzDocument->fz_resolve_link(uri, nullptr, &yp);
        int pageNumber = m_fzDocument->fz_page_number_from_location(location);

        emit goToPosition(pageNumber, yp);
    }
}

QString BookService::getFilePath() const
{
    auto book = getBook();
    return book->getFilePath();
}

int BookService::getPageCount() const
{
    auto book = getBook();
    return book->getPageCount();
}

int BookService::getCurrentPage() const
{
    auto book = getBook();
    return book->getCurrentPage();
}

void BookService::setCurrentPage(int newCurrentPage)
{
    auto book = getBook();
    book->setCurrentPage(newCurrentPage);
}

float BookService::getZoom() const
{
    return m_zoom;
}

void BookService::setZoom(float newZoom)
{
    m_zoom = newZoom;
}

core::FilteredTOCModel* BookService::getTableOfContents()
{
    if(m_TOCModel == nullptr)
    {
        auto data = m_fzDocument->fz_load_outline();
        m_TOCModel =
            std::make_unique<TOCModel>(data.m_internal, *m_fzDocument.get());
        m_filteredTOCModel = std::make_unique<FilteredTOCModel>();
        m_filteredTOCModel->setSourceModel(m_TOCModel.get());
    }

    return m_filteredTOCModel.get();
}

void BookService::setupUserData(const QString& token, const QString& email)
{
    m_highlightStorageManager->setUserData(email, token);
}

void BookService::clearUserData()
{
    m_highlightStorageManager->clearUserData();
}

/**
 * Everytime that we want to access data on the book we need to get the book
 * directly from the library service. We can't store it because the library
 * container can be resized at any time and thus invalidate the pointer.
 */
domain::entities::Book* BookService::getBook()
{
    auto* book = m_libraryService->getBook(m_uuid);
    if(book == nullptr)
        qWarning() << "Failed opening book with uuid: " << m_uuid;

    return book;
}

const domain::entities::Book* BookService::getBook() const
{
    auto* book = m_libraryService->getBook(m_uuid);
    if(book == nullptr)
        qWarning() << "Failed opening book with uuid: " << m_uuid;

    return book;
}

}  // namespace application::services