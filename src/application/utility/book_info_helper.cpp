#include "book_info_helper.hpp"
#include <memory>
#include <QMimeDatabase>
#include <QDebug>
#include "observer.h"
#include "page.h"
#include "settings.hpp"


namespace application::utility
{

BookInfoHelper::BookInfoHelper()
    : m_observer(std::make_unique<TempObserver>())
{
    Okular::Settings::instance(QStringLiteral("okularproviderrc"));
    m_currentDocument = std::make_unique<Okular::Document>(nullptr);
    
    QObject::connect(m_observer.get(), &TempObserver::pageChanged, [this] (int a, int b) {
        if(a == 0 && b == Okular::DocumentObserver::Pixmap)
            bookCoverPixmapReady();
    });
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
    QString prefix = "file://";
    QString systemRelativePath = filePath.mid(prefix.size());
    
    QMimeDatabase db;
    auto mimeType = db.mimeTypeForUrl(filePath);
    
    m_currentDocument->closeDocument();
    auto result = m_currentDocument->openDocument(systemRelativePath, 
                                                  filePath, mimeType, "");
    if(result != Okular::Document::OpenSuccess)
        emit gettingBookCoverFailed();
    
    m_currentDocument->addObserver(m_observer.get());
    
    auto request = new Okular::PixmapRequest(m_observer.get(), 0, m_coverWidth, m_coverHeight, 1, 1, Okular::PixmapRequest::NoFeature);
    QList<Okular::PixmapRequest*> pixmapRequests;
    pixmapRequests.push_back(request);
    
    m_currentDocument->requestPixmaps(pixmapRequests);
}

void BookInfoHelper::bookCoverPixmapReady()
{
    const QPixmap* coverPixmap = m_currentDocument->page(0)->getPixmap(m_observer.get(), m_coverWidth, m_coverHeight);
    
    if(m_currentDocument->page(0)->hasPixmap(m_observer.get(), m_coverWidth, m_coverHeight))
        emit bookCoverGenerated(coverPixmap);
}

void TempObserver::notifyPageChanged(int page, int flags)
{
    emit pageChanged(page, flags);
}

} // namespace application::utility