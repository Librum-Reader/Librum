#include "book_controller.hpp"
#include <QBuffer>
#include <QDebug>
#include <QVariant>
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"

namespace adapters::controllers
{

using namespace domain::entities;
using namespace dtos;

BookController::BookController(application::IBookService* bookService) :
    m_bookService(bookService),
    m_libraryModel(m_bookService->getBooks())
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


    // book clearing
    connect(m_bookService, &application::IBookService::bookClearingStarted,
            &m_libraryModel, &data_models::LibraryModel::startBookClearing);

    connect(m_bookService, &application::IBookService::bookClearingEnded,
            &m_libraryModel, &data_models::LibraryModel::endBookClearing);

    connect(m_bookService, &application::IBookService::bookClearingEnded, this,
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
    return static_cast<int>(result);
}

int BookController::deleteBook(const QString& uuid)
{
    auto result = m_bookService->deleteBook(uuid);
    return static_cast<int>(result);
}

int BookController::uninstallBook(const QString& uuid)
{
    auto result = m_bookService->uninstallBook(uuid);
    return static_cast<int>(result);
}

int BookController::downloadBook(const QString& uuid)
{
    auto result = m_bookService->downloadBook(uuid);
    return static_cast<int>(result);
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
        case MetaProperty::Invalid:
            return static_cast<int>(BookOperationStatus::PropertyDoesNotExist);
            break;
        default:
            return static_cast<int>(BookOperationStatus::PropertyDoesNotExist);
        }
    }


    auto result = m_bookService->updateBook(updatedBook);
    return static_cast<int>(result);
}

int BookController::changeBookCover(const QString& uuid, const QString& path)
{
    auto result = m_bookService->changeBookCover(uuid, path);
    return static_cast<int>(result);
}

int BookController::addTag(const QString& bookUuid, const QString& tagName,
                           const QString& tagUuid)
{
    if(QUuid(tagUuid).isNull())
    {
        qWarning() << QString("Adding tag with name: %1 to book with uuid: %2 "
                              "failed. The given uuid was invalid.")
                          .arg(tagName, bookUuid);
        return static_cast<int>(BookOperationStatus::OperationFailed);
    }

    Tag tag(tagName, tagUuid);
    auto result = m_bookService->addTagToBook(bookUuid, tag);

    return static_cast<int>(result);
}

void BookController::removeAllTagsWithUuid(const QString& tagUuid)
{
    if(QUuid(tagUuid).isNull())
        return;

    auto& books = m_bookService->getBooks();
    for(const auto& book : books)
    {
        if(vectorContainsTag(book.getTags(), tagUuid))
        {
            m_bookService->removeTagFromBook(book.getUuid(), tagUuid);
        }
    }
}

void BookController::renameTags(const QString& oldName, const QString& newName)
{
    auto& books = m_bookService->getBooks();
    for(const auto& book : books)
    {
        auto tagUuid = getTagUuidByName(book, oldName);
        if(!tagUuid.isNull())
            m_bookService->renameTagOfBook(book.getUuid(), tagUuid, newName);
    }
}

int BookController::removeTag(const QString& bookUuid, const QString& tagUuid)
{
    auto result = m_bookService->removeTagFromBook(bookUuid, tagUuid);
    return static_cast<int>(result);
}

dtos::BookDto BookController::getBook(const QString& uuid)
{
    const auto& books = m_bookService->getBooks();
    auto book = std::ranges::find_if(books,
                                     [&uuid](const Book& b)
                                     {
                                         return b.getUuid() == uuid;
                                     });

    return book == books.end() ? dtos::BookDto() : getDtoFromBook(*book);
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
    m_bookService->refreshLastOpenedDateOfBook(uuid);
}

dtos::BookDto BookController::getDtoFromBook(const domain::entities::Book& book)
{
    dtos::BookDto bookDto;
    addBookMetaDataToDto(book, bookDto);
    addBookTagsToDto(book, bookDto);

    return bookDto;
}

QUuid BookController::getTagUuidByName(const Book& book, const QString& name)
{
    for(const auto& tag : book.getTags())
    {
        if(tag.getName() == name)
            return tag.getUuid();
    }

    return QUuid();
}

void BookController::addBookMetaDataToDto(const Book& book, BookDto& bookDto)
{
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
    bookDto.bookReadingProgress = book.getBookReadingProgress();
    bookDto.coverPath = book.getCoverPath();
    bookDto.downloaded = book.getDownloaded();

    bookDto.addedToLibrary = book.getAddedToLibrary().toLocalTime().toString(
        Book::dateTimeStringFormat);

    bookDto.lastOpened = book.getLastOpened().isNull()
                             ? "Never"
                             : book.getLastOpened().toLocalTime().toString(
                                   Book::dateTimeStringFormat);
}

void BookController::addBookTagsToDto(const Book& book, BookDto& bookDto)
{
    for(const auto& tag : book.getTags())
    {
        dtos::TagDto tagDto;
        tagDto.name = tag.getName();

        bookDto.tags.push_back(tagDto);
    }
}

bool BookController::vectorContainsTag(const std::vector<Tag>& tags, QUuid uuid)
{
    return std::ranges::any_of(tags,
                               [&uuid](const Tag& tag)
                               {
                                   return tag.getUuid() == uuid;
                               });
}

}  // namespace adapters::controllers