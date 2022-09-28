#include "book_info_helper.hpp"


namespace application::utility
{

QString BookInfoHelper::parseBookTitleFromFilePath(const QString& filePath)
{
    auto lastIndexOfSlash = filePath.lastIndexOf("/");
    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
    if(lastIndexOfDot == -1)
        return filePath.mid(lastIndexOfSlash + 1);
    
    auto result = filePath.mid(lastIndexOfSlash + 1, 
                               lastIndexOfDot - lastIndexOfSlash - 1);
    return result;
}

QByteArray BookInfoHelper::getBookCover(const QString& filePath)
{
    return QByteArray("Smth");
}

} // namespace application::utility