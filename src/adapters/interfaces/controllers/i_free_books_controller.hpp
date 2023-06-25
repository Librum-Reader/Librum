#pragma once
#include <QObject>

namespace adapters
{

class IFreeBooksController : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksController() noexcept = default;

    virtual void getBooksMetadata() = 0;
};

}  // namespace adapters
