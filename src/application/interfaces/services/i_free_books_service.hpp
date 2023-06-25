#pragma once
#include <QObject>

namespace application
{

class IFreeBooksService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksService() noexcept = default;

    virtual void getBooksMetadata() = 0;
};

}  // namespace application
