#pragma once
#include <i_book_info_helper.hpp>
#include "document.h"
#include "observer.h"
#include "generator.h"


namespace application::utility
{


class TempObserver : public QObject, public Okular::DocumentObserver
{
    Q_OBJECT

public:
    void notifyPageChanged(int page, int flags) override;

signals:
    void pageChanged(int page, int flags);

private:
    DocumentItem *m_document;
};


class BookInfoHelper : public IBookInfoHelper
{
    Q_OBJECT
    
public:
    BookInfoHelper();
    
    QString parseBookTitleFromFilePath(const QString& filePath) override;
    void getBookCover(const QString& filePath) override;
    
private slots:
    void bookCoverPixmapReady();
    
private:
    std::unique_ptr<Okular::Document> m_currentDocument;
    std::unique_ptr<TempObserver> m_observer;
    
    const int m_defaultCoverWidth = 188;
    const int m_defaultCoverHeight = 238;
    int m_coverWidth = 0;
    int m_coverHeight = 0;
};

} // namespace application::utility