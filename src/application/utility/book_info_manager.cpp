#include "book_info_manager.hpp"


namespace application::utility
{

BookInfoManager::BookInfoManager()
{
    
}

QString BookInfoManager::getBookTitle(const QString& filePath)
{
    auto lastIndexOfSlash = filePath.lastIndexOf("/");
    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
    auto result = filePath.mid(lastIndexOfSlash + 1, lastIndexOfDot - lastIndexOfSlash);
    return result;
}

} // namespace application::utility