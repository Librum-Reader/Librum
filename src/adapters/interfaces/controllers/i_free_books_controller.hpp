#pragma once
#include <QObject>

namespace adapters
{

class IFreeBooksController : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksController() noexcept = default;

    Q_INVOKABLE virtual void getBooksMetadata() = 0;
    Q_INVOKABLE virtual void getBookMedia(const QString& url) = 0;

signals:
    void gettingBookFinished(const QString& path);
};

}  // namespace adapters
