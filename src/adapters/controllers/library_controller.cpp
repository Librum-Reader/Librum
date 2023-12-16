#include "library_controller.hpp"
#include <QBuffer>
#include <QDebug>
#include <QFileInfo>
#include <QUrl>
#include <QVariant>
#include <QtWidgets/QApplication>
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"

namespace adapters::controllers
{

using namespace domain::entities;
using namespace dtos;

LibraryController::LibraryController(
    application::ILibraryService* bookService) :
    m_libraryService(bookService),
    m_libraryModel(m_libraryService->getBooks())
{
    // book insertion
    connect(m_libraryService,
            &application::ILibraryService::bookInsertionStarted,
            &m_libraryModel, &data_models::LibraryModel::startInsertingRow);

    connect(m_libraryService, &application::ILibraryService::bookInsertionEnded,
            &m_libraryModel, &data_models::LibraryModel::endInsertingRow);

    connect(m_libraryService, &application::ILibraryService::bookInsertionEnded,
            this, &LibraryController::bookCountChanged);

    // Library syncing
    connect(m_libraryService,
            &application::ILibraryService::syncingLibraryStarted,
            [this]()
            {
                m_currentlySyncing = true;
                emit isSyncingChanged();
            });

    connect(m_libraryService,
            &application::ILibraryService::syncingLibraryFinished,
            [this]()
            {
                m_currentlySyncing = false;
                emit isSyncingChanged();
            });


    // book deletion
    connect(m_libraryService,
            &application::ILibraryService::bookDeletionStarted, &m_libraryModel,
            &data_models::LibraryModel::startDeletingBook);

    connect(m_libraryService, &application::ILibraryService::bookDeletionEnded,
            &m_libraryModel, &data_models::LibraryModel::endDeletingBook);

    connect(m_libraryService, &application::ILibraryService::bookDeletionEnded,
            this, &LibraryController::bookCountChanged);


    // book clearing
    connect(m_libraryService,
            &application::ILibraryService::bookClearingStarted, &m_libraryModel,
            &data_models::LibraryModel::startBookClearing);

    connect(m_libraryService, &application::ILibraryService::bookClearingEnded,
            &m_libraryModel, &data_models::LibraryModel::endBookClearing);

    connect(m_libraryService, &application::ILibraryService::bookClearingEnded,
            this, &LibraryController::bookCountChanged);

    // Storage limit exceeded
    connect(m_libraryService,
            &application::ILibraryService::storageLimitExceeded, this,
            &LibraryController::storageLimitExceeded);

    // tags changed
    connect(m_libraryService, &application::ILibraryService::tagsChanged,
            &m_libraryModel, &data_models::LibraryModel::refreshTags);

    // data changed
    connect(m_libraryService, &application::ILibraryService::dataChanged,
            &m_libraryModel, &data_models::LibraryModel::refreshBook);

    // download book media progress changed
    connect(m_libraryService,
            &application::ILibraryService::downloadingBookMediaProgressChanged,
            &m_libraryModel,
            &data_models::LibraryModel::downloadingBookMediaProgressChanged);

    // downloaded Project Gutenberg books
    connect(m_libraryService,
            &application::ILibraryService::downloadedProjectGutenbergIdsReady,
            this, &LibraryController::downloadedProjectGutenbergIdsReady);

    m_libraryProxyModel.setSourceModel(&m_libraryModel);
}

void LibraryController::syncWithServer()
{
    m_libraryService->downloadBooks();
}

int LibraryController::addBook(const QString& path, bool allowDuplicates,
                               int projectGutenbergId)
{
    auto localPath = QUrl(path).toLocalFile();
    QFileInfo fileInfo(localPath);
    if(!fileInfo.isFile())
        return 0;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto result = m_libraryService->addBook(localPath, allowDuplicates,
                                            projectGutenbergId);
    QApplication::restoreOverrideCursor();

    emit addingBookFinished(
        projectGutenbergId,
        result == BookOperationStatus::Success ? true : false);

    return static_cast<int>(result);
}

int LibraryController::deleteBook(const QString& uuid)
{
    auto result = m_libraryService->deleteBook(QUuid(uuid));
    return static_cast<int>(result);
}

int LibraryController::deleteAllBooks()
{
    auto result = m_libraryService->deleteAllBooks();
    return static_cast<int>(result);
}

int LibraryController::uninstallBook(const QString& uuid)
{
    auto result = m_libraryService->uninstallBook(QUuid(uuid));
    return static_cast<int>(result);
}

int LibraryController::downloadBookMedia(const QString& uuid)
{
    auto result = m_libraryService->downloadBookMedia(QUuid(uuid));
    return static_cast<int>(result);
}

int LibraryController::updateBook(const QString& uuid,
                                  const QVariant& operations)
{
    auto bookToUpdate = m_libraryService->getBook(QUuid(uuid));
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

    auto result = m_libraryService->updateBook(updatedBook);
    return static_cast<int>(result);
}

int LibraryController::changeBookCover(const QString& uuid, const QString& path)
{
    auto result = m_libraryService->changeBookCover(QUuid(uuid),
                                                    QUrl(path).toLocalFile());
    return static_cast<int>(result);
}

int LibraryController::addTag(const QString& bookUuid, const QString& tagName,
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
    auto result = m_libraryService->addTagToBook(QUuid(bookUuid), tag);

    return static_cast<int>(result);
}

void LibraryController::removeAllTagsWithUuid(const QString& tagUuid)
{
    if(QUuid(tagUuid).isNull())
        return;

    auto& books = m_libraryService->getBooks();
    for(const auto& book : books)
    {
        if(listContainsTag(book.getTags(), QUuid(tagUuid)))
        {
            m_libraryService->removeTagFromBook(book.getUuid(), QUuid(tagUuid));
        }
    }
}

void LibraryController::renameTags(const QString& oldName,
                                   const QString& newName)
{
    auto& books = m_libraryService->getBooks();
    for(const auto& book : books)
    {
        auto tagUuid = getTagUuidByName(book, oldName);
        if(!tagUuid.isNull())
            m_libraryService->renameTagOfBook(book.getUuid(), tagUuid, newName);
    }
}

int LibraryController::removeTag(const QString& bookUuid,
                                 const QString& tagUuid)
{
    auto result =
        m_libraryService->removeTagFromBook(QUuid(bookUuid), QUuid(tagUuid));
    return static_cast<int>(result);
}

dtos::BookDto LibraryController::getBook(const QString& uuid)
{
    const auto& books = m_libraryService->getBooks();
    auto book = std::ranges::find_if(books,
                                     [&uuid](const Book& b)
                                     {
                                         return b.getUuid() == QUuid(uuid);
                                     });

    return book == books.end() ? dtos::BookDto() : getDtoFromBook(*book);
}

int LibraryController::getBookCount() const
{
    return m_libraryService->getBookCount();
}

bool LibraryController::isSyncing() const
{
    return m_currentlySyncing;
}

data_models::LibraryProxyModel* LibraryController::getLibraryModel()
{
    return &m_libraryProxyModel;
}

int LibraryController::saveBookToFile(const QString& uuid, const QUrl& path)
{
    auto result =
        m_libraryService->saveBookToFile(QUuid(uuid), path.toLocalFile());

    return static_cast<int>(result);
}

void LibraryController::refreshLastOpenedFlag(const QString& uuid)
{
    m_libraryService->refreshLastOpenedDateOfBook(QUuid(uuid));
}

dtos::BookDto LibraryController::getDtoFromBook(
    const domain::entities::Book& book)
{
    dtos::BookDto bookDto;
    addBookMetaDataToDto(book, bookDto);
    addBookTagsToDto(book, bookDto);

    return bookDto;
}

QUuid LibraryController::getTagUuidByName(const Book& book, const QString& name)
{
    for(const auto& tag : book.getTags())
    {
        if(tag.getName() == name)
            return tag.getUuid();
    }

    return QUuid();
}

void LibraryController::addBookMetaDataToDto(const Book& book, BookDto& bookDto)
{
    auto pathWithScheme = QUrl::fromLocalFile(book.getCoverPath()).toString();

    bookDto.uuid = book.getUuid().toString(QUuid::WithoutBraces);
    bookDto.projectGutenbergId = book.getProjectGutenbergId();
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
    bookDto.coverPath = pathWithScheme;
    bookDto.downloaded = book.isDownloaded();

    bookDto.addedToLibrary = book.getAddedToLibrary().toLocalTime().toString(
        Book::dateTimeStringFormat);

    bookDto.lastOpened = book.getLastOpened().isNull()
                             ? "Never"
                             : book.getLastOpened().toLocalTime().toString(
                                   Book::dateTimeStringFormat);
}

void LibraryController::addBookTagsToDto(const Book& book, BookDto& bookDto)
{
    for(const auto& tag : book.getTags())
    {
        dtos::TagDto tagDto;
        tagDto.name = tag.getName();

        bookDto.tags.push_back(tagDto);
    }
}

bool LibraryController::listContainsTag(const QList<Tag>& tags, QUuid uuid)
{
    return std::ranges::any_of(tags,
                               [&uuid](const Tag& tag)
                               {
                                   return tag.getUuid() == uuid;
                               });
}

}  // namespace adapters::controllers
