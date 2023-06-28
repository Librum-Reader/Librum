#pragma once

namespace domain::entities
{

struct FreeBook
{
    QString title;
    QString authors;
    QString languages;
    QString mediaType;
    QString formats;
    long downloadCount;
};

}  // namespace domain::entities
