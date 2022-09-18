#pragma once
#include <i_book_info_manager.hpp>


namespace application::utility
{

class BookInfoManager : public IBookInfoManager
{
public:
    QString getBookTitle(const QString& filePath) override;
    QByteArray getBookCover(const QString& filePath) override;
};

} // namespace application::utility