#pragma once
#include <QString>


namespace application
{

class IBookInfoManager
{
public:
    virtual ~IBookInfoManager() noexcept = default;
    
    virtual QString getBookTitle(const QString& filePath) = 0;
    virtual QByteArray getBookCover(const QString& filePath) = 0;
};

} // namespace application