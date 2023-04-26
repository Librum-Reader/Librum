#pragma once
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "book_meta_data.hpp"
#include "tag.hpp"

namespace domain::entities
{

class Book
{
public:
    Book(const QString& filePath, const value_objects::BookMetaData& metaData,
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
    void updateLastOpened();

    const QDateTime& getLastModified() const;
    void setLastModified(const QDateTime& newLastModified);
    void updateLastModified();

    const QString& getFilePath() const;
    void setFilePath(const QString& newLocalPath);

    bool hasCover() const;
    void setHasCover(bool hasCover);

    const QString& getCoverPath() const;
    void setCoverPath(const QString& path);

    const QDateTime& getCoverLastModified() const;
    void updateCoverLastModified();
    void setCoverLastModified(const QDateTime& newTime);

    bool getDownloaded() const;
    void setDownloaded(bool newDownloaded);
    
    int getBookReadingProgress() const;


    domain::entities::Tag* getTagByUuid(const QUuid& uuid);
    domain::entities::Tag* getTagByName(const QString& name);
    bool addTag(const Tag& tag);
    bool removeTag(const QUuid& uuid);
    bool renameTag(const QUuid& uuid, const QString& newName);
    const std::vector<Tag>& getTags() const;

    void update(const Book& other);
    QByteArray toJson() const;
    static Book fromJson(const QJsonObject& jsonBook);


    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };

    inline static const QString dateTimeStringFormat {
        "hh:mm:ss - dd.MM.yyyy"
    };

private:
    bool tagsAreTheSame(const std::vector<Tag>& other) const;
    QJsonArray serializeTags() const;
    static value_objects::BookMetaData getBookMetaDataFromJson(
        const QJsonObject& jsonBook);
    static void addTagsToBook(Book& book, const QJsonArray& jsonTags);

    QUuid m_uuid;
    value_objects::BookMetaData m_metaData;
    QString m_filePath;
    bool m_downloaded = true;
    int m_currentPage = 0;
    std::vector<Tag> m_tags;
};

}  // namespace domain::entities