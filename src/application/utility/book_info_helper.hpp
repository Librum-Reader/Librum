#pragma once
#include "i_book_info_helper.hpp"
#include <QMimeType>
#include "document.h"
#include "cover_observer.hpp"


namespace application::utility
{

class BookInfoHelper : public IBookInfoHelper
{
    Q_OBJECT
    
public:
    BookInfoHelper();
    
    bool setupDocument(const QString& filePath) override;
    QString getTitle() const override;
    QString getAuthor() const override;
    QString getCreator() const override;
    int getPageCount() const override;
    QString getCreationDate() const override;
    QString getFormat() const override;
    QString getDocumentSize() const override;
    QString getPagesSize() const override;
    void getCover() const override;
    
private slots:
    void proccessBookCoverPixmap(int page, int flag);
    
private:
    QSize getCoverSize() const;
    QString getSystemRelativePath(const QString& qPath) const;
    QString parseTitleFromPath(const QString& path) const;
    
    std::unique_ptr<Okular::Document> m_document;
    std::unique_ptr<CoverObserver> m_observer;
    QString m_systemRelativePath;
    QMimeType m_mimeType;
    const int m_maxCoverWidth = 188;
    const int m_maxCoverHeight = 238;
};

} // namespace application::utility