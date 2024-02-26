#include "external_book_controller.hpp"
#include <QUuid>
#include <memory>
#include "bookmark.hpp"
#include "external_book_getter.hpp"
#include "fz_utils.hpp"
#include "highlight.hpp"
#include "i_book_service.hpp"
#include "search_options.hpp"

using namespace application::core;
using application::utility::ExternalBookGetter;
using domain::entities::Bookmark;
using domain::entities::Highlight;

namespace adapters::controllers
{

ExternalBookController::ExternalBookController(
    application::IBookService* bookService) :
    m_externalBookService(bookService)
{
    connect(m_externalBookService, &application::IBookService::goToPosition,
            this, &ExternalBookController::goToPosition);

    connect(m_externalBookService, &application::IBookService::highlightText,
            this,
            [this](int pageNumber, mupdf::FzQuad quad)
            {
                auto rect = utils::fzQuadToQRectF(quad);
                QPointF left(rect.left(), rect.center().y());
                QPointF right(rect.right(), rect.center().y());

                emit selectText(pageNumber, left, right);
            });

    connect(m_externalBookService,
            &application::IBookService::noSearchHitsFound, this,
            &IBookController::noSearchHitsFound);
}

void ExternalBookController::setUp(QString filePath)
{
    m_externalBookService->setUp(
        std::make_unique<ExternalBookGetter>(filePath));
}

mupdf::FzDocument* ExternalBookController::getFzDocument()
{
    return m_externalBookService->getFzDocument();
}

void ExternalBookController::search(const QString& text)
{
    m_externalBookService->search(text, m_searchOptions);
}

void ExternalBookController::clearSearch()
{
    m_externalBookService->clearSearch();
}

void ExternalBookController::goToNextSearchHit()
{
    m_externalBookService->goToNextSearchHit();
}

void ExternalBookController::goToPreviousSearchHit()
{
    m_externalBookService->goToPreviousSearchHit();
}

const QList<Highlight>& ExternalBookController::getHighlights() const
{
}

void ExternalBookController::addHighlight(const Highlight& highlight)
{
    Q_UNUSED(highlight);
}

void ExternalBookController::removeHighlight(const QUuid& uuid)
{
    Q_UNUSED(uuid);
}

void ExternalBookController::changeHighlightColor(const QUuid& uuid,
                                                  const QColor& color)
{
    Q_UNUSED(uuid);
    Q_UNUSED(color);
}

const Highlight* ExternalBookController::getHighlightAtPoint(
    const QPointF& point, int page) const
{
    Q_UNUSED(point);
    Q_UNUSED(page);

    return nullptr;
}

const QList<Bookmark>& ExternalBookController::getBookmark() const
{
}

QString ExternalBookController::addBookmark(const QString& name, int pageNumber,
                                            float yOffset)
{
    Q_UNUSED(name);
    Q_UNUSED(pageNumber);
    Q_UNUSED(yOffset);
}

void ExternalBookController::renameBookmark(const QString& uuid,
                                            const QString& newName)
{
    Q_UNUSED(uuid);
    Q_UNUSED(newName);
}

void ExternalBookController::removeBookmark(const QString& uuid)
{
    Q_UNUSED(uuid);
}

void ExternalBookController::goToBookmark(const QString& uuid)
{
    Q_UNUSED(uuid);
}

void ExternalBookController::followLink(const char* uri)
{
    return m_externalBookService->followLink(uri);
}

QString ExternalBookController::getFilePath() const
{
    return m_externalBookService->getFilePath();
}

int ExternalBookController::getPageCount() const
{
    return m_externalBookService->getPageCount();
}

void ExternalBookController::setCurrentPage(int newCurrentPage)
{
    m_externalBookService->setCurrentPage(newCurrentPage);
    m_searchOptions.currentPage = m_externalBookService->getCurrentPage();

    emit currentPageChanged(newCurrentPage);
}

int ExternalBookController::getCurrentPage() const
{
    return m_externalBookService->getCurrentPage();
}

float ExternalBookController::getZoom() const
{
    return m_externalBookService->getZoom();
}

void ExternalBookController::setZoom(float newZoom)
{
    m_externalBookService->setZoom(newZoom);
    emit zoomChanged(newZoom);
}

bool ExternalBookController::getSearchWholeWords() const
{
    return m_searchOptions.wholeWords;
}

void ExternalBookController::setSearchWholeWords(bool newSearchWholeWords)
{
    m_searchOptions.wholeWords = newSearchWholeWords;
    emit searchWholeWordsChanged();
}

bool ExternalBookController::getSearchCaseSensitive() const
{
    return m_searchOptions.caseSensitive;
}

void ExternalBookController::setSearchCaseSensitive(bool newCaseSensitive)
{
    m_searchOptions.caseSensitive = newCaseSensitive;
    emit searchCaseSensitiveChanged();
}

bool ExternalBookController::getSearchFromStart() const
{
    return m_searchOptions.fromStart;
}

void ExternalBookController::setSearchFromStart(bool newSearchFromStart)
{
    m_searchOptions.fromStart = newSearchFromStart;
    emit searchFromStartChanged();
}

QString ExternalBookController::getColorTheme()
{
    return m_externalBookService->getColorTheme();
}

void ExternalBookController::setColorTheme(const QString& colorTheme)
{
    m_externalBookService->setColorTheme(colorTheme);
    emit colorThemeChanged(colorTheme);
}

FilteredTOCModel* ExternalBookController::getTableOfContents()
{
    return m_externalBookService->getTableOfContents();
}

data_models::BookmarksProxyModel* ExternalBookController::getBookmarksModel()
{
    return nullptr;
}

}  // namespace adapters::controllers
