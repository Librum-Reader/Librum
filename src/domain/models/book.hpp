#pragma once
#include <vector>
#include <QObject>
#include <QImage>
#include <QString>
#include <QUuid>
#include <QJsonObject>
#include "tag.hpp"
#include "book_meta_data.hpp"


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
    
    const QString& getAuthor() const;
    void setAuthor(const QString& newAuthor);
    
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
    
    const QString& getAddedToLibrary() const;
    void setAddedToLibrary(const QString& newAddedToLibrary);
    
    const QString& getLastOpened() const;
    void setLastOpened(const QString& newLastOpened);
    
    const QString& getFilePath() const;
    void setFilePath(const QString& newLocalPath);
    
    QImage getCover() const;
    QString getCoverAsString() const;
    QString getCoverAsStringWithType() const;
    void setCover(const QImage& newCover);
    
    bool getDownloaded() const;
    void setDownloaded(bool newDownloaded);
    
    
    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    const std::vector<Tag>& getTags() const;
    
    void update(const Book& other);
    QByteArray toJson() const;
    static Book fromJson(const QJsonObject& jsonObject);
    
    
    static const int maxCoverWidth{ 188 };
    static const int maxCoverHeight{ 238 };
    
    
private:
    bool tagsAreTheSame(const std::vector<Tag>& other);
    
    QUuid m_uuid;
    BookMetaData m_metaData;
    QString m_filePath;
    bool m_downloaded = true;
    int m_currentPage;
    std::vector<Tag> m_tags;
};

} // namespace domain::models