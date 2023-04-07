#pragma once
#include <QBuffer>
#include <QImage>
#include <QList>
#include <QObject>
#include <QString>
#include <QUuid>
#include <QVariant>
#include "tag_dto.hpp"

namespace adapters::dtos
{

struct BookDto
{
    Q_GADGET
    Q_PROPERTY(QString uuid MEMBER uuid)
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString authors MEMBER authors)
    Q_PROPERTY(QString filePath MEMBER filePath)
    Q_PROPERTY(QString creator MEMBER creator)
    Q_PROPERTY(QString creationDate MEMBER creationDate)
    Q_PROPERTY(QString format MEMBER format)
    Q_PROPERTY(QString language MEMBER language)
    Q_PROPERTY(QString documentSize MEMBER documentSize)
    Q_PROPERTY(QString pagesSize MEMBER pagesSize)
    Q_PROPERTY(int pageCount MEMBER pageCount)
    Q_PROPERTY(int currentPage MEMBER currentPage)
    Q_PROPERTY(int bookProgressPercentage MEMBER bookProgressPercentage)
    Q_PROPERTY(QString addedToLibrary MEMBER addedToLibrary)
    Q_PROPERTY(QString lastOpened MEMBER lastOpened)
    Q_PROPERTY(QString coverPath MEMBER coverPath)
    Q_PROPERTY(bool downloaded MEMBER downloaded)

public:
    QString uuid;
    QString title;
    QString authors;
    QString filePath;
    QString creator;
    QString creationDate;
    QString format;
    QString language;
    QString documentSize;
    QString pagesSize;
    int pageCount;
    int currentPage;
    int bookProgressPercentage;
    QString addedToLibrary;
    QString lastOpened;
    QString coverPath;
    bool downloaded;
    QList<TagDto> tags;
};

}  // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::BookDto)