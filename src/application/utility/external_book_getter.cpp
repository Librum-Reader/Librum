#include "external_book_getter.hpp"
#include <QDebug>
#include "metadata_extractor.hpp"

using domain::entities::Book;

namespace application::utility
{

ExternalBookGetter::ExternalBookGetter(const QString& filePath)
{
    core::MetadataExtractor metadataExtractor;
    m_isValid = metadataExtractor.setup(filePath);
    if(!m_isValid)
    {
        qWarning() << QString("Failed setting up external book with path: %1 ")
                          .arg(filePath);
    }

    auto metaData = metadataExtractor.getBookMetaData();
    m_externalBook = std::make_unique<Book>(filePath, metaData);
}

domain::entities::Book* ExternalBookGetter::getBook()
{
    return m_externalBook.get();
}

void ExternalBookGetter::setUuid(const QUuid& uuid)
{
    // This is an external book, we can't set the uuid.
    Q_UNUSED(uuid);
}

void ExternalBookGetter::updateBook(Book* book)
{
    // This is an external book, we can't update it.
    Q_UNUSED(book);
}

bool ExternalBookGetter::bookIsValid()
{
    return m_isValid;
}


}  // namespace application::utility