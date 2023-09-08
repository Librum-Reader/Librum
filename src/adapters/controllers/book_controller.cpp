#include "book_controller.hpp"
#include <QUuid>
#include "fz_utils.hpp"

using namespace application::core;

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
                auto rect = utils::fzQuadToQRectF(quad);
                QPointF left(rect.left(), rect.center().y());
                QPointF right(rect.right(), rect.center().y());

                left = utils::scalePointToCurrentZoom(left, 1, getZoom());
                right = utils::scalePointToCurrentZoom(right, 1, getZoom());

                emit highlightText(pageNumber, left, right);
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

const QList<domain::entities::Highlight>& BookController::getHighlights() const
{
    return m_bookService->getHighlights();
}

void BookController::addHighlight(const domain::entities::Highlight& highlight)
{
    m_bookService->addHighlight(highlight);
}

void BookController::removeHighlight(const QUuid& uuid)
{
    m_bookService->removeHighlight(uuid);
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

}  // namespace adapters::controllers