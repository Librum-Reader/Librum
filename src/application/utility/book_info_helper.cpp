#include "book_info_helper.hpp"


namespace application::utility
{

QString BookInfoManager::parseBookTitleFromFilePath(const QString& filePath)
{
    auto lastIndexOfSlash = filePath.lastIndexOf("/");
    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
    auto result = filePath.mid(lastIndexOfSlash + 1, lastIndexOfDot - lastIndexOfSlash - 1);
    return result;
}

QByteArray BookInfoManager::getBookCover(const QString& filePath)
{
    return QByteArray("Smth");
}

} // namespace application::utility