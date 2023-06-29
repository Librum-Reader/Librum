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
    virtual void getBookMedia(const QString& url) = 0;

signals:
    void gettingBookFinished(const QString& filePath);
};

}  // namespace application
