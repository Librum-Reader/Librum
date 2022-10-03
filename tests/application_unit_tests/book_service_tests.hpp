#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include "book.hpp"
#include "book_operation_status.hpp"
#include "i_book_info_helper.hpp"
#include "book_service.hpp"
#include "tag.hpp"


using namespace testing;
using ::testing::ReturnRef;
using application::BookOperationStatus;
using namespace application::services;
using namespace domain;


class BookInfoHelperMock : public application::IBookInfoHelper
{
public:
    MOCK_METHOD(bool, setupDocument, (const QString& filePath), (override));
    MOCK_METHOD(QString, getTitle, (), (override));
    MOCK_METHOD(void, getCover, (), (override));
    MOCK_METHOD(QString, getAuthor, (), (override));
};


struct ABookService : public ::testing::Test
{
    void SetUp() override
    {
        // Default actions
        EXPECT_CALL(bookInfoHelperMock, setupDocument(_))
                .WillRepeatedly(Return(true));
        
        EXPECT_CALL(bookInfoHelperMock, getTitle())
                .WillRepeatedly(Return("SomeTitle"));
        
        EXPECT_CALL(bookInfoHelperMock, getAuthor())
                .WillRepeatedly(Return("SomeAuthor"));
        
        EXPECT_CALL(bookInfoHelperMock, getCover())
                .WillRepeatedly(Return());
        
        
        bookService = std::make_unique<BookService>(&bookInfoHelperMock);
    }
    
    BookInfoHelperMock bookInfoHelperMock;
    std::unique_ptr<BookService> bookService;
};



TEST_F(ABookService, SucceedsAddingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    // Act
    auto result = bookService->addBook("some/path.pdf");
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, FailsAddingABookIfBookAlreadyExists)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(2)
            .WillRepeatedly(Return("SomeBook"));  // Returns the same title twice
    
    
    // Act
    bookService->addBook("some/first.pdf");  // First time added
    auto result = bookService->addBook("some/second.pdf");  // Second time added
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookService, SucceedsDeletingABook)
{
    // Arrange
    QString bookTitle = "SomeBook";
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(2)
            .WillOnce(Return(bookTitle))
            .WillOnce(Return("SomeOtherBook"));
    
    // Act
    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");
    auto previousBookCount = bookService->getBookCount();
    
    auto result = bookService->deleteBook(bookTitle);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
    EXPECT_EQ(previousBookCount - 1, bookService->getBookCount());
}

TEST_F(ABookService, FailsDeletingABookIfBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return("SomeBook"));
    
    // Act
    bookService->addBook("some/first.pdf");
    
    auto result = bookService->deleteBook("SomeNonExistentBook");
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookService, SucceedsUpdatingABook)
{
    // Arrange
    QString originalBookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    models::Book bookToUpdateWith("SomeUpdatedTitle", "SomeUpdatedAuthor",
                                   "SomeUpdaedPath", QImage("SomeUpdatedCover"));
    bookToUpdateWith.addTag(firstTag);
    bookToUpdateWith.addTag(secondTag);
    
    auto expectedStatus = BookOperationStatus::Success;
    auto expectedResult = bookToUpdateWith;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(originalBookTitle));
    
    // Act
    bookService->addBook("some/path.pdf");
    
    auto resultStatus = bookService->updateBook(originalBookTitle, bookToUpdateWith);
    auto result = bookService->getBook(bookToUpdateWith.title());
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_NE(result, nullptr);
    
    EXPECT_EQ(expectedResult.title(), result->title());
    EXPECT_EQ(expectedResult.filePath(), result->filePath());
    
    for(int i = 0; i < expectedResult.tags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags()[i], result->tags()[i]);
    }
}

TEST_F(ABookService, FailsUpdatingABookIfBookDoesNotExist)
{
    // Arrange
    QString originalBookTitle = "SomeBook";
    models::Book bookToUpdateWidth("SomeUpdatedTitle", "SomeUpdatedAuthor",
                                   "SomeUpdaedPath", QImage("SomeUpdatedCover"));
    
    auto expectedStatus = BookOperationStatus::BookDoesNotExist;
    auto expectedResult = bookToUpdateWidth;
    
    
    // Act
    auto resultStatus = bookService->updateBook(originalBookTitle, bookToUpdateWidth);
    auto result = bookService->getBook(bookToUpdateWidth.title());
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_EQ(result, nullptr);
}



TEST_F(ABookService, SucceedsGettingABook)
{
    // Arrange
    QString title = "SomeBook";
    QString author = "SomeAuthor";
    QString path = "some/path.pdf";
    QImage cover("SomeCover");
    
    models::Book expectedResult(title, author, path, cover);
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(title));
    
    // Act
    bookService->addBook(path);
    auto result = bookService->getBook(title);
    
    // Assert
    EXPECT_EQ(expectedResult.title(), result->title());
    EXPECT_EQ(expectedResult.filePath(), result->filePath());
    
    for(int i = 0; i < expectedResult.tags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags()[i].name(), result->tags()[i].name());
    }
}

TEST_F(ABookService, FailsGettingABookIfBookDoesNotExist)
{
    // Arrange
    QString title = "SomeBook";
    
    auto expectedResult = nullptr;
    
    
    // Act
    auto result = bookService->getBook(title);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookService, SucceedsAddingATag)
{
    // Arrange
    QString bookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    auto expectedResultStatus = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(bookTitle));
    
    // Act
    bookService->addBook("some/path.pdf");
    
    auto firstResultStatus = bookService->addTag(bookTitle, firstTag);
    auto secondResultStatus = bookService->addTag(bookTitle, secondTag);
    
    auto result = bookService->getBook(bookTitle);
    
    // Assert
    EXPECT_EQ(expectedResultStatus, firstResultStatus);
    EXPECT_EQ(expectedResultStatus, secondResultStatus);
    
    EXPECT_EQ(firstTag, result->tags()[0]);
    EXPECT_EQ(secondTag, result->tags()[1]);
}

TEST_F(ABookService, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    QString bookTitle = "SomeBook";
    
    models::Tag tag("FirstTag");
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(bookTitle));
    
    // Act
    bookService->addBook("some/path.pdf");
    
    bookService->addTag(bookTitle, tag);
    auto result = bookService->addTag(bookTitle, tag);
    
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, FailsAddingATagIfBookDoesNotExist)
{
    // Arrange
    QString bookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    auto result = bookService->addTag(bookTitle, firstTag);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}


TEST_F(ABookService, SucceedsGettingAllBooks)
{
    // Arrange
    models::Book firstBook("FirstBook", "Author1", "FirstFilePath", QImage("FirstCover"));
    models::Book secondBook("SecondBook", "Author2", "SecondFilePath", QImage("SecondCover"));
    models::Book thirdBook("ThirdBook", "Author3", "ThirdFilePath", QImage("ThirdCover"));
    std::vector<models::Book> expectedResult { firstBook, secondBook, thirdBook };
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(3)
            .WillOnce(Return(firstBook.title()))
            .WillOnce(Return(secondBook.title()))
            .WillOnce(Return(thirdBook.title()));
    
    EXPECT_CALL(bookInfoHelperMock, getAuthor())
            .Times(3)
            .WillOnce(Return(firstBook.author()))
            .WillOnce(Return(secondBook.author()))
            .WillOnce(Return(thirdBook.author()));
    
    // Act
    bookService->addBook(firstBook.filePath());
    bookService->addBook(secondBook.filePath());
    bookService->addBook(thirdBook.filePath());
    
    auto results = bookService->getBooks();
    
    // Assert
    for(int i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_EQ(expectedResult[i].title(), results[i].title());
        EXPECT_EQ(expectedResult[i].author(), results[i].author());
        EXPECT_EQ(expectedResult[i].filePath(), results[i].filePath());
    }
}

TEST_F(ABookService, SucceedsGettingTheBookCount)
{
    // Arrange
    auto expectedResult = 2;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(2)
            .WillOnce(Return("SomeBook"))
            .WillOnce(Return("SomeOtherBook"));
    
    // Act
    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");
    
    auto result = bookService->getBookCount();
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}