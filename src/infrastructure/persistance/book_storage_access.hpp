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
    std::vector<adapters::dtos::BookDto> getBooksMetaData(const QString& 
                                                          authToken) override;
    void downloadBook(const QString& authToken, const QUuid& uuid) override;
    
private slots:
    void proccessBookCreationResult();
    
private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode);
    
    const QString m_bookCreationEndpoint;
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

} // namespace infrastructure::persistence