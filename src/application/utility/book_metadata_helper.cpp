#include "book_metadata_helper.hpp"
#include <QDebug>
#include "document.hpp"


using namespace domain::value_objects;

namespace application::utility
{

bool BookMetadataHelper::setup(const QString& filePath)
{
    try
    {
        m_document = std::make_unique<core::Document>(filePath);
        return true;
    }
    catch(...)
    {
        qWarning() << "Failed creating Document for book at: " + filePath;
        return false;
    }
}

domain::value_objects::BookMetaData BookMetadataHelper::getBookMetaData()
{
    BookMetaData metaData {
        .title = m_document->getTitle(),
        .authors = m_document->getAuthors(),
        .creator = m_document->getCreator(),
        .creationDate = m_document->getCreationDate(),
        .format = m_document->getFormat(),
        .language = "",
        .documentSize = "",
        .pagesSize = "",
        .pageCount = m_document->getPageCount(),
        .lastOpened = QDateTime(),
    };

    return metaData;
}

QImage BookMetadataHelper::getBookCover()
{
    try
    {
        return m_document->getCover();
    }
    catch(...)
    {
        qWarning() << "Failed loading book cover";
        return QImage();
    }
}

}  // namespace application::utility