#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include <ranges>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "local_library_tracker.hpp"


using namespace testing;
using namespace application::utility;
using namespace domain::entities;
using namespace domain::value_objects;

namespace tests::application
{


struct ALocalLibraryTracker : public ::testing::Test
{
    void SetUp() override
    {
        downloadedBooksTracker.setLibraryOwner(testLibraryName);
    }

    void TearDown() override
    {
        auto libraryDir = downloadedBooksTracker.getLibraryDir();
        libraryDir.removeRecursively();
    }

    LocalLibraryTracker downloadedBooksTracker;

private:
    QString testLibraryName = "201xlibrum_local_library_testsatlibrum";
};

TEST_F(ALocalLibraryTracker, SucceedsTrackingABook)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);

    bool expectedResultStatus = true;
    const Book& expectedResult = book;


    // Act
    auto resultStatus = downloadedBooksTracker.trackBook(book);

    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
}

TEST_F(ALocalLibraryTracker, FailsTrackingABookIfBookAlreadyExists)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);

    Book secondBook = book;

    bool expectedResultStatus = false;


    // Act
    downloadedBooksTracker.trackBook(book);
    auto resultStatus = downloadedBooksTracker.trackBook(secondBook);

    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
}

TEST_F(ALocalLibraryTracker, SucceedsGettingATrackedBook)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .coverLastModified = QDateTime::currentDateTimeUtc(),
        .hasCover = true,
        .coverPath = "some/path.png",
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);

    downloadedBooksTracker.trackBook(book);

    bool expectedResultStatus = true;
    const Book& expectedResult = book;


    // Act
    auto result = downloadedBooksTracker.getTrackedBook(book.getUuid());

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(expectedResult, result.value());
}

TEST_F(ALocalLibraryTracker, FailsGettingUntrackedBook)
{
    // Arrange
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);


    // Act
    auto result = downloadedBooksTracker.getTrackedBook(QUuid(uuid));

    // Assert
    EXPECT_FALSE(result.has_value());
}

TEST_F(ALocalLibraryTracker, SucceedsGettingAllTrackedBooks)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };
    int currentPage = 224;

    auto firstUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    Book firstBook("some/path.pdf", metaData, currentPage, firstUuid);

    auto secondUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    Book secondBook("some/other/path.pdf", metaData, currentPage, secondUuid);
    secondBook.setTitle("SomeOtherBook");
    secondBook.setPageCount(820);

    auto thirdUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    Book thirdBook("some/random/path.pdf", metaData, currentPage, thirdUuid);
    secondBook.setAuthors("SomeOtherAuthor");
    secondBook.setPageCount(412);


    downloadedBooksTracker.trackBook(firstBook);
    downloadedBooksTracker.trackBook(secondBook);
    downloadedBooksTracker.trackBook(thirdBook);

    // Act
    auto result = downloadedBooksTracker.getTrackedBooks();

    // Assert
    EXPECT_TRUE(result.size() == 3);
}

TEST_F(ALocalLibraryTracker, FailsGettingAllTrackedBooksIfNoneExist)
{
    // Act
    auto result = downloadedBooksTracker.getTrackedBooks();

    // Assert
    EXPECT_TRUE(result.empty());
}

TEST_F(ALocalLibraryTracker, SucceedsUntrackingATrackedBook)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);

    downloadedBooksTracker.trackBook(book);

    bool expectedResultStatus = true;

    // Act
    auto resultStatus = downloadedBooksTracker.untrackBook(book.getUuid());
    auto result = downloadedBooksTracker.getTrackedBook(book.getUuid());


    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
    EXPECT_FALSE(result.has_value());
}

TEST_F(ALocalLibraryTracker, FailsUntrackingIfNoTrackedBookExists)
{
    // Arrange
    auto nonExistendUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    bool expectedResultStatus = false;

    // Act
    auto resultStatus =
        downloadedBooksTracker.untrackBook(QUuid(nonExistendUuid));
    auto result = downloadedBooksTracker.getTrackedBook(QUuid(nonExistendUuid));


    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
    EXPECT_FALSE(result.has_value());
}

TEST_F(ALocalLibraryTracker, SucceedsUpdatingATrackedBook)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);
    downloadedBooksTracker.trackBook(book);

    auto newBook = book;
    newBook.setTitle("SomeOtherBook");
    newBook.setAuthors("SomeOtherAuthor");

    bool expectedResultStatus = true;


    // Act
    auto resultStatus = downloadedBooksTracker.updateTrackedBook(newBook);
    auto result = downloadedBooksTracker.getTrackedBook(book.getUuid());


    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
    EXPECT_EQ(newBook.getTitle(), result.value().getTitle());
    EXPECT_EQ(newBook.getAuthors(), result.value().getAuthors());
    EXPECT_EQ(book.getFormat(), result.value().getFormat());
}

TEST_F(ALocalLibraryTracker, FailsUpdatingAnUntrackedBook)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);

    bool expectedResultStatus = false;


    // Act
    auto resultStatus = downloadedBooksTracker.updateTrackedBook(book);
    auto result = downloadedBooksTracker.getTrackedBook(book.getUuid());


    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
    EXPECT_FALSE(result.has_value());
}

}  // namespace tests::application