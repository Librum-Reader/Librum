#pragma once
#include "i_book_info_helper.hpp"
#include <QMimeDatabase>
#include "document.h"
#include "cover_observer.hpp"


namespace application::utility
{

class BookInfoHelper : public IBookInfoHelper
{
    Q_OBJECT
    
public:
    BookInfoHelper();
    
    QString parseBookTitleFromFilePath(const QString& filePath) override;
    void getBookCover(const QString& filePath) override;
    
private slots:
    void proccessBookCoverPixmap(int page, int flag);
    
private:
    QSize getCoverSize();
    QString getSystemRelativePath(const QString& qPath);
    
    std::unique_ptr<Okular::Document> m_currentDocument;
    std::unique_ptr<CoverObserver> m_observer;
    QMimeDatabase m_mimeDb;
    const int m_maxCoverWidth = 188;
    const int m_maxCoverHeight = 240;
};

} // namespace application::utility