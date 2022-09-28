#pragma once
#include <i_book_info_helper.hpp>


namespace application::utility
{

class BookInfoManager : public IBookInfoHelper
{
public:
    QString parseBookTitleFromFilePath(const QString& filePath) override;
    QByteArray getBookCover(const QString& filePath) override;
};

} // namespace application::utility