#pragma once
#include <QObject>

namespace application
{

class IFreeBooksStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageGateway() noexcept = default;

    virtual void getBooksMetadata() = 0;
};

}  // namespace application
