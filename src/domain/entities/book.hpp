#pragma once
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>
#include "book_meta_data.hpp"
#include "bookmark.hpp"
#include "domain_export.hpp"
#include "highlight.hpp"
#include "tag.hpp"

namespace domain::entities
{

class DOMAIN_EXPORT Book
{
public:
    Book(const QString& filePath, const value_objects::BookMetaData& metaData,
         int currentPage = 0, const QString& uuid = "");

    bool operator==(const Book& rhs) const;

    const QUuid& getUuid() const;

    int getProjectGutenbergId() const;
    void setProjectGutenbergId(int newProjectGutenbergId);
    bool isFromProjectGutenberg() const;

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

    double getMediaDownloadProgress() const;
    void setMediaDownloadProgress(double newProgress);

    QString getColorTheme() const;
    void setColorTheme(const QString& newColorTheme);

    int getZoom() const;
    void setZoom(int newZoom);

    float getYOffset() const;
    void setYOffset(float newYOffset);

    QString getFileHash() const;
    void setFileHash(const QString& newFileHash);

    const QList<Highlight>& getHighlights() const;
    void setHighlights(QList<Highlight>&& highlights);
    void addHighlight(const Highlight& highlight);
    void changeHighlightColor(const QUuid& uuid, const QColor& newColor);
    void removeHighlight(QUuid uuid);

    const QList<Bookmark>& getBookmarks() const;
    void setBookmarks(QList<Bookmark>&& bookmarks);
    void addBookmark(const Bookmark& bookmark);
    void renameBookmark(const QUuid& uuid, const QString& newName);
    void removeBookmark(QUuid uuid);

    const QDateTime& getCoverLastModified() const;
    void updateCoverLastModified();
    void setCoverLastModified(const QDateTime& newTime);

    bool isDownloaded() const;
    void setDownloaded(bool newDownloaded);

    bool existsOnlyOnClient() const;
    void setExistsOnlyOnClient(bool newExistsOnlyOnClient);

    int getBookReadingProgress() const;

    domain::entities::Tag* getTagByUuid(const QUuid& uuid);
    domain::entities::Tag* getTagByName(const QString& name);
    bool addTag(const Tag& tag);
    bool removeTag(const QUuid& uuid);
    bool renameTag(const QUuid& uuid, const QString& newName);
    const QList<Tag>& getTags() const;

    void update(const Book& other);
    bool isValid() const;
    long getSizeInBytes() const;
    QByteArray toJson() const;
    static Book fromJson(const QJsonObject& jsonBook);

    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };

    inline static const QString dateTimeStringFormat {
        "hh:mm:ss - dd.MM.yyyy"
    };

private:
    bool tagsAreTheSame(const QList<Tag>& other) const;
    bool highlightsAreTheSame(const QList<Highlight>& other) const;
    bool bookmarksAreTheSame(const QList<Bookmark>& other) const;
    QJsonArray serializeTags() const;
    QJsonArray serializeHighlights() const;
    QJsonArray serializeBookmarks() const;
    static value_objects::BookMetaData getBookMetaDataFromJson(
        const QJsonObject& jsonBook);
    static void addTagsToBook(Book& book, const QJsonArray& jsonTags);
    static void addHighlightsToBook(Book& book,
                                    const QJsonArray& jsonHighlights);
    static void addBookmarksToBook(Book& book, const QJsonArray& jsonBookmarks);
    long getBytesFromSizeString(QString size) const;
    long getCoverSizeInBytes() const;
    QPair<long, QString> splitSizeStringInNumbersAndFormat(
        const QString& sizeString) const;

    QUuid m_uuid;
    int m_projectGutenbergId = 0;
    value_objects::BookMetaData m_metaData;
    QString m_filePath;
    bool m_isDownloaded = true;
    bool m_existsOnlyOnClient = true;
    int m_currentPage = 0;
    QList<Tag> m_tags;
    QList<Highlight> m_highlights;
    QList<Bookmark> m_bookmarks;
};

}  // namespace domain::entities
