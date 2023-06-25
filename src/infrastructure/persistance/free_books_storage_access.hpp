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
    void getBooksMetadata() override;

private:
    QNetworkAccessManager m_networkAccessManager;

    QNetworkRequest createRequest(const QUrl& url);
};

}  // namespace infrastructure::persistence
