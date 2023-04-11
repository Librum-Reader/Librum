#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QString>
#include <QUuid>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "book_operation_status.hpp"
#include "book_service.hpp"
#include "i_book_metadata_helper.hpp"
#include "i_book_storage_manager.hpp"
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

class BookMetaDataHelperMock : public IBookMetadataHelper
{
public:
    MOCK_METHOD(std::optional<BookMetaData>, getBookMetaData, (const QString&),
                (override));
    MOCK_METHOD(void, loadCover, (), (const, override));
};

class BookStorageManagerMock : public IBookStorageManager
{
public:
    MOCK_METHOD(void, addBook, (const Book&), (override));
    MOCK_METHOD(void, addBookLocally, (const Book&), (override));
    MOCK_METHOD(void, deleteBook, (BookForDeletion), (override));
    MOCK_METHOD(void, deleteBookLocally, (QUuid), (override));
    MOCK_METHOD(void, uninstallBook, (const QUuid&), (override));
    MOCK_METHOD(void, downloadBook, (const QUuid&), (override));
    MOCK_METHOD(void, updateBook, (const Book&), (override));
    MOCK_METHOD(void, updateBookLocally, (const Book&), (override));
    MOCK_METHOD(void, updateBookRemotely, (const Book&), (override));
    MOCK_METHOD(void, changeBookCover, (const Book&), (override));
    MOCK_METHOD(std::optional<QString>, saveBookCoverToFile,
                (const QUuid&, const QPixmap&), (override));
    MOCK_METHOD(bool, deleteBookCoverLocally, (const QUuid&), (override));
    MOCK_METHOD(void, getCoverForBook, (const QUuid&), (override));
    MOCK_METHOD(std::vector<Book>, loadLocalBooks, (), (override));
    MOCK_METHOD(void, loadRemoteBooks, (), (override));
    MOCK_METHOD(void, setUserData, (const QString&, const QString&),
                (override));
    MOCK_METHOD(void, clearUserData, (), (override));
};

struct ABookService : public ::testing::Test
{
    void SetUp() override
    {
        EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
            .WillRepeatedly(Return(BookMetaData()));

        bookService = std::make_unique<BookService>(&bookMetaDataHelperMock,
                                                    &bookStorageManagerMock);
    }

    BookMetaDataHelperMock bookMetaDataHelperMock;
    BookStorageManagerMock bookStorageManagerMock;
    std::unique_ptr<BookService> bookService;
};

TEST_F(ABookService, SucceedsAddingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);

    // Act
    auto result = bookService->addBook("some/path.pdf");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, FailsAddingABookIfGettingBookMetaDataFails)
{
    // Arrange
    auto expectedResult = BookOperationStatus::OpeningBookFailed;


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(std::nullopt));

    // Act
    auto result = bookService->addBook("some/path.pdf");

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsDeletingABook)
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

TEST_F(ABookService, FailsDeletingABookIfBookDoesNotExist)
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

TEST_F(ABookService, SucceedsUninstallingABook)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &BookService::dataChanged);

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
    EXPECT_FALSE(bookService->getBook(bookUuid)->getDownloaded());

    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(bookService->getBookIndex(bookUuid), arguments[0].toInt());
}

TEST_F(ABookService, FailsUninstallingABookIfBookDoesNotExist)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &BookService::dataChanged);

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

TEST_F(ABookService, SucceedsUpdatingABook)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &BookService::dataChanged);

    // Create book
    QString originalPath = "/some/path.pdf";
    BookMetaData bookMetaData { .title = "SomeBook", .authors = "SomeAuthor" };
    bookService->addBook(originalPath);
    auto originalUuid =
        bookService->getBooks()[0].getUuid().toString(QUuid::WithoutBraces);

    // Create book-for-update with
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

TEST_F(ABookService, FailsUpdatingABookIfBookDoesNotExist)
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

// TEST_F(ABookService, FailsUninstallingABookIfBookDoesNotExist)
//{
//     // Arrange
//     auto uuid = QUuid::createUuid();

//    auto expectedResult = BookOperationStatus::BookDoesNotExist;


//    // Expect
//    //    EXPECT_CALL(bookStorageManagerMock, uninstallBook(_)).Times(0);

//    // Act
//    auto result = bookService->changeBookCover(uuid);

//    // Assert
//    EXPECT_EQ(expectedResult, result);
//    EXPECT_EQ(0, spy.count());
//}

TEST_F(ABookService, SucceedsGettingABook)
{
    // Arrange
    QString path = "some/path.pdf";
    BookMetaData bookMetaData { .title = "SomeTitle", .authors = "SomeAuthor" };
    Book expectedResult(path, bookMetaData);


    // Expect
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
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

TEST_F(ABookService, FailsGettingABookIfBookDoesNotExist)
{
    // Arrange
    QString bookUuid = "non-existend-uuid";

    auto expectedResult = nullptr;


    // Act
    auto* result = bookService->getBook(bookUuid);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsAddingATag)
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

TEST_F(ABookService, FailsAddingATagIfTagAlreadyExists)
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

TEST_F(ABookService, FailsAddingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = "non-existend-uuid";
    Tag firstTag("FirstTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->addTagToBook(bookUuid, firstTag);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsRemovingATag)
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

TEST_F(ABookService, FailsRemovingATagIfTagDoesNotExist)
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

TEST_F(ABookService, FailsRemovingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = "non-existend-uuid";
    Tag someTag("SomeTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->removeTagFromBook(bookUuid, someTag.getUuid());

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsRenamingATag)
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

TEST_F(ABookService, FailsRenamingATagIfTagDoesNotExist)
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

TEST_F(ABookService, FailsRenamingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = "non-existend-uuid";
    Tag someTag("SomeTag");

    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    auto result = bookService->renameTagOfBook(bookUuid, someTag.getUuid(),
                                               someTag.getUuid().toString());

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsGettingAllBooks)
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
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
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

TEST_F(ABookService, SucceedsGettingABookIndex)
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
    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
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

TEST_F(ABookService, FailsGettingABookIndexIfBookDoesNotExist)
{
    // Arrange
    QUuid nonExistentBook = QUuid::createUuid();

    int expectedResult = -1;

    // Act
    auto result = bookService->getBookIndex(nonExistentBook);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, SucceedsGettingTheBookCount)
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

TEST_F(ABookService, SucceedsRefreshingLastOpened)
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

TEST_F(ABookService, SucceedsMergingARemoteBookWitUpdatedData)
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
        .lastModified = QDateTime::currentDateTimeUtc().addDays(-2),
    };

    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);

    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    QString filePath = "some/path.pdf";
    bookService->addBook(filePath);
    const Book& localBook = bookService->getBooks()[0];
    auto localBookUuid = localBook.getUuid().toString(QUuid::WithoutBraces);

    BookMetaData remoteBookMetaData {
        .title = "DifferentBook",
        .authors = "DifferentAuthor",
        .creator = "DifferentCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),  // More recent
    };
    Book remoteBook("", remoteBookMetaData, 0, localBookUuid);


    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.loadingRemoteBooksFinished(myBooks);

    // Assert
    EXPECT_EQ(remoteBook.getTitle(), localBook.getTitle());
    EXPECT_EQ(remoteBook.getAuthors(), localBook.getAuthors());
    EXPECT_EQ(remoteBook.getCreator(), localBook.getCreator());
    EXPECT_EQ(remoteBook.getLastModified(), localBook.getLastModified());
    EXPECT_EQ(remoteBook.getLastModified(), localBook.getLastModified());
    // Check that the local file path was not overwritten
    EXPECT_EQ(filePath, localBook.getFilePath());
}

TEST_F(ABookService, SucceedsMergingARemoteBookWithANewCurrentPage)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeBook",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc().addDays(-2),
    };

    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);

    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(metaData));

    bookService->addBook("some/path.pdf");
    const Book& localBook = bookService->getBooks()[0];
    auto localBookUuid = localBook.getUuid().toString(QUuid::WithoutBraces);

    Book remoteBook("", metaData, 0, localBookUuid);
    remoteBook.setLastOpened(QDateTime::currentDateTimeUtc());
    remoteBook.setCurrentPage(21);

    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.loadingRemoteBooksFinished(myBooks);

    // Assert
    EXPECT_EQ(remoteBook.getLastOpened(), localBook.getLastOpened());
    EXPECT_EQ(remoteBook.getCurrentPage(), localBook.getCurrentPage());
}

TEST_F(ABookService, SucceedsMergingALocalBookDataToServer)
{
    // Arrange
    BookMetaData localBookMetaData {
        .title = "NewTitle",
        .authors = "NewAuthor",
        .creator = "NewCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),  // More recent
    };

    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);

    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    bookService->addBook("some/path.pdf");
    const Book& localBook = bookService->getBooks()[0];
    auto localBookUuid = localBook.getUuid().toString(QUuid::WithoutBraces);

    BookMetaData remoteBookMetaData {
        .title = "OldTitle",
        .authors = "OldAuthor",
        .creator = "OldCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc().addDays(-2),
    };
    Book remoteBook("", remoteBookMetaData, 0, localBookUuid);


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely(_)).Times(1);

    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.loadingRemoteBooksFinished(myBooks);
}

TEST_F(ABookService, SucceedsMergingALocalBookWithANewCurrentPageToServer)
{
    // Arrange
    BookMetaData localBookMetaData {
        .title = "NewTitle",
        .authors = "NewAuthor",
        .creator = "NewCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),
    };


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely(_)).Times(1);
    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(1);

    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    // Arrange
    bookService->addBook("some/path.pdf");
    auto localBookUuid =
        bookService->getBooks()[0].getUuid().toString(QUuid::WithoutBraces);

    auto localBook = bookService->getBook(localBookUuid);
    localBook->setLastOpened(QDateTime::currentDateTimeUtc());  // More recent
    localBook->setCurrentPage(10);

    Book remoteBook("", localBookMetaData, 0, localBookUuid);
    remoteBook.setLastOpened(QDateTime::currentDateTimeUtc().addDays(-1));
    remoteBook.setCurrentPage(251);

    // Act
    std::vector<Book> myBooks { remoteBook };
    emit bookStorageManagerMock.loadingRemoteBooksFinished(myBooks);
}

TEST_F(ABookService, SucceedsAddingLocalBooksToServer)
{
    // Arrange
    BookMetaData localBookMetaData {
        .title = "NewTitle",
        .authors = "NewAuthor",
        .creator = "NewCreator",
        .creationDate = "20.01.2022",
        .format = "pdf",
        .documentSize = "2MiB",
        .pagesSize = "200 x 400",
        .pageCount = 151,
        .lastModified = QDateTime::currentDateTimeUtc(),  // More recent
    };

    EXPECT_CALL(bookStorageManagerMock, addBook(_)).Times(2);

    EXPECT_CALL(bookMetaDataHelperMock, getBookMetaData(_))
        .Times(1)
        .WillOnce(Return(localBookMetaData));

    bookService->addBook("some/path.pdf");

    // Act
    std::vector<Book> myBooks {};
    emit bookStorageManagerMock.loadingRemoteBooksFinished(myBooks);
    ;
}

}  // namespace tests::application