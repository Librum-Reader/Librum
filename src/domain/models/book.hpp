#pragma once
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "book_meta_data.hpp"
#include "tag.hpp"

namespace domain::models
{

class Book
{
public:
    Book(const QString& filePath, const BookMetaData& metaData,
         int currentPage = 0, const QString& uuid = "");

    bool operator==(const Book& rhs) const;


    const QUuid& getUuid() const;

    const QString& getTitle() const;
    void setTitle(const QString& newTitle);

    const QString& getAuthors() const;
    void setAuthors(const QString& newAuthor);

    const QString& getCreator() const;
    void setCreator(const QString& newCreator);

    int getPageCount() const;
    void setPageCount(int newPageCount);

    int getCurrentPage() const;
    void setCurrentPage(int newCurrentPage);

    const QString& getCreationDate() const;
    void setCreationDate(const QString& newCreationDate);

    const QString& getFormat() const;
    void setFormat(const QString& newFormat);

    const QString& getLanguage() const;
    void setLanguage(const QString& newLanguage);

    const QString& getDocumentSize() const;
    void setDocumentSize(const QString& newDocumentSize);

    const QString& getPagesSize() const;
    void setPagesSize(const QString& newPagesSize);

    const QDateTime& getAddedToLibrary() const;
    void setAddedToLibrary(const QDateTime& newAddedToLibrary);

    const QDateTime& getLastOpened() const;
    void setLastOpened(const QDateTime& newLastOpened);

    const QDateTime& getLastModified() const;
    void updateLastModified();

    const QString& getFilePath() const;
    void setFilePath(const QString& newLocalPath);

    QImage getCover() const;
    QString getCoverAsString() const;
    QString getCoverAsStringWithType() const;
    void setCover(const QImage& newCover);

    bool getDownloaded() const;
    void setDownloaded(bool newDownloaded);


    int getPercentageRead() const;

    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    const std::vector<Tag>& getTags() const;

    void update(const Book& other);
    QByteArray toJson() const;
    static Book fromJson(const QJsonObject& jsonObject);


    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };


    inline static const QString dateTimeStringFormat {
        "hh:mm:ss - dd.MM.yyyy"
    };

private:
    bool tagsAreTheSame(const std::vector<Tag>& other);

    QUuid m_uuid;
    BookMetaData m_metaData;
    QString m_filePath;
    bool m_downloaded = true;
    int m_currentPage = 0;
    std::vector<Tag> m_tags;
};

}  // namespace domain::models