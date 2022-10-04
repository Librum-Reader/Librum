#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QImage>
#include <QVariant>
#include "tag_dto.hpp"


namespace adapters::dtos
{

struct BookDto
{
    Q_GADGET
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString author MEMBER author)
    Q_PROPERTY(QString filePath MEMBER filePath)
    Q_PROPERTY(QString creator MEMBER creator)
    Q_PROPERTY(QString creationDate MEMBER creationDate)
    Q_PROPERTY(QString format MEMBER format)
    Q_PROPERTY(QString documentSize MEMBER documentSize)
    Q_PROPERTY(QString pagesSize MEMBER pagesSize)
    Q_PROPERTY(int pageCount MEMBER pageCount)
    Q_PROPERTY(QString addedToLibrary MEMBER addedToLibrary)
    Q_PROPERTY(QString lastModified MEMBER lastModified)
    Q_PROPERTY(QImage cover MEMBER cover)
    
public:
    QString title;
    QString author;
    QString filePath;
    QString creator;
    QString creationDate;
    QString format;
    QString documentSize;
    QString pagesSize;
    int pageCount;
    QString addedToLibrary;
    QString lastModified;
    QImage cover;
    QList<TagDto> tags;
};

} // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::BookDto)