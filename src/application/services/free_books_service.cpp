#include "free_books_service.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUuid>
#include "free_book.hpp"

namespace application::services
{

using domain::value_objects::FreeBook;

FreeBooksService::FreeBooksService(
    IFreeBooksStorageGateway* freeBooksStorageGateway) :
    m_freeBooksStorageGateway(freeBooksStorageGateway)
{
    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBookMediaFinished, this,
            &FreeBooksService::saveBookToFile);

    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBooksMetaDataFinished, this,
            &FreeBooksService::saveBookMetaData);

    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBookCoverFinished, this,
            &FreeBooksService::setBookCover);
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

std::vector<FreeBook>& FreeBooksService::getFreeBooks()
{
    return m_freeBooks;
}

void FreeBooksService::setBookCover(int bookId, const QImage& cover)
{
    auto freeBook = getFreeBookById(bookId);
    if(freeBook == nullptr)
    {
        qDebug() << "Failed setting cover for free book. Book does not exist.";
        return;
    }

    freeBook->cover =
        cover.scaled(maxCoverWidth, maxCoverHeight, Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);
    emit dataChanged(getFreeBookIndexById(bookId));
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

void FreeBooksService::saveBookMetaData(std::vector<FreeBook>& books)
{
    for(auto& book : books)
    {
        emit bookInsertionStarted(m_freeBooks.size());
        m_freeBooks.emplace_back(book);
        emit bookInsertionEnded();
    }
}

FreeBook* FreeBooksService::getFreeBookById(int id)
{
    for(auto& freeBook : m_freeBooks)
    {
        if(freeBook.id == id)
            return &freeBook;
    }

    return nullptr;
}

int FreeBooksService::getFreeBookIndexById(int id)
{
    for(int i = 0; i < m_freeBooks.size(); ++i)
    {
        if(m_freeBooks.at(i).id == id)
            return i;
    }

    return -1;
}

}  // namespace application::services
