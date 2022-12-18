#pragma once
#include <QDateTime>
#include <QImage>
#include <QString>

namespace domain::value_objects
{

struct BookMetaData
{
    QString title;
    QString authors;
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
        // Handle case if both "lastOpened" are invalid
        bool lastOpenedIsSame =
            (!lastOpened.isValid() && !rhs.lastOpened.isValid()) ||
            lastOpened.toSecsSinceEpoch() == rhs.lastOpened.toSecsSinceEpoch();

        return title == rhs.title && authors == rhs.authors &&
               creator == rhs.creator && creationDate == rhs.creationDate &&
               format == rhs.format && language == rhs.language &&
               documentSize == rhs.documentSize && pagesSize == rhs.pagesSize &&
               pageCount == rhs.pageCount && lastOpenedIsSame &&
               addedToLibrary.toSecsSinceEpoch() ==
                   rhs.addedToLibrary.toSecsSinceEpoch() &&
               lastModified.toSecsSinceEpoch() ==
                   rhs.lastModified.toSecsSinceEpoch() &&
               cover == rhs.cover;
    }
};

}  // namespace domain::value_objects