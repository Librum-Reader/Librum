#pragma once
#include <QImage>
#include <QObject>
#include "application_export.hpp"
#include "free_book.hpp"

namespace application
{

class APPLICATION_EXPORT IFreeBooksStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageGateway() noexcept = default;

    virtual void fetchFirstBooksMetadataPageWithFilter(
        const QString& authorsAndTitle) = 0;
    virtual void fetchBooksMetadataPage(const QString& url) = 0;
    virtual void getBookCover(const int id, const QString& url) = 0;
    virtual void getBookMedia(const int id, const QUuid& uuid,
                              const QString& url) = 0;

signals:
    void fetchingFirstMetadataPageSuccessful(const bool success);
    void fetchingBooksMetaDataFinished(
        std::vector<domain::value_objects::FreeBook>& books,
        const int booksTotalCount, const QString& nextMetadataPageUrl,
        const QString& prevMetadataPageUrl);
    void gettingBookCoverFinished(int id, const QImage& cover);
    void gettingBookMediaChunkReady(int id, const QUuid& uuid,
                                    const QByteArray& data,
                                    const QString& format, bool isChunkLast);
    void gettingBookMediaProgressChanged(const int id, qint64 bytesReceived,
                                         qint64 bytesTotal);
};

}  // namespace application
