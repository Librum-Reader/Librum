#include "book_metadata_helper.hpp"
#include <QDateTime>
#include <QDebug>
#include <QMimeDatabase>
#include "document.hpp"


using namespace domain::value_objects;

namespace application::utility
{


std::optional<BookMetaData> BookMetadataHelper::getBookMetaData(
    const QString& filePath)
{
    auto document = core::Document(filePath);

    BookMetaData metaData {
        .title = document.getTitle(),
        .authors = document.getAuthors(),
        .creator = document.getCreator(),
        .creationDate = document.getCreationDate(),
        .format = document.getFormat(),
        .language = "",
        .documentSize = "",
        .pagesSize = "",
        .pageCount = document.getPageCount(),
        .lastOpened = QDateTime(),
    };

    return metaData;
}

}  // namespace application::utility