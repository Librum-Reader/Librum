#include "free_books_service.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QUuid>
#include "free_book.hpp"
#include "save_book_helper.hpp"

namespace application::services
{

using domain::value_objects::FreeBook;

FreeBooksService::FreeBooksService(
    IFreeBooksStorageGateway* freeBooksStorageGateway) :
    m_freeBooksStorageGateway(freeBooksStorageGateway)
{
    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBooksMetaDataFinished, this,
            &FreeBooksService::saveBookMetaData);

    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBookCoverFinished, this,
            &FreeBooksService::setBookCover);

    connect(m_freeBooksStorageGateway,
            &IFreeBooksStorageGateway::gettingBookMediaChunkReady, this,
            &FreeBooksService::saveDownloadedBookMediaChunkToFile);
}

void FreeBooksService::getBooksMetadata(const QString& author,
                                        const QString& title)
{
    m_freeBooksStorageGateway->getBooksMetadata(author, title);
}

void FreeBooksService::getBookMedia(const QString& url)
{
    auto newBookUuid = QUuid::createUuid();

    m_freeBooksStorageGateway->getBookMedia(url, newBookUuid);
}

void FreeBooksService::setupUserData(const QString& token, const QString& email)
{
    Q_UNUSED(token);

    m_userEmail = email;
}

void FreeBooksService::clearUserData()
{
    m_userEmail.clear();
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

void FreeBooksService::saveDownloadedBookMediaChunkToFile(
    const QByteArray& data, bool isLastChunk, const QUuid& uuid,
    const QString& format)
{
    auto destDir = getLibraryDir();
    QString fileName = uuid.toString(QUuid::WithoutBraces) + "." + format;
    auto destination = destDir.filePath(fileName);

    application::utility::saveDownloadedBookMediaChunkToFile(
        data, isLastChunk, fileName, destination);

    if(isLastChunk)
    {
        emit gettingBookFinished(QUrl::fromLocalFile(destination).toString());
    }
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

QDir FreeBooksService::getLibraryDir()
{
    auto applicationPath = QDir::current().path();
    auto hash = qHash(m_userEmail);
    auto userLibName = QString::number(hash);

    auto uniqueFolderName =
        applicationPath + "/" + "librum_localLibraries" + "/" + userLibName;

    return QDir(uniqueFolderName);
}

}  // namespace application::services
