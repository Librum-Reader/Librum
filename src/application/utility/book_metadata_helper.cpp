#include "book_metadata_helper.hpp"
#include <QDebug>
#include <QFileInfo>
#include "document.hpp"


using namespace domain::value_objects;

namespace application::utility
{

bool BookMetadataHelper::setup(const QString& filePath)
{
    m_filePath = filePath;
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
    auto title = m_document->getTitle();
    if(title.isEmpty())
        title = getTitleFromPath();

    auto documentSize = getDocumentSize();

    BookMetaData metaData {
        .title = title,
        .authors = m_document->getAuthors(),
        .creator = m_document->getCreator(),
        .creationDate = m_document->getCreationDate(),
        .format = m_document->getFormat(),
        .language = "",
        .documentSize = documentSize,
        .pagesSize = "",
        .pageCount = m_document->getPageCount(),
        .lastOpened = QDateTime(),
    };

    if(metaData.format.isEmpty())
        metaData.format = "Unknown";

    return metaData;
}

QString BookMetadataHelper::getTitleFromPath()
{
    auto indexOfLastSlash = m_filePath.lastIndexOf("/");
    auto indexOfLastDot = m_filePath.lastIndexOf(".");

    if(indexOfLastDot == -1)
        return m_filePath.mid(indexOfLastSlash + 1);

    auto result = m_filePath.mid(indexOfLastSlash + 1,
                                 indexOfLastDot - indexOfLastSlash - 1);

    return result;
}

QString BookMetadataHelper::getDocumentSize()
{
    QFileInfo fileInfo(m_filePath);
    double fileSize = fileInfo.size();  // Needs to be double for divisions

    qint64 KB = 1000;
    qint64 MB = KB * 1000;
    qint64 GB = MB * 1000;

    QString result;
    if(fileSize >= GB)
    {
        double raw = fileSize / GB;
        result = QString::number(roundToPrecisionOf2(raw)) + " GB";
    }
    else if(fileSize >= MB)
    {
        double raw = fileSize / MB;
        result = QString::number(roundToPrecisionOf2(raw)) + " MB";
    }
    else if(fileSize >= KB)
    {
        double raw = fileSize / KB;
        result = QString::number(roundToPrecisionOf2(raw)) + " KB";
    }
    else
    {
        result = QString::number(fileSize) + " B";
    }

    return result;
}

double BookMetadataHelper::roundToPrecisionOf2(double raw)
{
    return (static_cast<int>(raw * 100 + .5) / 100.0);
};

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