#pragma once
#include <QImage>
#include <QObject>
#include <QString>
#include <optional>
#include "book_meta_data.hpp"
#include "application_export.hpp"

namespace application
{

/**
 * The BookMetaDataHelper class extracts metadata from a book.
 */
class APPLICATION_LIBRARY IBookMetadataHelper
{
public:
    virtual ~IBookMetadataHelper() noexcept = default;

    virtual bool setup(const QString& filePath) = 0;
    virtual domain::value_objects::BookMetaData getBookMetaData() = 0;
    virtual QImage getBookCover() = 0;
};

}  // namespace application
