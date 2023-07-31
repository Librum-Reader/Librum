#pragma once
#include <QObject>

namespace adapters
{

class IFreeBooksStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageAccess() noexcept = default;

    virtual void getBooksMetadata(const QString& author,
                                  const QString& title) = 0;
    virtual void getCoverForBook(int bookId, const QString& coverUrl) = 0;
    virtual void getBookMedia(const QString& url, const QUuid& uuid) = 0;

signals:
    void gettingBooksMetadataFinished(const QByteArray& data);
    void gettingBookCoverFinished(int bookId, const QByteArray& data);
    void gettingBookMediaChunkReady(const QByteArray& data,
                                    const bool isChunkLast, const QUuid& uuid,
                                    const QString& format);
};

}  // namespace adapters
