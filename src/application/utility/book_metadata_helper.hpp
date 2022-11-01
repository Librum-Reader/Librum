#pragma once
#include <QMimeType>
#include <QSize>
#include <memory>
#include "cover_observer.hpp"
#include "document.h"
#include "i_book_metadata_helper.hpp"

namespace application::utility
{

class BookMetadataHelper : public IBookMetadataHelper
{
    Q_OBJECT

public:
    std::optional<domain::models::BookMetaData> getBookMetaData(
        const QString& filePath) override;

private slots:
    void loadFirstPagePixmap(int page, int flag);
    void getCover() const override;

private:
    bool setupDocument(const QString& filePath);
    void setupDocumentObserver();

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
    QMimeType getMimeType(const QString& filePath);
    QString removeTypeFromMimeString(const QString& mimeString) const;
    QString removeAppendingsFromMimeString(const QString& mimeString) const;

    std::unique_ptr<Okular::Document> m_document;
    std::unique_ptr<CoverObserver> m_observer;
};

}  // namespace application::utility