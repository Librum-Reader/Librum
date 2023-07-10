#pragma once
#include <QMimeType>
#include <QSize>
#include <memory>
#include "i_book_metadata_helper.hpp"

namespace application::utility
{

class BookMetadataHelper : public IBookMetadataHelper
{
    Q_OBJECT

public:
    std::optional<domain::value_objects::BookMetaData> getBookMetaData(
        const QString& filePath) override;
};

}  // namespace application::utility