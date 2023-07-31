#pragma once
#include <QImage>
#include <QObject>
#include "free_book.hpp"

namespace application
{

class IFreeBooksStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksStorageGateway() noexcept = default;

    virtual void getBooksMetadata(const QString& author,
                                  const QString& title) = 0;
    virtual void getBookMedia(const QString& url, const QUuid& uuid) = 0;

signals:
    void gettingBooksMetaDataFinished(
        std::vector<domain::value_objects::FreeBook>& books);
    void gettingBookCoverFinished(int bookId, const QImage& cover);
    void gettingBookMediaChunkReady(const QByteArray& data,
                                    const bool isChunkLast, const QUuid& uuid,
                                    const QString& format);
};

}  // namespace application
