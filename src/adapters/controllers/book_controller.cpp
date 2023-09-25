#include "book_controller.hpp"
#include <QUuid>
#include "fz_utils.hpp"
#include "highlight.hpp"
#include "search_options.hpp"

using namespace application::core;
using domain::entities::Highlight;

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

                emit selectText(pageNumber, left, right);
            });

    connect(m_bookService, &application::IBookService::noSearchHitsFound, this,
            &IBookController::noSearchHitsFound);
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

void BookController::changeHighlightColor(const QUuid& uuid,
                                          const QColor& color)
{
    m_bookService->changeHighlightColor(uuid, color);
}

void BookController::saveHighlights()
{
    m_bookService->saveHighlights();
}

const Highlight* BookController::getHighlightAtPoint(const QPointF& point,
                                                     int page) const
{
    auto restoredPoint = utils::restoreQPoint(point, getZoom());

    return m_bookService->getHighlightAtPoint(restoredPoint, page);
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

bool BookController::getSearchWholeWords() const
{
    return m_searchOptions.wholeWords;
}

void BookController::setSearchWholeWords(bool newSearchWholeWords)
{
    m_searchOptions.wholeWords = newSearchWholeWords;
}

bool BookController::getSearchCaseSensitive() const
{
    return m_searchOptions.caseSensitive;
}

void BookController::setSearchCaseSensitive(bool newCaseSensitive)
{
    m_searchOptions.caseSensitive = newCaseSensitive;
}

FilteredTOCModel* BookController::getTableOfContents()
{
    return m_bookService->getTableOfContents();
}

}  // namespace adapters::controllers