#include "book_service.hpp"
#include <mupdf/classes.h>
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