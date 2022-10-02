#include "book_info_helper.hpp"
#include <memory>
#include <QSize>
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
    m_currentDocument = std::make_unique<Document>(nullptr);
    m_currentDocument->addObserver(m_observer.get());
    
    QObject::connect(m_observer.get(), &CoverObserver::pageChanged, 
                     this, &BookInfoHelper::proccessBookCoverPixmap);
}


QString BookInfoHelper::parseBookTitleFromFilePath(const QString& filePath)
{
    auto lastIndexOfSlash = filePath.lastIndexOf("/");
    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
    if(lastIndexOfDot == -1)
        return filePath.mid(lastIndexOfSlash + 1);
    
    auto result = filePath.mid(lastIndexOfSlash + 1, 
                               lastIndexOfDot - lastIndexOfSlash - 1);
    return result;
}


void BookInfoHelper::getBookCover(const QString& filePath)
{
    auto systemPath = getSystemRelativePath(filePath);
    auto mimeType = m_mimeDb.mimeTypeForUrl(filePath);
    
    m_currentDocument->closeDocument();
    auto result = m_currentDocument->openDocument(systemPath, filePath,
                                                  mimeType, "");
    if(result != Document::OpenSuccess)
        emit gettingBookCoverFailed();
    
    
    QSize coverSize = getCoverSize();
    auto request = new PixmapRequest(m_observer.get(), 0, coverSize.width(),
                                     coverSize.height(), 1, 1, PixmapRequest::NoFeature);
    
    m_currentDocument->requestPixmaps({request});
}

QSize BookInfoHelper::getCoverSize()
{
    const auto& coverPage = m_currentDocument->page(0);
    
    QSize size;
    if(m_maxCoverWidth*coverPage->ratio() <= m_maxCoverHeight)
    {
        size.setHeight(m_maxCoverWidth*coverPage->ratio());
        size.setWidth(m_maxCoverWidth);
    }
    else
    {
        size.setHeight(m_maxCoverHeight);
        size.setWidth(m_maxCoverHeight/coverPage->ratio());
    }
    
    return size;
}


QString BookInfoHelper::getSystemRelativePath(const QString& qPath)
{
    QString prefix = "file://";
    return qPath.mid(prefix.size());
}

void BookInfoHelper::proccessBookCoverPixmap(int page, int flag)
{
    if(page != 0 || flag != DocumentObserver::Pixmap)
        return;
    
    const QPixmap* coverPixmap = m_currentDocument->page(0)->
                                 getPixmap(m_observer.get(), 100, 100);
    
    if(coverPixmap)
        emit bookCoverGenerated(coverPixmap);
}

} // namespace application::utility