#pragma once
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QList>
#include "tag.hpp"


namespace domain::models
{

class Book
{
public:
    Book(QString title);
    
    const QString& title();
    void setTitle(const QString& newTitle);
    
    const QString& localPath() const;
    void setLocalPath(const QString& newLocalPath);
    
    const QByteArray& cover() const;
    void setCover(const QByteArray& newCover);
    
    const QList<Tag>& tags();
    bool addTag(const Tag& tag);
    bool removeTag(const Tag& tag);
    
    
private:
    QString m_title;
    QString m_localPath;
    QByteArray m_cover;
    QList<Tag> m_tags;
};

} // namespace domain::models