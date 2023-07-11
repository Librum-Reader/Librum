#pragma once
#include <memory>
#include "document.hpp"
#include "i_book_metadata_helper.hpp"

namespace application::utility
{

class BookMetadataHelper : public IBookMetadataHelper
{
public:
    bool setup(const QString& filePath) override;
    domain::value_objects::BookMetaData getBookMetaData() override;
    QImage getBookCover() override;

private:
    std::unique_ptr<core::Document> m_document;
};

}  // namespace application::utility