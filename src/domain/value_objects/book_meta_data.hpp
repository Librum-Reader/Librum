#pragma once
#include <QDateTime>
#include <QImage>
#include <QString>
#include "domain_export.hpp"

namespace domain::value_objects
{

struct DOMAIN_EXPORT BookMetaData
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
    QDateTime coverLastModified;
    bool hasCover = false;
    double bookMediaDownloadProgress = 0.0;
    QString coverPath;
    QString colorTheme;
    QString fileHash;
    int zoom;
    float yOffset;

    bool operator==(const BookMetaData& rhs) const
    {
        // 'lastOpened' is equals if both are invalid or the same
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
               coverLastModified.toSecsSinceEpoch() ==
                   rhs.coverLastModified.toSecsSinceEpoch() &&
               hasCover == rhs.hasCover && coverPath == coverPath &&
               colorTheme == rhs.colorTheme && fileHash == rhs.fileHash &&
               zoom == rhs.zoom && yOffset == rhs.yOffset;
    }
};

}  // namespace domain::value_objects
