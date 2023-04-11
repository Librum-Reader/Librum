#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "book.hpp"
#include "book_for_deletion.hpp"
#include "book_storage_manager.hpp"
#include "i_book_storage_gateway.hpp"
#include "i_downloaded_books_tracker.hpp"


using namespace testing;
using namespace application::utility;
using namespace application;
using namespace domain::entities;
using namespace domain::value_objects;

namespace tests::application
{

class BookStorageGatewayMock : public IBookStorageGateway
{
public:
    MOCK_METHOD(void, createBook, (const QString&, const Book&), (override));
    MOCK_METHOD(void, deleteBook, (const QString&, const QUuid& uuid),
                (override));
    MOCK_METHOD(void, updateBook, (const QString&, const Book&), (override));
    MOCK_METHOD(void, changeBookCover,
                (const QString&, const QUuid&, const QString&), (override));
    MOCK_METHOD(void, deleteBookCover, (const QString&, const QUuid&),
                (override));
    MOCK_METHOD(void, getCoverForBook, (const QString&, const QUuid&),
                (override));
    MOCK_METHOD(void, getBooksMetaData, (const QString&), (override));
    MOCK_METHOD(void, downloadBook, (const QString&, const QUuid&), (override));
};

class DownloadedBooksTrackerMock : public IDownloadedBooksTracker
{
public:
    MOCK_METHOD(void, setLibraryOwner, (const QString&), (override));
    MOCK_METHOD(void, clearLibraryOwner, (), (override));
    MOCK_METHOD(QDir, getLibraryDir, (), (const, override));
    MOCK_METHOD(std::vector<Book>, getTrackedBooks, (), (override));
    MOCK_METHOD(std::optional<Book>, getTrackedBook, (const QUuid&),
                (override));
    MOCK_METHOD(bool, trackBook, (const Book& book), (override));
    MOCK_METHOD(bool, untrackBook, (const QUuid&), (override));
    MOCK_METHOD(bool, updateTrackedBook, (const Book&), (override));
};

struct ABookStorageManager : public ::testing::Test
{
    void SetUp() override
    {
        bookStorageManager = std::make_unique<BookStorageManager>(
            &bookStorageGatewayMock, &downloadedBooksTrackerMock);
    }

    BookStorageGatewayMock bookStorageGatewayMock;
    DownloadedBooksTrackerMock downloadedBooksTrackerMock;
    std::unique_ptr<BookStorageManager> bookStorageManager;
};

TEST_F(ABookStorageManager, SucceedsAddingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, trackBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, createBook(_, _)).Times(1);

    // Act
    bookStorageManager->addBook(book);
}

TEST_F(ABookStorageManager, SucceedsAddingLocalBookToRemoteLibrary)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});
    book.setDownloaded(false);  // Mark the book as local

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, trackBook(_)).Times(0);
    EXPECT_CALL(bookStorageGatewayMock, createBook(_, _)).Times(1);

    // Act
    bookStorageManager->addBook(book);
}

TEST_F(ABookStorageManager, SucceedsDeletingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    BookForDeletion bookForDeletion {
        .uuid = QUuid::createUuid().toString(),
        .downloaded = true,
    };

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageManager->deleteBook(bookForDeletion);
}

TEST_F(ABookStorageManager, SucceedsDeletingABookWhenBookIsNotDownloaded)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    BookForDeletion bookForDeletion {
        .uuid = QUuid::createUuid().toString(),
        .downloaded = false,
    };

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(0);
    EXPECT_CALL(bookStorageGatewayMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageManager->deleteBook(bookForDeletion);
}

TEST_F(ABookStorageManager, SucceedsUninstallingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(1);

    // Act
    bookStorageManager->uninstallBook(
        book.getUuid().toString(QUuid::WithoutBraces));
}

TEST_F(ABookStorageManager, SucceedsUpdatingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(1);

    // Act
    bookStorageManager->updateBook(book);
}

TEST_F(ABookStorageManager, SucceedsUpdatingABookLocally)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(0);

    // Act
    bookStorageManager->updateBookLocally(book);
}

TEST_F(ABookStorageManager, FailsUpdatingABookLocallyIfItsNotDownloaded)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});
    book.setDownloaded(false);

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(0);

    // Act
    bookStorageManager->updateBookLocally(book);
}

TEST_F(ABookStorageManager, SucceedsUpdatingABookRemotely)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(0);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(1);

    // Act
    bookStorageManager->updateBookRemotely(book);
}

TEST_F(ABookStorageManager, SucceedsLoadingLocalBooks)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, getTrackedBooks()).Times(1);

    // Act
    bookStorageManager->loadLocalBooks();
}

TEST_F(ABookStorageManager, SucceedsLoadingRemoteBooks)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, getBooksMetaData(_)).Times(1);

    // Act
    bookStorageManager->loadRemoteBooks();
}

}  // namespace tests::application