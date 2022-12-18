#pragma once
#include <QObject>
#include <QPixmap>
#include <QString>
#include <optional>
#include "book_meta_data.hpp"

namespace application
{

/**
 * The BookMetaDataHelper class extracts metadata from a book.
 */
class IBookMetadataHelper : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookMetadataHelper() noexcept = default;

    virtual std::optional<domain::value_objects::BookMetaData> getBookMetaData(
        const QString& filePath) = 0;

    virtual void loadCover() const = 0;

signals:
    void bookCoverGenerated(const QPixmap* pixmal);
    void gettingBookCoverFailed();
};

}  // namespace application