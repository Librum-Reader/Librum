#pragma once
#include <QObject>

namespace adapters
{

class IFreeBooksStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageAccess() noexcept = default;

    virtual void getBooksMetadata() = 0;

signals:
    void gettingBooksMetadataFinished(const QByteArray& data);
};

}  // namespace adapters
