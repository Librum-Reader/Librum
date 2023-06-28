#pragma once
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
};

}  // namespace application
