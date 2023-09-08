#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "book.hpp"
#include "book_for_deletion.hpp"
#include "i_library_storage_gateway.hpp"
#include "i_local_library_tracker.hpp"
#include "library_storage_manager.hpp"


using namespace testing;
using namespace application::utility;
using namespace application;
using namespace domain::entities;
using namespace domain::value_objects;

namespace tests::application
{

class LibraryStorageGatewayMock : public ILibraryStorageGateway
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
    MOCK_METHOD(void, downloadBookMedia, (const QString&, const QUuid&),
                (override));
};

class LocalLibraryTrackerMock : public ILocalLibraryTracker
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

struct ALibraryStorageManager : public ::testing::Test
{
    void SetUp() override
    {
        bookStorageManager = std::make_unique<managers::LibraryStorageManager>(
            &bookStorageGatewayMock, &downloadedBooksTrackerMock);
    }

    LibraryStorageGatewayMock bookStorageGatewayMock;
    LocalLibraryTrackerMock downloadedBooksTrackerMock;
    std::unique_ptr<managers::LibraryStorageManager> bookStorageManager;
};

TEST_F(ALibraryStorageManager, SucceedsAddingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, trackBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, createBook(_, _)).Times(1);

    // Act
    bookStorageManager->addBook(book);
}

TEST_F(ALibraryStorageManager, SucceedsAddingLocalBookToRemoteLibrary)
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

TEST_F(ALibraryStorageManager, SucceedsDeletingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    BookForDeletion bookForDeletion {
        .uuid = QUuid(QUuid::createUuid().toString()),
        .downloaded = true,
    };

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageManager->deleteBook(bookForDeletion);
}

TEST_F(ALibraryStorageManager, SucceedsDeletingABookWhenBookIsNotDownloaded)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    BookForDeletion bookForDeletion {
        .uuid = QUuid(QUuid::createUuid().toString()),
        .downloaded = false,
    };

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(0);
    EXPECT_CALL(bookStorageGatewayMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageManager->deleteBook(bookForDeletion);
}

TEST_F(ALibraryStorageManager, SucceedsUninstallingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData { .format = "pdf" });

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, untrackBook(_)).Times(1);

    // Act
    bookStorageManager->uninstallBook(book);
}

TEST_F(ALibraryStorageManager, SucceedsUpdatingABook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(1);

    // Act
    bookStorageManager->updateBook(book);
}

TEST_F(ALibraryStorageManager, SucceedsUpdatingABookLocally)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(1);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(0);

    // Act
    bookStorageManager->updateBookLocally(book);
}

TEST_F(ALibraryStorageManager, FailsUpdatingABookLocallyIfItsNotDownloaded)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});
    book.setDownloaded(false);

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(0);

    // Act
    bookStorageManager->updateBookLocally(book);
}

TEST_F(ALibraryStorageManager, SucceedsUpdatingABookRemotely)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, updateTrackedBook(_)).Times(0);
    EXPECT_CALL(bookStorageGatewayMock, updateBook(_, _)).Times(1);

    // Act
    bookStorageManager->updateBookRemotely(book);
}

TEST_F(ALibraryStorageManager, SucceedsLoadingLocalBooks)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(downloadedBooksTrackerMock, getTrackedBooks()).Times(1);

    // Act
    bookStorageManager->loadLocalBooks();
}

TEST_F(ALibraryStorageManager, SucceedsLoadingRemoteBooks)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, getBooksMetaData(_)).Times(1);

    // Act
    bookStorageManager->downloadRemoteBooks();
}

TEST_F(ALibraryStorageManager, SucceedsDownloadingBook)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, downloadBookMedia(_, _)).Times(1);

    // Act
    bookStorageManager->downloadBookMedia(book.getUuid());
}

TEST_F(ALibraryStorageManager, SucceedsChangingBookCoverRemotely)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});
    book.setHasCover(true);

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, changeBookCover(_, _, _)).Times(1);

    // Act
    bookStorageManager->updateBookCoverRemotely(book.getUuid(),
                                                book.hasCover());
}

TEST_F(ALibraryStorageManager, SucceedsDeletingBookCoverRemotely)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});
    book.setHasCover(false);

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, deleteBookCover(_, _)).Times(1);

    // Act
    bookStorageManager->updateBookCoverRemotely(book.getUuid(),
                                                book.hasCover());
}

TEST_F(ALibraryStorageManager, SucceedsDownloadingBookCover)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData {});

    // Expect
    EXPECT_CALL(bookStorageGatewayMock, getCoverForBook(_, _)).Times(1);

    // Act
    bookStorageManager->downloadBookCover(book.getUuid());
}

}  // namespace tests::application
