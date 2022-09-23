#include "book_info_manager.hpp"


namespace application::utility
{

QString BookInfoManager::getBookTitle(const QString& filePath)
{
//    auto lastIndexOfSlash = filePath.lastIndexOf("/");
//    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
//    auto result = filePath.mid(lastIndexOfSlash + 1, lastIndexOfDot - lastIndexOfSlash);
    return /*result*/filePath;
}

QByteArray BookInfoManager::getBookCover(const QString& filePath)
{
    return QByteArray("Smth");
}

} // namespace application::utility