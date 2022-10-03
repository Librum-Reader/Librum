#include "book_info_helper.hpp"
#include <memory>
#include <QSize>
#include <QMimeDatabase>
#include "document.h"
#include "generator.h"
#include "observer.h"
#include "page.h"
#include "settings.hpp"


using namespace Okular;


namespace application::utility
{

BookInfoHelper::BookInfoHelper()
    : m_observer(std::make_unique<CoverObserver>())
{
    Settings::instance(QStringLiteral("okularproviderrc"));
    m_document = std::make_unique<Document>(nullptr);
    m_document->addObserver(m_observer.get());
    
    QObject::connect(m_observer.get(), &CoverObserver::pageChanged, 
                     this, &BookInfoHelper::proccessBookCoverPixmap);
}


bool BookInfoHelper::setupDocument(const QString& filePath)
{
    m_document->closeDocument();
    
    QMimeDatabase mimeDb;
    m_systemRelativePath = getSystemRelativePath(filePath);
    m_mimeType = mimeDb.mimeTypeForUrl(filePath);
    
    auto result = m_document->openDocument(m_systemRelativePath, filePath,
                                           m_mimeType, "");
    
    return result == Document::OpenSuccess;
}

QString BookInfoHelper::getTitle() const
{
    const QString& title = m_document->documentInfo().get(DocumentInfo::Title);
    if(title.isEmpty())
    {
        return parseTitleFromPath(m_systemRelativePath);
    }
    
    return title;
}

void BookInfoHelper::getCover() const
{
    auto coverSize = getCoverSize();
    auto request = new PixmapRequest(m_observer.get(), 0, coverSize.width(),
                                     coverSize.height(), 1, 1, PixmapRequest::NoFeature);
    
    m_document->requestPixmaps({request});
}

QString BookInfoHelper::getAuthor() const
{
    const QString& author = m_document->documentInfo().get(DocumentInfo::Author);
    return author;
}

QSize BookInfoHelper::getCoverSize() const
{
    const auto& coverPage = m_document->page(0);
    
    QSize size;
    if(m_maxCoverWidth * coverPage->ratio() <= m_maxCoverHeight)
    {
        size.setHeight(m_maxCoverWidth * coverPage->ratio());
        size.setWidth(m_maxCoverWidth);
    }
    else
    {
        size.setHeight(m_maxCoverHeight);
        size.setWidth(m_maxCoverHeight / coverPage->ratio());
    }
    
    return size;
}

QString BookInfoHelper::getSystemRelativePath(const QString& qPath) const
{
    QString prefix = "file://";
    return qPath.mid(prefix.size());
}

QString BookInfoHelper::parseTitleFromPath(const QString& path) const
{
    auto indexOfLastSlash = path.lastIndexOf("/");
    auto indexOfLastDot = path.lastIndexOf(".");
    
    if(indexOfLastDot == -1)
        return m_systemRelativePath.mid(indexOfLastSlash + 1);
    
    auto result = m_systemRelativePath.mid(indexOfLastSlash + 1, 
                                           indexOfLastDot - indexOfLastSlash - 1);
    
    return result;
}

void BookInfoHelper::proccessBookCoverPixmap(int page, int flag)
{
    if(page != 0 || flag != DocumentObserver::Pixmap)
        return;
    
    auto coverPixmap = m_document->page(0)->getPixmap(m_observer.get(),
                                                      m_maxCoverWidth,
                                                      m_maxCoverHeight);
    
    if(coverPixmap)
        emit bookCoverGenerated(coverPixmap);
}

} // namespace application::utility