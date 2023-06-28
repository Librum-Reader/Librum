#pragma once
#include <QImage>
#include <QObject>
#include "free_book.hpp"

namespace application
{

class IFreeBooksStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageGateway() noexcept = default;

    virtual void getBooksMetadata() = 0;

signals:
    void gettingBooksMetaDataFinished(
        std::vector<domain::value_objects::FreeBook>& books);
    void gettingBookCoverFinished(int bookId, const QImage& cover);
};

}  // namespace application
