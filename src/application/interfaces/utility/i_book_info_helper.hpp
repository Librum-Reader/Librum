#pragma once
#include <QString>


namespace application
{

class IBookInfoHelper
{
public:
    virtual ~IBookInfoHelper() noexcept = default;
    
    virtual QString parseBookTitleFromFilePath(const QString& filePath) = 0;
    virtual QByteArray getBookCover(const QString& filePath) = 0;
};

} // namespace application