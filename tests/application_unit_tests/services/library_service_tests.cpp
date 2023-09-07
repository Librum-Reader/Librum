#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QString>
#include <QUuid>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "book_operation_status.hpp"
#include "i_metadata_extractor.hpp"
#include "i_library_storage_manager.hpp"
#include "library_service.hpp"
#include "tag.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace application::services;
using namespace application::utility;
using namespace application;
using namespace domain::entities;
using namespace domain::value_objects;

namespace tests::application
{

class BookMetaDataHelperMock : public IMetadataExtractor
{
public:
    MOCK_METHOD(bool, setup, (const QString&), (override));
    MOCK_METHOD(BookMetaData, getBookMetaData, (), (override));
    MOCK_METHOD(QImage, getBookCover, (), (override));
};

class LibraryStorageManagerMock : public ILibraryStorageManager
{
public:
    MOCK_METHOD(void, addBook, (const Book&), (override));
    MOCK_METHOD(void, addBookLocally, (const Book&), (override));
    MOCK_METHOD(void, deleteBook, (BookForDeletion), (override));
    MOCK_METHOD(void, deleteAllBooks, (), (override));
    MOCK_METHOD(void, deleteBookLocally, (BookForDeletion), (override));
    MOCK_METHOD(void, uninstallBook, (const Book&), (override));
    MOCK_METHOD(void, downloadBookMedia, (const QUuid&), (override));
    MOCK_METHOD(void, updateBook, (const Book&), (override));
    MOCK_METHOD(void, updateBookLocally, (const Book&), (override));
    MOCK_METHOD(void, updateBookRemotely, (const Book&), (override));
    MOCK_METHOD(void, updateBookCoverRemotely, (const QUuid&, bool),
                (override));
    MOCK_METHOD(QString, saveBookCoverToFile, (const QUuid&, const QImage&),
                (override));
    MOCK_METHOD(bool, deleteBookCoverLocally, (const QUuid&), (override));
    MOCK_METHOD(void, downloadBookCover, (const QUuid&), (override));
    MOCK_METHOD(std::vector<Book>, loadLocalBooks, (), (override));
    MOCK_METHOD(void, downloadRemoteBooks, (), (override));
    MOCK_METHOD(void, setUserData, (const QString&, const QString&),
                (override));
    MOCK_METHOD(void, clearUserData, (), (override));
};

struct ALibraryService : public ::testing::Test
{
    void SetUp() override
    {
        bookService = std::make_unique<LibraryService>(&bookMetaDataHelperMock,
                                                       &bookStorageManagerMock);

        // Make sure that adding books succeeds by default
        EXPECT_CALL(bookMetaDataHelperMock, setup(_))
            .WillRepeatedly(Return(true));
        EXPECT_CALL(bookStorageManagerMock, saveBookCoverToFile(_, _))
            .WillRepeatedly(Return("/some/path"));
    }

    BookMetaDataHelperMock bookMetaDataHelperMock;
    LibraryStorageManagerMock bookStorageManagerMock;
    std::unique_ptr<LibraryService> bookService;
};

TEST_F(ALibraryService, SucceedsAddingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;

    // Act
    auto result = bookService->addBook("some/path.pdf");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsAddingABookIfOpeningDocumentFails)
{
    // Arrange
    auto expectedResult = BookOperationStatus::OpeningBookFailed;


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, setup(_))
        .Times(1)
        .WillOnce(Return(false));

    // Act
    auto result = bookService->addBook("some/path.pdf");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsDeletingABook)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, deleteBook(_)).Times(1);

    // Act
    auto preDeleteBookCount = bookService->getBookCount();
    auto result = bookService->deleteBook(bookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
    EXPECT_EQ(preDeleteBookCount - 1, bookService->getBookCount());
}

TEST_F(ALibraryService, FailsDeletingABookIfBookDoesNotExist)
{
    // Arrange
    auto nonExistentBookUuid = QUuid::createUuid();

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Act
    bookService->addBook("some/first.pdf");

    auto result = bookService->deleteBook(nonExistentBookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsUninstallingABook)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &LibraryService::dataChanged);

    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, uninstallBook(_)).Times(1);

    // Act
    auto result = bookService->uninstallBook(bookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
    EXPECT_FALSE(bookService->getBook(bookUuid)->isDownloaded());

    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(bookService->getBookIndex(bookUuid), arguments[0].toInt());
}

TEST_F(ALibraryService, FailsUninstallingABookIfBookDoesNotExist)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &LibraryService::dataChanged);

    bookService->addBook("some/path.pdf");
    auto nonExistentBookUuid = QUuid::createUuid();

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, uninstallBook(_)).Times(0);

    // Act
    auto result = bookService->uninstallBook(nonExistentBookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
    EXPECT_EQ(0, spy.count());
}

TEST_F(ALibraryService, SucceedsUpdatingABook)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &LibraryService::dataChanged);

    // Create book
    QString originalPath = "/some/path.pdf";
    BookMetaData bookMetaData { .title = "SomeBook", .authors = "SomeAuthor" };
    bookService->addBook(originalPath);
    auto originalUuid =
        bookService->getBooks()[0].getUuid().toString(QUuid::WithoutBraces);

    // Create book-for-update
    BookMetaData newBookMetaData { .title = "ANewTitle",
                                   .authors = "ANewAuthor" };
    Book bookToUpdateWith("some/path", newBookMetaData, 0, originalUuid);
    bookToUpdateWith.addTag(Tag("FirstTag"));
    bookToUpdateWith.addTag(Tag("FirstTag"));

    auto expectedStatus = BookOperationStatus::Success;
    auto expectedResult = bookToUpdateWith;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBook(_)).Times(1);

    // Act
    auto resultStatus = bookService->updateBook(bookToUpdateWith);
    auto* result = bookService->getBook(bookToUpdateWith.getUuid());

    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_EQ(expectedResult.getTitle(), result->getTitle());
    EXPECT_EQ(expectedResult.getFilePath(), result->getFilePath());
    EXPECT_EQ(expectedResult.getAuthors(), result->getAuthors());

    for(int i = 0; i < expectedResult.getTags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.getTags()[i], result->getTags()[i]);
    }

    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(bookService->getBookIndex(bookToUpdateWith.getUuid()),
              arguments[0].toInt());
}

TEST_F(ALibraryService, FailsUpdatingABookIfAPropertyIsTooLong)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &LibraryService::dataChanged);

    // Create book
    BookMetaData bookMetaData { .title = "SomeBook", .authors = "SomeAuthor" };
    bookService->addBook("/some/path.pdf");
    auto originalUuid =
        bookService->getBooks()[0].getUuid().toString(QUuid::WithoutBraces);

    // Create book-for-update
    QString veryLongString(500, 'x');
    BookMetaData newBookMetaData { .title = "ANewTitle",
                                   .creator = veryLongString };  // Too long
    Book bookToUpdateWith("some/path.pdf", newBookMetaData, 0, originalUuid);

    auto expectedResult = BookOperationStatus::OperationFailed;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBook(_)).Times(0);

    // Act
    auto result = bookService->updateBook(bookToUpdateWith);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsUpdatingABookIfBookDoesNotExist)
{
    // Arrange
    QString bookUuid = "non-existend-uuid";
    Book bookToUpdateWidth("some/path", BookMetaData(), 0, bookUuid);

    auto expectedStatus = BookOperationStatus::BookDoesNotExist;


    // Act
    auto resultStatus = bookService->updateBook(bookToUpdateWidth);

    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
}

TEST_F(ALibraryService, SucceedsGettingABook)
{
    // Arrange
    QString path = "some/path.pdf";
    BookMetaData bookMetaData { .title = "SomeTitle", .authors = "SomeAuthor" };
    Book expectedResult(path, bookMetaData);


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(bookMetaData));

    // Act
    bookService->addBook(path);
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto* result = bookService->getBook(bookUuid);

    // Assert
    EXPECT_EQ(expectedResult.getTitle(), result->getTitle());
    EXPECT_EQ(expectedResult.getAuthors(), result->getAuthors());
    EXPECT_EQ(expectedResult.getFilePath(), result->getFilePath());
}

TEST_F(ALibraryService, FailsGettingABookIfBookDoesNotExist)
{
    // Arrange
    QString bookUuid = "non-existend-uuid";

    auto expectedResult = nullptr;


    // Act
    auto* result = bookService->getBook(QUuid(bookUuid));

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsAddingATag)
{
    // Arrange
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    auto expectedResultStatus = BookOperationStatus::Success;


    // Act
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto firstResultStatus = bookService->addTagToBook(bookUuid, firstTag);
    auto secondResultStatus = bookService->addTagToBook(bookUuid, secondTag);

    auto* result = bookService->getBook(bookUuid);

    // Assert
    EXPECT_EQ(expectedResultStatus, firstResultStatus);
    EXPECT_EQ(expectedResultStatus, secondResultStatus);

    EXPECT_EQ(firstTag, result->getTags()[0]);
    EXPECT_EQ(secondTag, result->getTags()[1]);
}

TEST_F(ALibraryService, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    Tag tag("FirstTag");
    auto expectedResult = BookOperationStatus::TagAlreadyExists;


    // Act
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    bookService->addTagToBook(bookUuid, tag);
    auto result = bookService->addTagToBook(bookUuid, tag);


    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsAddingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = QUuid("non-existend-uuid");
    Tag firstTag("FirstTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->addTagToBook(bookUuid, firstTag);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsRemovingATag)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    Tag firstTag("FirstTag");
    bookService->addTagToBook(bookUuid, firstTag);

    auto expectedResultStatus = BookOperationStatus::Success;


    // Act
    bookService->removeTagFromBook(bookUuid, firstTag.getUuid());

    // Assert
    EXPECT_EQ(0, bookService->getBooks()[0].getTags().size());
}

TEST_F(ALibraryService, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto expectedResult = BookOperationStatus::TagDoesNotExist;


    // Act
    auto result = bookService->removeTagFromBook(bookUuid, QUuid::createUuid());


    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsRemovingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = QUuid("non-existend-uuid");
    Tag someTag("SomeTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->removeTagFromBook(bookUuid, someTag.getUuid());

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsRenamingATag)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    QString newName = "SomeNewName";
    Tag firstTag("FirstTag");
    bookService->addTagToBook(bookUuid, firstTag);

    auto expectedResultStatus = BookOperationStatus::Success;


    // Act
    bookService->renameTagOfBook(bookUuid, firstTag.getUuid(), newName);

    // Assert
    EXPECT_EQ(newName, bookService->getBook(bookUuid)->getTags()[0].getName());
}

TEST_F(ALibraryService, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    auto expectedResult = BookOperationStatus::TagDoesNotExist;


    // Act
    auto result =
        bookService->renameTagOfBook(bookUuid, QUuid::createUuid(), "SomeName");


    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsRenamingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = QUuid("non-existend-uuid");
    Tag someTag("SomeTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->renameTagOfBook(bookUuid, someTag.getUuid(),
                                               someTag.getUuid().toString());

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsGettingAllBooks)
{
    // Arrange
    BookMetaData firstBookMetaData { .title = "FirstBook",
                                     .authors = "Author1" };
    Book firstBook("/some/path", firstBookMetaData);
    BookMetaData secondBookMetaData { .title = "SecondBook",
                                      .authors = "Author2" };
    Book secondBook("/some/path2", secondBookMetaData);
    BookMetaData thirdBookMetaData { .title = "ThirdBook",
                                     .authors = "Author3" };
    Book thirdBook("/some/path2", thirdBookMetaData);

    std::vector<Book> expectedResult { firstBook, secondBook, thirdBook };


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(3)
        .WillOnce(Return(firstBookMetaData))
        .WillOnce(Return(secondBookMetaData))
        .WillOnce(Return(thirdBookMetaData));

    // Act
    bookService->addBook(firstBook.getFilePath());
    bookService->addBook(secondBook.getFilePath());
    bookService->addBook(thirdBook.getFilePath());

    auto results = bookService->getBooks();

    // Assert
    for(int i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_EQ(expectedResult[i].getTitle(), results[i].getTitle());
        EXPECT_EQ(expectedResult[i].getAuthors(), results[i].getAuthors());
        EXPECT_EQ(expectedResult[i].getFilePath(), results[i].getFilePath());
    }
}

TEST_F(ALibraryService, SucceedsGettingABookIndex)
{
    // Arrange
    BookMetaData firstBookMetaData { .title = "FirstBook",
                                     .authors = "Author1" };
    BookMetaData secondBookMetaData { .title = "SecondBook",
                                      .authors = "Author2" };

    Book firstBook("/some/path", firstBookMetaData);
    Book secondBook("/some/path2", secondBookMetaData);

    int expectedResult = 1;


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(2)
        .WillOnce(Return(firstBookMetaData))
        .WillOnce(Return(secondBookMetaData));


    // Act
    bookService->addBook(firstBook.getFilePath());
    bookService->addBook(secondBook.getFilePath());
    auto secondBookUuid = bookService->getBooks()[1].getUuid();

    auto result = bookService->getBookIndex(secondBookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsChangingABookCoverByDeletingIt)
{
    // Arrange
    BookMetaData bookMetaData { .title = "FirstBook", .authors = "Author1" };
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(bookMetaData));
    bookService->addBook("some/path.pdf");

    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBook(_)).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely(_, _)).Times(1);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally(_)).Times(1);

    // Act
    auto& uuid = bookService->getBooks()[0].getUuid();
    auto result = bookService->changeBookCover(uuid, "");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsChangingABookCoverIfNewCoverDoesNotExist)
{
    // Arrange
    BookMetaData bookMetaData { .title = "FirstBook", .authors = "Author1" };
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(bookMetaData));
    bookService->addBook("some/path.pdf");

    auto expectedResult = BookOperationStatus::OperationFailed;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBook(_)).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely(_, _)).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally(_)).Times(0);

    // Act
    auto& uuid = bookService->getBooks()[0].getUuid();
    auto result = bookService->changeBookCover(uuid, "non-existent/book.pdf");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsChangingABookCoverIfBookDoesNotExist)
{
    // Arrange
    auto nonExistentBook = QUuid::createUuid();

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBook(_)).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely(_, _)).Times(0);

    // Act
    auto result = bookService->changeBookCover(nonExistentBook, "");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsDownloadingABook)
{
    // Arrange
    BookMetaData bookMetaData { .title = "FirstBook", .authors = "Author1" };
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(bookMetaData));
    bookService->addBook("some/path.pdf");

    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, downloadBookMedia(_)).Times(1);

    // Act
    auto uuid = bookService->getBooks()[0].getUuid();
    auto result = bookService->downloadBookMedia(uuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsDownloadingBookIfItDoesNotExist)
{
    // Arrange
    auto nonExistentBook = QUuid::createUuid();

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, downloadBookMedia(_)).Times(0);

    // Act
    auto result = bookService->downloadBookMedia(nonExistentBook);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, FailsGettingABookIndexIfBookDoesNotExist)
{
    // Arrange
    QUuid nonExistentBook = QUuid::createUuid();

    int expectedResult = -1;

    // Act
    auto result = bookService->getBookIndex(nonExistentBook);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsGettingTheBookCount)
{
    // Arrange
    auto expectedResult = 2;


    // Act
    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");

    auto result = bookService->getBookCount();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ALibraryService, SucceedsRefreshingLastOpened)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();

    // Act
    auto before = bookService->getBook(bookUuid)->getLastOpened();
    bookService->refreshLastOpenedDateOfBook(bookUuid);
    auto after = bookService->getBook(bookUuid)->getLastOpened();

    // Assert
    EXPECT_NE(before, after);
}

TEST_F(ALibraryService, SucceedsMergingARemoteBookIntoALocalBook)
{
    // Arrange
    BookMetaData localBookMetaData {
        .title = "SomeBook",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),
    };

    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    QString filePath = "some/path.pdf";
    bookService->addBook(filePath);
    const Book& localBook = bookService->getBooks()[0];
    auto localBookUuid = localBook.getUuid().toString(QUuid::WithoutBraces);

    BookMetaData remoteMetaData = localBookMetaData;
    remoteMetaData.lastModified = QDateTime::currentDateTimeUtc().addDays(2);
    remoteMetaData.title = "SomeOtherTitle";
    Book remoteBook(filePath, remoteMetaData, 0, localBookUuid);


    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.finishedDownloadingRemoteBooks(myBooks);

    // Assert
    EXPECT_EQ(remoteBook, localBook);
}

TEST_F(ALibraryService, SucceedsAddingARemoteBookToALocalBook)
{
    // Arrange
    QSignalSpy startSpy(bookService.get(),
                        &LibraryService::bookInsertionStarted);
    QSignalSpy endSpy(bookService.get(), &LibraryService::bookInsertionEnded);

    BookMetaData remoteMetaData {
        .title = "SomeBook",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),
        .hasCover = true,
    };
    remoteMetaData.lastModified = QDateTime::currentDateTimeUtc().addDays(2);
    remoteMetaData.title = "SomeOtherTitle";
    Book remoteBook("some/path.pdf", remoteMetaData, 0);


    // Expect
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover(_)).Times(1);

    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.finishedDownloadingRemoteBooks(myBooks);

    // Assert
    EXPECT_EQ(1, startSpy.count());
    EXPECT_EQ(1, endSpy.count());
}

TEST_F(ALibraryService, SucceedsAddingALocalBookToRemoteServer)
{
    // Arrange
    BookMetaData localBookMetaData {
        .title = "SomeBook",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc().addDays(2),
    };


    // Expect two calls, one during test setup, and one while merging
    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(2);  // Twice!
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData())
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    bookService->updateUsedBookStorage(0, 99999999);  // Setup storage limit
    bookService->addBook("some/path.pdf");


    // Act
    std::vector<Book> myBooks {};
    emit bookStorageManagerMock.finishedDownloadingRemoteBooks(myBooks);
}

}  // namespace tests::application
