#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "i_free_books_storage_access.hpp"

namespace infrastructure::persistence
{

class FreeBooksStorageAccess : public adapters::IFreeBooksStorageAccess
{
    Q_OBJECT

public:
    void fetchFirstBooksMetadataPageWithFilter(
        const QString& authorsAndTitle) override;
    void fetchBooksMetadataPage(const QString& url) override;
    void getBookCover(int id, const QString& url) override;
    void getBookMedia(const int id, const QUuid& uuid,
                      const QString& url) override;

private:
    QNetworkAccessManager m_networkAccessManager;
    QString m_whitespaceCode = "%20";

    QNetworkRequest createGetBooksMetadataRequest(
        const QString& authorsAndTitle);
    QNetworkRequest createRequest(const QUrl& url);
};

}  // namespace infrastructure::persistence
