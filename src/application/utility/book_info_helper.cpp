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
    m_currentDocument = std::make_unique<Document>(nullptr);
    m_currentDocument->addObserver(m_observer.get());
    
    QObject::connect(m_observer.get(), &CoverObserver::pageChanged, 
                     this, &BookInfoHelper::proccessBookCoverPixmap);
}


bool BookInfoHelper::setupDocument(const QString& filePath)
{
    QMimeDatabase mimeDb;
    m_systemRelativePath = getSystemRelativePath(filePath);
    m_mimeType = mimeDb.mimeTypeForUrl(filePath);
    
    m_currentDocument->closeDocument();
    auto result = m_currentDocument->openDocument(m_systemRelativePath, filePath,
                                                  m_mimeType, "");
    
    if(result != Document::OpenSuccess)
        return false;
    
    return true;
}

QString BookInfoHelper::getTitle()
{
    auto lastIndexOfSlash = m_systemRelativePath.lastIndexOf("/");
    auto lastIndexOfDot = m_systemRelativePath.lastIndexOf(".");
    
    if(lastIndexOfDot == -1)
        return m_systemRelativePath.mid(lastIndexOfSlash + 1);
    
    auto result = m_systemRelativePath.mid(lastIndexOfSlash + 1, 
                                           lastIndexOfDot - lastIndexOfSlash - 1);
    return result;
}

void BookInfoHelper::getCover()
{
    QSize coverSize = getCoverSize();
    auto request = new PixmapRequest(m_observer.get(), 0, coverSize.width(),
                                     coverSize.height(), 1, 1, PixmapRequest::NoFeature);
    
    m_currentDocument->requestPixmaps({request});
}

QString BookInfoHelper::getAuthor()
{
    QString title = m_currentDocument->documentInfo().get(DocumentInfo::Author);
    return title;
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