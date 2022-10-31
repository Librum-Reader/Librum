#pragma once
#include <QDateTime>
#include <QImage>
#include <QString>

namespace domain::models
{

struct BookMetaData
{
    QString title;
    QString author;
    QString creator;
    QString creationDate;
    QString format;
    QString language;
    QString documentSize;
    QString pagesSize;
    int pageCount;
    QDateTime addedToLibrary = QDateTime::currentDateTimeUtc();
    QDateTime lastModified = QDateTime::currentDateTimeUtc();
    QDateTime lastOpened;
    QImage cover;

    bool operator==(const BookMetaData& rhs) const
    {
        return title == rhs.title && author == rhs.author &&
               creator == rhs.creator && creationDate == rhs.creationDate &&
               format == rhs.format && language == rhs.language &&
               documentSize == rhs.documentSize && pagesSize == rhs.pagesSize &&
               pageCount == rhs.pageCount &&
               addedToLibrary.currentSecsSinceEpoch() ==
                   rhs.addedToLibrary.currentSecsSinceEpoch() &&
               lastOpened.currentSecsSinceEpoch() ==
                   rhs.lastOpened.currentSecsSinceEpoch() &&
               lastModified.currentSecsSinceEpoch() ==
                   rhs.lastModified.currentSecsSinceEpoch() &&
               cover == rhs.cover;
    }
};

}  // namespace domain::models