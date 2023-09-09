#pragma once
#include <QObject>

namespace adapters
{

class IFreeBooksStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageAccess() noexcept = default;

    virtual void fetchFirstBooksMetadataPageWithFilter(
        const QString& author, const QString& title) = 0;
    virtual void fetchBooksMetadataPage(const QString& url) = 0;
    virtual void getBookCover(int id, const QString& url) = 0;
    virtual void getBookMedia(const int id, const QUuid& uuid,
                              const QString& url) = 0;

signals:
    void fetchingFirstMetadataPageSuccessful(const bool success);
    void fetchingBooksMetaDataFinished(const QByteArray& data);
    void gettingBookCoverFinished(int id, const QByteArray& data);
    void gettingBookMediaChunkReady(const QByteArray& data,
                                    const bool isChunkLast, const QUuid& uuid,
                                    const QString& format);
    void gettingBookMediaProgressChanged(const int id, qint64 bytesReceived,
                                         qint64 bytesTotal);
};

}  // namespace adapters
