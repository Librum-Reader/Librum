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

BookService::BookService(ILibraryService* libraryService) :
    m_libraryService(libraryService)
{
}

void BookService::setUp(QUuid uuid)
{
    // Clean up previous book data first
    m_TOCModel = nullptr;
    m_book = nullptr;

    m_book = m_libraryService->getBook(uuid);
    if(m_book == nullptr)
    {
        qDebug() << "Failed opening book with uuid: " << uuid;
        return;
    }

    auto stdFilePath = m_book->getFilePath().toStdString();
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
    return m_book->getFilePath();
}

int BookService::getPageCount() const
{
    return m_book->getPageCount();
}

int BookService::getCurrentPage() const
{
    return m_book->getCurrentPage();
}

void BookService::setCurrentPage(int newCurrentPage)
{
    m_book->setCurrentPage(newCurrentPage);
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

}  // namespace application::services