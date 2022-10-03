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
         const QString& localPath, const QImage& cover = QImage());
    
    const QString& title() const;
    void setTitle(const QString& newTitle);
    
    const QString& author() const;
    void setAuthor(const QString& newAuthor);
    
    const QString& filePath() const;
    void setFilePath(const QString& newLocalPath);
    
    const QImage& cover() const;
    void setCover(const QImage& newCover);
    
    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    const std::vector<Tag>& tags() const;
    
    void update(const Book& other);
    
private:
    bool tagsAreTheSame(const std::vector<Tag>& other);
    
    QString m_title;
    QString m_author;
    QString m_filePath;
    QImage m_cover;
    std::vector<Tag> m_tags;
};

} // namespace domain::models