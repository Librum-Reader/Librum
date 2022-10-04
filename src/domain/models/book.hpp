#pragma once
#include <vector>
#include <QObject>
#include <QImage>
#include <QString>
#include "tag.hpp"


namespace domain::models
{

class Book
{
public:
    Book(const QString& title, const QString& author,
         const QString& filePath, const QImage& cover = QImage());
    
    Book(const QString& title, const QString& author,
         const QString& filePath, const QString& creator,
         const QString& creationDate, const QString& format,
         const QString& docSize, const QString& pagesSize,
         int pageCount, const QString& addedToLibrary,
         const QString& lastModified, const QImage& cover = QImage());
    
    const QString& getTitle() const;
    void setTitle(const QString& newTitle);
    
    const QString& getAuthor() const;
    void setAuthor(const QString& newAuthor);
    
    const QString& getCreator() const;
    void setCreator(const QString& newCreator);
    
    int getPageCount() const;
    void setPageCount(int newPageCount);
    
    const QString& getCreationDate() const;
    void setCreationDate(const QString& newCreationDate);
    
    const QString& getFormat() const;
    void setFormat(const QString& newFormat);
    
    const QString& getDocumentSize() const;
    void setDocumentSize(const QString& newDocumentSize);
    
    const QString& getPagesSize() const;
    void setPagesSize(const QString& newPagesSize);
    
    const QString& getAddedToLibrary() const;
    void setAddedToLibrary(const QString& newAddedToLibrary);
    
    const QString& getLastModified() const;
    void setLastModified(const QString& newLastModified);
    
    const QString& getFilePath() const;
    void setFilePath(const QString& newLocalPath);
    
    const QImage& getCover() const;
    void setCover(const QImage& newCover);
    
    
    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    const std::vector<Tag>& getTags() const;
    
    
    void update(const Book& other);
    
    
private:
    bool tagsAreTheSame(const std::vector<Tag>& other);
    
    QString m_title;
    QString m_author;
    QString m_filePath;
    QString m_creator;
    QString m_creationDate;
    QString m_format;
    QString m_documentSize;
    QString m_pagesSize;
    int m_pageCount;
    QString m_addedToLibrary;
    QString m_lastModified;
    QImage m_cover;
    std::vector<Tag> m_tags;
};

} // namespace domain::models