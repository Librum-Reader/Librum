#include "book_metadata_helper.hpp"
#include <QDateTime>
#include <QMimeDatabase>
#include "book.hpp"
#include "document.h"
#include "generator.h"
#include "observer.h"
#include "page.h"
#include "settings.hpp"


using namespace Okular;
using namespace domain::models;

namespace application::utility
{


std::optional<BookMetaData> BookMetadataHelper::getBookMetaData(
    const QString& filePath)
{
    auto setupSucceeded = setupDocument(filePath);
    if(!setupSucceeded)
        return std::nullopt;

    BookMetaData metaData {
        .title = getTitle(filePath),
        .author = getAuthor(),
        .creator = getCreator(),
        .creationDate = getCreationDate(),
        .format = getFormat(),
        .language = "",
        .documentSize = getDocumentSize(),
        .pagesSize = getPagesSize(),
        .pageCount = getPageCount(),
        .lastOpened = QDateTime(),
        .cover = QImage(),
    };

    return metaData;
}

bool BookMetadataHelper::setupDocument(const QString& filePath)
{
    Settings::instance(QStringLiteral("okularproviderrc"));
    m_document = std::make_unique<Document>(nullptr);

    setupDocumentObserver();

    auto systemRelativPath = getSystemRelativePath(filePath);
    auto mimeType = getMimeType(filePath);
    QString password = "";

    auto openResult = m_document->openDocument(systemRelativPath, filePath,
                                               mimeType, password);

    return openResult == Document::OpenSuccess;
}

void BookMetadataHelper::setupDocumentObserver()
{
    m_observer = std::make_unique<CoverObserver>();
    m_document->addObserver(m_observer.get());
}

QString BookMetadataHelper::getTitle(const QString& filePath) const
{
    const QString& title = m_document->documentInfo().get(DocumentInfo::Title);
    if(title.isEmpty())
    {
        auto systemRelativPath = getSystemRelativePath(filePath);
        return parseTitleFromPath(systemRelativPath);
    }

    return title;
}

QString BookMetadataHelper::getAuthor() const
{
    const QString& author =
        m_document->documentInfo().get(DocumentInfo::Author);
    return author;
}

QString BookMetadataHelper::getCreator() const
{
    const QString& creator =
        m_document->documentInfo().get(DocumentInfo::Creator);
    return creator;
}

QString BookMetadataHelper::getCreationDate() const
{
    const QString& creationDate =
        m_document->documentInfo().get(DocumentInfo::CreationDate);
    return creationDate;
}

QString BookMetadataHelper::getFormat() const
{
    const QString& format =
        m_document->documentInfo().get(DocumentInfo::MimeType);

    auto formatWithoutType = removeTypeFromMimeString(format);
    auto result = removeAppendingsFromMimeString(formatWithoutType);

    return result;
}

QString BookMetadataHelper::getDocumentSize() const
{
    const QString& docSize =
        m_document->documentInfo().get(DocumentInfo::DocumentSize);
    return docSize;
}

QString BookMetadataHelper::getPagesSize() const
{
    const QString& pagesSize =
        m_document->documentInfo().get(DocumentInfo::PagesSize);
    return pagesSize;
}

int BookMetadataHelper::getPageCount() const
{
    const QString& pages = m_document->documentInfo().get(DocumentInfo::Pages);

    bool ok = false;
    int pagesAsInt = pages.toInt(&ok);

    if(ok)
        return pagesAsInt;

    return 0;
}

void BookMetadataHelper::loadCover() const
{
    connect(m_observer.get(), &CoverObserver::pageLoaded, this,
            &BookMetadataHelper::proccessCoverPixmap);

    auto coverSize = getCoverSize();
    auto request =
        new PixmapRequest(m_observer.get(), 0, coverSize.width(),
                          coverSize.height(), 1, 1, PixmapRequest::NoFeature);

    m_document->requestPixmaps({ request });
}

QSize BookMetadataHelper::getCoverSize() const
{
    const auto& coverPage = m_document->page(0);

    QSize size;
    if(Book::maxCoverWidth * coverPage->ratio() <= Book::maxCoverHeight)
    {
        size.setHeight(Book::maxCoverWidth * coverPage->ratio());
        size.setWidth(Book::maxCoverWidth);
    }
    else
    {
        size.setHeight(Book::maxCoverHeight);
        size.setWidth(Book::maxCoverHeight / coverPage->ratio());
    }

    return size;
}

QString BookMetadataHelper::getSystemRelativePath(const QString& qPath) const
{
    QString prefix = "file://";
    return qPath.mid(prefix.size());
}

QMimeType BookMetadataHelper::getMimeType(const QString& filePath)
{
    QMimeDatabase mimeDb;
    auto mimeType = mimeDb.mimeTypeForUrl(filePath);

    return mimeType;
}

QString BookMetadataHelper::parseTitleFromPath(const QString& path) const
{
    auto indexOfLastSlash = path.lastIndexOf("/");
    auto indexOfLastDot = path.lastIndexOf(".");

    if(indexOfLastDot == -1)
        return path.mid(indexOfLastSlash + 1);

    auto result =
        path.mid(indexOfLastSlash + 1, indexOfLastDot - indexOfLastSlash - 1);

    return result;
}

QString BookMetadataHelper::removeTypeFromMimeString(
    const QString& mimeString) const
{
    int lastPositionOfSlash = mimeString.lastIndexOf("/");
    if(lastPositionOfSlash == -1)
        return mimeString;

    auto result = mimeString.mid(lastPositionOfSlash + 1);
    return result;
}

QString BookMetadataHelper::removeAppendingsFromMimeString(
    const QString& mimeString) const
{
    int lastPositionOfPlus = mimeString.lastIndexOf("+");
    if(lastPositionOfPlus == -1)
        return mimeString;

    auto result = mimeString.mid(0, lastPositionOfPlus);
    return result;
}

void BookMetadataHelper::proccessCoverPixmap(int page, int flag)
{
    // The cover is just the first page of the book
    int firstPage = 0;
    if(page != firstPage || flag != DocumentObserver::Pixmap)
        return;

    auto coverPixmap = m_document->page(0)->getPixmap(
        m_observer.get(), Book::maxCoverWidth, Book::maxCoverHeight);

    if(coverPixmap)
        emit bookCoverGenerated(coverPixmap);
}

}  // namespace application::utility