#pragma once
#include <QString>
#include <QImage>


namespace domain::models
{

struct BookMetaData
{
    QString title;
    QString author;
    QString creator;
    QString releaseDate;
    QString format;
    QString language;
    QString documentSize;
    QString pagesSize;
    int pageCount;
    QString addedToLibrary;
    QString lastOpened;
    QImage cover;
};

} // namespace domain::models