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
    QString formats;
    QImage cover;
    long downloadCount;
    QString downloadLink;
};

}  // namespace domain::value_objects
