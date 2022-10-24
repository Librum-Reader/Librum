#pragma once
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include "i_book_storage_access.hpp"


namespace infrastructure::persistence
{

class BookStorageAccess : public adapters::IBookStorageAccess
{
    Q_OBJECT
    
public:
    BookStorageAccess();
        
    void createBook(const QString& authToken, 
                    const adapters::dtos::BookDto& bookDto) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken, 
                    const adapters::dtos::BookDto& bookDto) override;
    void getBooksMetaData(const QString& authToken) override;
    void downloadBook(const QString& authToken, const QUuid& uuid) override;
    
private slots:
    void proccessBookCreationResult();
    void proccessGettingBooksMetadataResult();
    
private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode, QNetworkReply* reply);
    
    const QString m_bookCreationEndpoint;
    const QString m_getBooksMetadataEndpoint;
    QNetworkAccessManager m_networkAccessManager;
    
    std::unique_ptr<QNetworkReply> m_bookCreationReply = nullptr;
    std::unique_ptr<QNetworkReply> m_gettingBooksMetadataReply = nullptr;
};

} // namespace infrastructure::persistence