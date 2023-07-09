#pragma once
#include <QMimeType>
#include <QSize>
#include <memory>
#include "i_book_metadata_helper.hpp"

namespace application::utility
{

class BookMetadataHelper : public IBookMetadataHelper
{
    Q_OBJECT

public:
    std::optional<domain::value_objects::BookMetaData> getBookMetaData(
        const QString& filePath) override;

private slots:
    void proccessCoverPixmap(int page, int flag);
    void loadCover() const override;

private:
    bool setupDocument(const QString& filePath);
    void setupDocumentObserver();

    QString getTitle(const QString& filePath) const;
    QString getAuthors() const;
    QString getCreator() const;
    QString getCreationDate() const;
    QString getFormat() const;
    QString getDocumentSize() const;
    QString getPagesSize() const;
    int getPageCount() const;

    QSize getCoverSize() const;
    QString getSystemRelativePath(const QString& qPath) const;
    QString getTitleFromBookPath(const QString& path) const;
    QMimeType getMimeType(const QString& filePath);
    QString getFileTypeFromPath(const QString& mimeString) const;
    QString removeSuffixFromMimeString(const QString& mimeString) const;
};

}  // namespace application::utility