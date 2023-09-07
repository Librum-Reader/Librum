#include "book_service.hpp"
#include <mupdf/classes.h>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <string>

using namespace application::core;

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

    m_book = m_libraryService->getBook(uuid);
    if(m_book == nullptr)
    {
        qDebug() << "Failed opening book with uuid: " << uuid;
        return;
    }

    auto stdFilePath = m_book->getFilePath().toStdString();
    m_fzDocument = std::make_unique<mupdf::FzDocument>(stdFilePath.c_str());
}

mupdf::FzDocument* BookService::getFzDocument()
{
    return m_fzDocument.get();
}

void BookService::search(const QString& text)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    clearSearch();
    for(int i = 0; i < m_fzDocument->fz_count_pages(); ++i)
    {
        mupdf::FzStextOptions options;
        mupdf::FzStextPage textPage(*m_fzDocument, i, options);
        const int maxHits = 100;
        int hitMarks[maxHits];
        auto hits = textPage.search_stext_page(text.toStdString().c_str(),
                                               hitMarks, maxHits);

        m_searchHits.reserve(hits.size());
        for(auto& hit : hits)
        {
            SearchHit searchHit {
                .pageNumber = i,
                .rect = hit,
            };
            m_searchHits.emplace_back(searchHit);
        }
    }

    QApplication::restoreOverrideCursor();

    if(!m_searchHits.empty())
    {
        auto hit = m_searchHits.front();
        m_currentSearchHit = 0;

        emit goToPosition(hit.pageNumber, hit.rect.ul.y);
        emit highlightText(hit.pageNumber, hit.rect);
    }
}

void BookService::clearSearch()
{
    m_searchHits.clear();
    m_currentSearchHit = -1;
}

void BookService::goToNextSearchHit()
{
    if(m_currentSearchHit == -1 || m_searchHits.empty())
        return;

    // Wrap to the beginning once you are over the end
    ++m_currentSearchHit;
    if(m_currentSearchHit >= m_searchHits.size())
    {
        m_currentSearchHit = 0;
    }

    auto hit = m_searchHits.at(m_currentSearchHit);

    emit goToPosition(hit.pageNumber, hit.rect.ul.y);
    emit highlightText(hit.pageNumber, hit.rect);
}

void BookService::goToPreviousSearchHit()
{
    if(m_currentSearchHit == -1 || m_searchHits.empty())
        return;

    // Wrap to the beginning once you are over the end
    --m_currentSearchHit;
    if(m_currentSearchHit <= 0)
    {
        m_currentSearchHit = m_searchHits.size() - 1;
    }

    auto hit = m_searchHits.at(m_currentSearchHit);

    emit goToPosition(hit.pageNumber, hit.rect.ul.y);
    emit highlightText(hit.pageNumber, hit.rect);
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