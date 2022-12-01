#include "book_controller.hpp"
#include <QBuffer>
#include <QDebug>
#include <QVariant>
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "qnamespace.h"
#include "tag.hpp"
#include "tag_dto.hpp"

namespace adapters::controllers
{

using namespace domain::models;
using application::BookOperationStatus;

BookController::BookController(application::IBookService* bookService) :
    m_bookChacheChanged(true),
    m_bookService(bookService),
    m_libraryModel(m_bookService->getBooks()),
    m_libraryProxyModel(static_cast<QObject*>(&m_libraryModel))
{
    // book insertion
    connect(m_bookService, &application::IBookService::bookInsertionStarted,
            &m_libraryModel, &data_models::LibraryModel::startInsertingRow);

    connect(m_bookService, &application::IBookService::bookInsertionEnded,
            &m_libraryModel, &data_models::LibraryModel::endInsertingRow);

    connect(m_bookService, &application::IBookService::bookInsertionEnded, this,
            &BookController::bookCountChanged);

    // book deletion
    connect(m_bookService, &application::IBookService::bookDeletionStarted,
            &m_libraryModel, &data_models::LibraryModel::startDeletingBook);

    connect(m_bookService, &application::IBookService::bookDeletionEnded,
            &m_libraryModel, &data_models::LibraryModel::endDeletingBook);

    connect(m_bookService, &application::IBookService::bookDeletionEnded, this,
            &BookController::bookCountChanged);

    // tags changed
    connect(m_bookService, &application::IBookService::tagsChanged,
            &m_libraryModel, &data_models::LibraryModel::refreshTags);

    // data changed
    connect(m_bookService, &application::IBookService::dataChanged,
            &m_libraryModel, &data_models::LibraryModel::refreshBook);

    // book cover processing
    connect(m_bookService, &application::IBookService::bookCoverGenerated,
            &m_libraryModel, &data_models::LibraryModel::processBookCover);


    m_libraryProxyModel.setSourceModel(&m_libraryModel);
}

int BookController::addBook(const QString& path)
{
    auto result = m_bookService->addBook(path);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }

    return static_cast<int>(result);
}

int BookController::deleteBook(const QString& uuid)
{
    auto result = m_bookService->deleteBook(uuid);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }

    return static_cast<int>(BookOperationStatus::BookDoesNotExist);
}

int BookController::uninstallBook(const QString& uuid)
{
    auto result = m_bookService->uninstallBook(uuid);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }

    return static_cast<int>(BookOperationStatus::BookDoesNotExist);
}

int BookController::updateBook(const QString& uuid, const QVariant& operations)
{
    auto bookToUpdate = m_bookService->getBook(uuid);
    if(!bookToUpdate)
        return static_cast<int>(BookOperationStatus::BookDoesNotExist);

    auto updatedBook = *bookToUpdate;

    auto operationsMap = operations.toMap();
    for(const auto& stringKey : operationsMap.keys())
    {
        int key = stringKey.toInt();

        auto value = operationsMap.value(stringKey);
        switch(static_cast<MetaProperty>(key))
        {
        case MetaProperty::Title:
            updatedBook.setTitle(value.toString());
            break;
        case MetaProperty::Authors:
            updatedBook.setAuthors(value.toString());
            break;
        case MetaProperty::FilePath:
            updatedBook.setFilePath(value.toString());
            break;
        case MetaProperty::Creator:
            updatedBook.setCreator(value.toString());
            break;
        case MetaProperty::CreationDate:
            updatedBook.setCreationDate(value.toString());
            break;
        case MetaProperty::Format:
            updatedBook.setFormat(value.toString());
            break;
        case MetaProperty::Language:
            updatedBook.setLanguage(value.toString());
            break;
        case MetaProperty::DocumentSize:
            updatedBook.setDocumentSize(value.toString());
            break;
        case MetaProperty::PagesSize:
            updatedBook.setPagesSize(value.toString());
            break;
        case MetaProperty::PageCount:
            updatedBook.setPageCount(value.toInt());
            break;
        case MetaProperty::CurrentPage:
            updatedBook.setCurrentPage(value.toInt());
            break;
        case MetaProperty::AddedToLibrary:
            updatedBook.setAddedToLibrary(
                QDateTime::fromString(value.toString()));
            break;
        case MetaProperty::LastModified:
            updatedBook.setLastOpened(QDateTime::fromString(value.toString()));
            break;
        case MetaProperty::Cover:
            updatedBook.setCover(getCorrectlySizedBookCover(value.toString()));
            break;
        default:
            return static_cast<int>(BookOperationStatus::PropertyDoesNotExist);
        }
    }


    auto result = m_bookService->updateBook(updatedBook);
    if(result == BookOperationStatus::Success)
        m_bookChacheChanged = true;

    return static_cast<int>(result);
}

int BookController::addTag(const QString& uuid, const QString& tagName)
{
    Tag tagToAdd(tagName);
    if(m_bookService->addTag(uuid, tagToAdd) == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }

    return static_cast<int>(BookOperationStatus::TagAlreadyExists);
}

int BookController::removeTag(const QString& uuid, const QString& tagName)
{
    Tag tagToRemove(tagName);
    if(m_bookService->removeTag(uuid, tagToRemove) ==
       BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }

    return static_cast<int>(BookOperationStatus::TagDoesNotExist);
}

dtos::BookDto BookController::getBook(const QString& uuid)
{
    if(m_bookChacheChanged)
        refreshBookChache();

    return *getBookFromChache(uuid);
}

int BookController::getBookCount() const
{
    return m_bookService->getBookCount();
}

data_models::LibraryProxyModel* BookController::getLibraryModel()
{
    return &m_libraryProxyModel;
}

int BookController::saveBookToFile(const QString& uuid, const QUrl& path)
{
    auto result = m_bookService->saveBookToFile(uuid, path);

    return static_cast<int>(result);
}

void BookController::refreshLastOpenedFlag(const QString& uuid)
{
    auto result = m_bookService->refreshLastOpened(uuid);
    if(result)
        m_bookChacheChanged = true;
}

void BookController::refreshBookChache()
{
    const auto& books = m_bookService->getBooks();

    m_bookCache.clear();
    for(const auto& book : books)
    {
        auto bookDto = getDtoFromBook(book);
        m_bookCache.emplace_back(std::move(bookDto));
    }

    m_bookChacheChanged = false;
}

dtos::BookDto* BookController::getBookFromChache(const QString& uuid)
{
    for(std::size_t i = 0; i < m_bookCache.size(); ++i)
    {
        if(m_bookCache[i].uuid == uuid)
            return &m_bookCache[i];
    }

    return nullptr;
}

QImage BookController::getCorrectlySizedBookCover(const QString& pathToCover)
{
    if(pathToCover.isEmpty())
        return QImage();

    QString localFilePath = QUrl(pathToCover).toLocalFile();
    QImage cover(localFilePath);
    auto scaledCover =
        cover.scaled(Book::maxCoverWidth, Book::maxCoverHeight,
                     Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return scaledCover;
}

dtos::BookDto BookController::getDtoFromBook(const domain::models::Book& book)
{
    const QString dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";

    dtos::BookDto bookDto;
    bookDto.uuid = book.getUuid().toString(QUuid::WithoutBraces);
    bookDto.title = book.getTitle();
    bookDto.authors = book.getAuthors();
    bookDto.filePath = book.getFilePath();
    bookDto.creator = book.getCreator();
    bookDto.creationDate = book.getCreationDate();
    bookDto.format = book.getFormat();
    bookDto.language = book.getLanguage();
    bookDto.documentSize = book.getDocumentSize();
    bookDto.pagesSize = book.getPagesSize();
    bookDto.pageCount = book.getPageCount();
    bookDto.currentPage = book.getCurrentPage();

    bookDto.addedToLibrary =
        book.getAddedToLibrary().toLocalTime().toString(dateTimeFormat);

    bookDto.lastOpened =
        book.getLastOpened().isNull()
            ? "Never"
            : book.getLastOpened().toLocalTime().toString(dateTimeFormat);

    bookDto.cover = book.getCoverAsStringWithType();
    bookDto.downloaded = book.getDownloaded();


    for(std::size_t i = 0; i < book.getTags().size(); ++i)
    {
        dtos::TagDto tagDto;
        tagDto.name = book.getTags()[i].getName();

        bookDto.tags.push_back(tagDto);
    }

    return bookDto;
}

}  // namespace adapters::controllers