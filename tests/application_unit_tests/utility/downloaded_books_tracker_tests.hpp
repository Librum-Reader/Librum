#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

}