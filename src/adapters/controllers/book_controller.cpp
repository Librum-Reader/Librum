#include "book_controller.hpp"
#include <QUuid>

using application::core::FilteredTOCModel;

namespace adapters::controllers
{

BookController::BookController(application::IBookService* bookService) :
    m_bookService(bookService)
{
    connect(m_bookService, &application::IBookService::goToPosition, this,
            &BookController::goToPosition);

    connect(m_bookService, &application::IBookService::highlightText, this,
            [this](int pageNumber, mupdf::FzQuad quad)
            {
                auto rect = fzQuadToQRectF(quad);
                emit highlightText(pageNumber, rect);
            });
}

void BookController::setUp(QString uuid)
{
    m_bookService->setUp(QUuid(uuid));
}

mupdf::FzDocument* BookController::getFzDocument()
{
    return m_bookService->getFzDocument();
}

void BookController::search(const QString& text)
{
    m_bookService->search(text);
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

FilteredTOCModel* BookController::getTableOfContents()
{
    return m_bookService->getTableOfContents();
}

QRectF BookController::fzQuadToQRectF(const mupdf::FzQuad& rect)
{
    float width = rect.ur.x - rect.ul.x;
    float height = rect.ll.y - rect.ul.y;
    return QRectF(rect.ul.x, rect.ul.y, width, height);
}

}  // namespace adapters::controllers