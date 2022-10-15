#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QImage>
#include <QVariant>
#include <QBuffer>
#include <QVariant>
#include <QUuid>
#include "tag_dto.hpp"


namespace adapters::dtos
{

struct BookDto
{
    Q_GADGET
    Q_PROPERTY(QString uuid MEMBER uuid)
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString author MEMBER author)
    Q_PROPERTY(QString filePath MEMBER filePath)
    Q_PROPERTY(QString creator MEMBER creator)
    Q_PROPERTY(QString creationDate MEMBER creationDate)
    Q_PROPERTY(QString format MEMBER format)
    Q_PROPERTY(QString language MEMBER language)
    Q_PROPERTY(QString documentSize MEMBER documentSize)
    Q_PROPERTY(QString pagesSize MEMBER pagesSize)
    Q_PROPERTY(int pageCount MEMBER pageCount)
    Q_PROPERTY(QString addedToLibrary MEMBER addedToLibrary)
    Q_PROPERTY(QString lastOpened MEMBER lastOpened)
    Q_PROPERTY(QVariant cover READ getCover CONSTANT)
    
public:
    QString uuid;
    QString title;
    QString author;
    QString filePath;
    QString creator;
    QString creationDate;
    QString format;
    QString language;
    QString documentSize;
    QString pagesSize;
    int pageCount;
    QString addedToLibrary;
    QString lastOpened;
    QImage cover;
    QList<TagDto> tags;
    
private:
    QString getCover()
    {
        if(cover.isNull())
            return QString("");
        
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        cover.save(&buffer, "png");
        QString base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/jpg;base64,") + base64;
    }
};

} // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::BookDto)