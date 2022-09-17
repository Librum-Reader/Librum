#pragma once
#include <i_book_info_manager.hpp>


namespace application::utility
{

class BookInfoManager : public IBookInfoManager
{
public:
    BookInfoManager();
    
public:
    QString getBookTitle(const QString& filePath) override;
};

} // namespace application::utility