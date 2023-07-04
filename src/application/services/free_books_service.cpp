#include "free_books_service.hpp"
#include <QDir>
#include <QFile>
#include <QUuid>

namespace application::services
{

FreeBooksService::FreeBooksService(
    IFreeBooksStorageGateway* freeBooksStorageGateway) :
    m_freeBooksStorageGateway(freeBooksStorageGateway)
{
    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBookMediaFinished, this,
            &FreeBooksService::saveBookToFile);
}

void FreeBooksService::getBooksMetadata(const QString& author,
                                        const QString& title)
{
    m_freeBooksStorageGateway->getBooksMetadata(author, title);
}

void FreeBooksService::getBookMedia(const QString& url)
{
    m_freeBooksStorageGateway->getBookMedia(url);
}

void FreeBooksService::saveBookToFile(const QByteArray& data)
{
    auto currentDir = QDir::current().path();
    auto folder = QDir(currentDir + "/" + m_freeBooksFolderName);
    if(!folder.exists())
        folder.mkpath(folder.path());

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString bookFilePath = folder.path() + "/" + uuid + ".epub";

    QFile file(bookFilePath);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    emit gettingBookFinished(bookFilePath);
}

}  // namespace application::services
