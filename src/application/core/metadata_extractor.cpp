#include "metadata_extractor.hpp"
#include <QCryptographicHash>
#include <QDebug>
#include <QFileInfo>
#include "book_utils.hpp"
#include "fz_utils.hpp"
#include "page_generator.hpp"


using namespace domain::value_objects;

namespace application::core
{

bool MetadataExtractor::setup(const QString& filePath)
{
    m_filePath = filePath;
    try
    {
        auto stdFilePath = filePath.toStdString();
        m_document = std::make_unique<mupdf::FzDocument>(stdFilePath.c_str());
        return true;
    }
    catch(...)
    {
        qWarning() << "Failed creating Document for book at: " + filePath;
        return false;
    }
}

domain::value_objects::BookMetaData MetadataExtractor::getBookMetaData()
{
    auto title = getDocumentInfo("info:Title");
    if(title.isEmpty())
        title = getTitleFromPath();

    auto documentSize = getDocumentSize();

    BookMetaData metaData {
        .title = title,
        .authors = getDocumentInfo("info:Author"),
        .creator = getDocumentInfo("info:Creator"),
        .creationDate = getDocumentInfo("info:CreationDate"),
        .format = getDocumentInfo("format"),
        .extension = getBookExtension(),
        .language = "",
        .documentSize = documentSize,
        .pagesSize = "",
        .pageCount = m_document->fz_count_pages(),
        .lastOpened = QDateTime(),
        .coverLastModified = QDateTime(),
        .coverPath = "",
        .colorTheme = "Normal",
        .fileHash = utility::generateFileHash(m_filePath),
    };

    if(metaData.format.isEmpty())
        metaData.format = "Unknown";

    return metaData;
}

QString MetadataExtractor::getTitleFromPath()
{
    auto indexOfLastSlash = m_filePath.lastIndexOf("/");
    auto indexOfLastDot = m_filePath.lastIndexOf(".");

    if(indexOfLastDot == -1)
        return m_filePath.mid(indexOfLastSlash + 1);

    auto result = m_filePath.mid(indexOfLastSlash + 1,
                                 indexOfLastDot - indexOfLastSlash - 1);

    return result;
}

QString MetadataExtractor::getDocumentSize()
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

QString MetadataExtractor::getBookExtension()
{
    QFileInfo fileInfo(m_filePath);
    auto suffix = fileInfo.suffix();
    if(suffix.isEmpty())
        return getDocumentInfo("format");

    return suffix;
}

double MetadataExtractor::roundToPrecisionOf2(double raw)
{
    return (static_cast<int>(raw * 100 + .5) / 100.0);
}

QImage MetadataExtractor::getBookCover()
{
    try
    {
        return getCover();
    }
    catch(...)
    {
        qWarning() << "Failed loading book cover";
        return QImage();
    }
}

QString MetadataExtractor::getDocumentInfo(const char* key)
{
    try
    {
        auto result = m_document->fz_lookup_metadata2(key);
        if(result == "null")
            result = "";

        return QString::fromStdString(result);
    }
    catch(...)
    {
        return "";
    }
}

QImage MetadataExtractor::getCover()
{
    try
    {
        core::PageGenerator page(m_document.get(), 0);
        return utils::qImageFromPixmap(page.renderPage(1.0, "#FFFFFF"));	// #FFFFFF is white color for default cover
    }
    catch(...)
    {
        return QImage();
    }
}

}  // namespace application::core
