#pragma once
#include <QImage>
#include <QString>

namespace domain::value_objects
{

struct FreeBook
{
    int id;
    QString title;
    QString authors;
    QString languages;
    QString mediaType;
    QString formats;
    QImage cover;
    long downloadCount;
};

}  // namespace domain::value_objects
