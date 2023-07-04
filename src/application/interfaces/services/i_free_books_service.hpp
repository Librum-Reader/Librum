#pragma once
#include <QObject>

namespace application
{

class IFreeBooksService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksService() noexcept = default;

    virtual void getBooksMetadata(const QString& author,
                                  const QString& title) = 0;
    virtual void getBookMedia(const QString& url) = 0;

signals:
    void gettingBookFinished(const QString& filePath);
};

}  // namespace application
