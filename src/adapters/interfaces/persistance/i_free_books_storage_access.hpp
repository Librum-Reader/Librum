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
    virtual void getCoverForBook(int bookId, const QString& coverUrl) = 0;
    virtual void getBookMedia(const QString& url) = 0;

signals:
    void gettingBooksMetadataFinished(const QByteArray& data);
    void gettingBookCoverFinished(int bookId, const QByteArray& data);
    void gettingBookMediaFinished(const QByteArray& data);
};

}  // namespace adapters
