#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QDateTime>
#include <QSignalSpy>
#include <QString>
#include "book.hpp"
#include "book_merger.hpp"
#include "book_meta_data.hpp"
#include "gmock/gmock.h"
#include "i_library_storage_manager.hpp"


using namespace testing;
using namespace application::utility;
using domain::entities::Book;
using domain::value_objects::BookMetaData;

namespace tests::application
{

class LibraryStorageManagerMock : public ::application::ILibraryStorageManager
{
public:
    MOCK_METHOD(void, addBook, (const Book&), (override));
    MOCK_METHOD(void, addBookLocally, (const Book&), (override));
    MOCK_METHOD(void, deleteBook, (BookForDeletion), (override));
    MOCK_METHOD(void, deleteAllBooks, (), (override));
    MOCK_METHOD(void, deleteBookLocally, (BookForDeletion), (override));
    MOCK_METHOD(void, uninstallBook, (const Book& book), (override));
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

struct ABookMerger : public ::testing::Test
{
    void SetUp() override
    {
        bookMerger = std::make_unique<BookMerger>();
    }

    LibraryStorageManagerMock bookStorageManagerMock;
    std::unique_ptr<BookMerger> bookMerger;
};

Book getDefaultBook()
{
    BookMetaData metaData {
        .title = "Some Book",
        .authors = "First, Second",
        .creator = "Some creator",
        .creationDate = "Some date",
        .format = "pdf",
        .language = "english",
        .documentSize = "3MiB",
        .pagesSize = "10 inches width",
        .pageCount = 241,
        .addedToLibrary = QDateTime::currentDateTimeUtc(),
        .lastModified = QDateTime::currentDateTimeUtc(),
        .lastOpened = QDateTime::currentDateTimeUtc(),
        .coverLastModified = QDateTime::currentDateTimeUtc(),
        .hasCover = true,
        .coverPath = "some/path.png",
    };

    return Book("some/path.pdf", metaData, 0);
}

TEST_F(ABookMerger, SucceedsMergingNothingWhenBooksAreEqual)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(0);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);

    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);
}

TEST_F(ABookMerger, SucceedsMergingCurrentPageFromRemoteIntoLocal)
{
    // Arrange
    QSignalSpy spy(bookMerger.get(), &BookMerger::bookDataChanged);

    auto first = getDefaultBook();
    auto second = first;
    second.setCurrentPage(20);
    second.setLastOpened(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(0);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);

    // Assert
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(second, first);
}

TEST_F(ABookMerger, SucceedsMergingCurrentPageFromLocalIntoRemote)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;

    first.setCurrentPage(20);
    first.setLastOpened(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(1);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);
}

TEST_F(ABookMerger, SucceedsMergingBookDataFromRemoteIntoLocal)
{
    // Arrange
    QSignalSpy spy(bookMerger.get(), &BookMerger::bookDataChanged);

    auto first = getDefaultBook();
    auto second = first;
    second.setTitle("New Title");
    second.setLastModified(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(0);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);

    // Assert
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(second, first);
}

TEST_F(ABookMerger, SucceedsMergingBookDataFromLocalIntoRemote)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;

    first.setTitle("New Title");
    first.setLastModified(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(1);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);
}

TEST_F(ABookMerger, SucceedsMergingNewBookCoverFromRemoteIntoLocal)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;
    second.setCoverLastModified(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(0);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(1);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);

    // Assert
    EXPECT_EQ(second, first);
}

TEST_F(ABookMerger, SucceedsMergingDeletedBookCoverFromRemoteIntoLocal)
{
    // Arrange
    QSignalSpy spy(bookMerger.get(), &BookMerger::localBookCoverDeleted);

    auto first = getDefaultBook();
    auto second = first;
    second.setCoverLastModified(QDateTime::currentDateTimeUtc().addSecs(100));
    second.setHasCover(false);
    second.setCoverPath("");


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(0);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(1);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(0);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);

    // Assert
    EXPECT_EQ(second, first);
    EXPECT_EQ(1, spy.count());
}

TEST_F(ABookMerger, SucceedsMergingNewBookCoverFromLocalIntoRemote)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;

    first.setCoverLastModified(QDateTime::currentDateTimeUtc().addSecs(100));


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(1);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(1);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);
}

TEST_F(ABookMerger, SucceedsMergingDeletedBookCoverFromLocalIntoRemote)
{
    // Arrange
    auto first = getDefaultBook();
    auto second = first;

    first.setCoverLastModified(QDateTime::currentDateTimeUtc().addSecs(100));
    first.setCoverPath("");


    // Expect
    EXPECT_CALL(bookStorageManagerMock, updateBookLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookRemotely).Times(1);
    EXPECT_CALL(bookStorageManagerMock, downloadBookCover).Times(0);
    EXPECT_CALL(bookStorageManagerMock, deleteBookCoverLocally).Times(0);
    EXPECT_CALL(bookStorageManagerMock, updateBookCoverRemotely).Times(1);


    // Act
    bookMerger->mergeBooks(first, second, &bookStorageManagerMock);
}

}  // namespace tests::application
