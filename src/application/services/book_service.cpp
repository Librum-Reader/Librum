#include "book_service.hpp"
#include <mupdf/classes.h>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <string>
#include "highlight.hpp"
#include "utils/book_searcher.hpp"

using namespace application::core;
using namespace utils;
using domain::entities::Highlight;

namespace application::services
{

void BookService::setUp(std::unique_ptr<IBookGetter> bookGetter)
{
    // Clean up previous book data first
    m_TOCModel = nullptr;

    m_bookGetter = std::move(bookGetter);
    auto book = m_bookGetter->getBook();
    auto stdFilePath = book->getFilePath().toStdString();
    m_fzDocument = std::make_unique<mupdf::FzDocument>(stdFilePath.c_str());

    m_bookSearcher = std::make_unique<BookSearcher>(m_fzDocument.get());
}

mupdf::FzDocument* BookService::getFzDocument()
{
    return m_fzDocument.get();
}

void BookService::search(const QString& text, SearchOptions searchOptions)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_bookSearcher->search(text, searchOptions);
    QApplication::restoreOverrideCursor();

    auto searchHit = m_bookSearcher->firstSearchHit();
    if(searchHit.pageNumber == -1)
    {
        emit noSearchHitsFound();
        return;
    }

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
    auto book = m_bookGetter->getBook();
    return book->getHighlights();
}

void BookService::addHighlight(const domain::entities::Highlight& highlight)
{
    auto book = m_bookGetter->getBook();
    book->addHighlight(highlight);

    updateBook();
}

void BookService::removeHighlight(const QUuid& uuid)
{
    auto book = m_bookGetter->getBook();
    book->removeHighlight(uuid);

    updateBook();
}

void BookService::changeHighlightColor(const QUuid& uuid, const QColor& color)
{
    auto book = m_bookGetter->getBook();
    book->changeHighlightColor(uuid, color);

    updateBook();
}

void BookService::updateBook()
{
    auto book = m_bookGetter->getBook();
    book->updateLastModified();
    m_bookGetter->updateBook(book);
}

const Highlight* BookService::getHighlightAtPoint(const QPointF& point,
                                                  int page) const
{
    auto book = m_bookGetter->getBook();
    for(auto& highlight : book->getHighlights())
    {
        if(highlight.getPageNumber() != page)
            continue;

        for(auto& rect : highlight.getRects())
        {
            if(rect.getQRect().contains(point))
                return &highlight;
        }
    }

    return nullptr;
}

const QList<domain::entities::Bookmark>& BookService::getBookmarks() const
{
    auto book = m_bookGetter->getBook();
    return book->getBookmarks();
}

void BookService::addBookmark(const domain::entities::Bookmark& bookmark)
{
    auto book = m_bookGetter->getBook();

    emit bookmarkInsertionStarted(book->getBookmarks().count());
    book->addBookmark(bookmark);
    emit bookmarkInsertionEnded();

    updateBook();
}

void BookService::renameBookmark(const QUuid& uuid, const QString& newName)
{
    auto book = m_bookGetter->getBook();

    book->renameBookmark(uuid, newName);
    emit bookmarkNameChanged(getIndexOfBookmark(uuid));

    updateBook();
}

void BookService::removeBookmark(const QUuid& uuid)
{
    auto book = m_bookGetter->getBook();

    emit bookmarkDeletionStarted(getIndexOfBookmark(uuid));
    book->removeBookmark(uuid);
    emit bookmarkDeletionEnded();

    updateBook();
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
    auto book = m_bookGetter->getBook();
    return book->getFilePath();
}

int BookService::getPageCount() const
{
    auto book = m_bookGetter->getBook();
    return book->getPageCount();
}

int BookService::getCurrentPage() const
{
    auto book = m_bookGetter->getBook();
    return book->getCurrentPage();
}

void BookService::setCurrentPage(int newCurrentPage)
{
    auto book = m_bookGetter->getBook();
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

QString BookService::getColorTheme()
{
    auto book = m_bookGetter->getBook();
    return book->getColorTheme();
}

void BookService::setColorTheme(const QString& colorTheme)
{
    auto book = m_bookGetter->getBook();
    book->setColorTheme(colorTheme);
    updateBook();
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

int BookService::getIndexOfBookmark(const QUuid& uuid) const
{
    auto book = m_bookGetter->getBook();

    auto bookmarks = book->getBookmarks();
    for(int i = 0; i < bookmarks.length(); ++i)
    {
        if(bookmarks.at(i).getUuid() == uuid)
            return i;
    }

    return -1;
}

}  // namespace application::services
