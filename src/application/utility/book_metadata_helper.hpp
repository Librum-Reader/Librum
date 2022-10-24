#pragma once
#include "i_book_metadata_helper.hpp"
#include <QMimeType>
#include <memory>
#include <QSize>
#include "document.h"
#include "cover_observer.hpp"


namespace application::utility
{

class BookMetadataHelper : public IBookMetadataHelper
{
    Q_OBJECT
    
public:
    std::optional<domain::models::BookMetaData> getBookMetaData(
            const QString& filePath) override;
    
private slots:
    void proccessBookCoverPixmap(int page, int flag);
    void getCover() const override;
    
private:
    bool setupDocument(const QString& filePath);
    void setupObserver();
    
    QString getTitle(const QString& filePath) const;
    QString getAuthor() const;
    QString getCreator() const;
    QString getCreationDate() const;
    QString getFormat() const;
    QString getDocumentSize() const;
    QString getPagesSize() const;
    int getPageCount() const;
    
    QSize getCoverSize() const;
    QString getSystemRelativePath(const QString& qPath) const;
    QString parseTitleFromPath(const QString& path) const;
    QString getCurrentDateTimeAsString();
    QMimeType getMimeType(const QString& filePath);
    QString removeTypeFromMimeString(const QString& mimeString) const;
    QString removeAppendingsFromMimeString(const QString& mimeString) const;
    
    std::unique_ptr<Okular::Document> m_document;
    std::unique_ptr<CoverObserver> m_observer;
};

} // namespace application::utility