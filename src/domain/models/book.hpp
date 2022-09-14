#pragma once
#include <vector>
#include <QObject>
#include <QString>
#include <QByteArray>
#include "tag.hpp"


namespace domain::models
{

class Book
{
public:
    Book(QString title, QString localPath = "", QByteArray cover = QByteArray());
    
    const QString& title() const;
    void setTitle(const QString& newTitle);
    
    const QString& filePath() const;
    void setFilePath(const QString& newLocalPath);
    
    const QByteArray& cover() const;
    void setCover(const QByteArray& newCover);
    
    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    const std::vector<Tag>& tags() const;
    
    void update(const Book& other);
    
private:
    bool tagsAreTheSame(const std::vector<Tag>& other);
    
    QString m_title;
    QString m_filePath;
    QByteArray m_cover;
    std::vector<Tag> m_tags;
};

} // namespace domain::models