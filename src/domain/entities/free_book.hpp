#pragma once

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
    long downloadCount;
};

}  // namespace domain::value_objects
