#pragma once
#include <i_book_info_helper.hpp>
#include "document.h"


namespace application::utility
{

class BookInfoHelper : public IBookInfoHelper
{
public:
    QString parseBookTitleFromFilePath(const QString& filePath) override;
    void getBookCover(const QString& filePath) override;
    
private:
    std::unique_ptr<Okular::Document> m_currentDocument;
};

} // namespace application::utility