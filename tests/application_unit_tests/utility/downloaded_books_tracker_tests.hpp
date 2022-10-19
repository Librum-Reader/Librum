#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ranges>
#include <QString>
#include "book_meta_data.hpp"
#include "book.hpp"
#include "downloaded_books_tracker.hpp"

#include <QDebug>


using namespace testing;
using namespace application::utility;
using namespace domain::models;


namespace tests::application
{


struct ADownloadedBooksTracker : public ::testing::Test
{
    void SetUp() override
    {
        downloadedBooksTracker.setLibraryOwner(testLibraryName);
    }
    
    void TearDown() override
    {
        auto libraryDir = downloadedBooksTracker.getUserLibraryDir();
        libraryDir.removeRecursively();
    }
    
    
    DownloadedBooksTracker downloadedBooksTracker;
    
private:
    QString testLibraryName = "201xlibrum_local_library_testsatlibrum";
};


TEST_F(ADownloadedBooksTracker, SucceedsTrackingABook)
{
    // Arrange
    BookMetaData metaData
    {
        .title = "SomeTitle",
        .author = "SomeAuthor",
        .creator = "SomeCreator",
        .releaseDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .addedToLibrary = "18.10.2022 - 8:54 pm",
        .lastOpened = "Never",
        .cover = QImage("")
    };
    
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path.pdf", metaData, currentPage, uuid);
    
    bool expectedResultStatus = true;
    const Book& expectedResult = book;
    
    // Act
    auto resultStatus = downloadedBooksTracker.trackBook(book);
    auto result = downloadedBooksTracker.getTrackedBook(book.getUuid());
    
    
    // Assert
    EXPECT_EQ(expectedResultStatus, resultStatus);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(expectedResult, result.value());
}

TEST_F(ADownloadedBooksTracker, SucceedsGettingAllBooks)
{
    // Arrange
    BookMetaData metaData
    {
        .title = "SomeTitle",
        .author = "SomeAuthor",
        .creator = "SomeCreator",
        .releaseDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .addedToLibrary = "18.10.2022 - 8:54 pm",
        .lastOpened = "Never",
        .cover = QImage("")
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
    secondBook.setAuthor("SomeOtherAuthor");
    secondBook.setPageCount(412);
    
    
    auto firstResultStatus = downloadedBooksTracker.trackBook(firstBook);
    auto secondResultStatus = downloadedBooksTracker.trackBook(secondBook);
    auto thirdResultStatus = downloadedBooksTracker.trackBook(thirdBook);
    
    // Act
    auto result = downloadedBooksTracker.getTrackedBooks();
            
    // Assert
    auto firstIt = std::ranges::find_if(result, [&firstBook](const Book& book) { 
        return book.getUuid() == firstBook.getUuid();  });
    
    auto secondIt = std::ranges::find_if(result, [&secondBook](const Book& book) { 
        return book.getUuid() == secondBook.getUuid(); });
    
    auto thirdIt = std::ranges::find_if(result, [&thirdBook](const Book& book) { 
        return book.getUuid() == thirdBook.getUuid(); });
    
    
    EXPECT_TRUE(firstResultStatus == secondResultStatus == thirdResultStatus == true);
    
    EXPECT_TRUE(firstIt != result.end());
    EXPECT_TRUE(secondIt != result.end());
    EXPECT_TRUE(thirdIt != result.end());
}

}