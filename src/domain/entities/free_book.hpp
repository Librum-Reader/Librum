#pragma once
#include <QImage>
#include <QString>

namespace domain::value_objects
{

struct FreeBook
{
    int id = 0;
    QString title;
    QString authors;
    QString languages;
    QString formats;
    QImage cover;
    QString coverDownloadLink;
    long downloadCount = 0;
    QString mediaDownloadLink;
    double mediaDownloadProgress = 0.0;
};

}  // namespace domain::value_objects
