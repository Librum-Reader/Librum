#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include "book.hpp"
#include "book_operation_status.hpp"
#include "i_book_info_manager.hpp"
#include "book_service.hpp"
#include "tag.hpp"


using namespace testing;
using ::testing::ReturnRef;
using application::BookOperationStatus;
using namespace application::services;
using namespace domain;


class BookInfoManagerMock : public application::IBookInfoManager
{
public:
    MOCK_METHOD(QString, getBookTitle, (const QString& filePath), (override));
    MOCK_METHOD(QByteArray, getBookCover, (const QString& filePath), (override));
};




TEST(ABookService, SucceedsAddingABook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return("SomeBook"));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    auto result = bookService.addBook("some/path.pdf");
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(ABookService, FailsAddingABookIfBookAlreadyExists)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = BookOperationStatus::BookAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(2)
            .WillRepeatedly(Return("SomeBook"));  // Always returns the same title
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/first.pdf");  // First time added
    auto result = bookService.addBook("some/second.pdf");  // Second time added
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST(ABookService, SucceedsDeletingABook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString bookTitle = "SomeBook";
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(2)
            .WillOnce(Return(bookTitle))
            .WillOnce(Return("SomeOtherBook"));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(2)
            .WillOnce(Return("SomeCover"))
            .WillOnce(Return("SomeOtherCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    bookService.addBook("some/other/path.pdf");
    auto previousBookCount = bookService.getBookCount();
    
    auto result = bookService.deleteBook(bookTitle);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
    EXPECT_EQ(previousBookCount -1 , bookService.getBookCount());
}

TEST(ABookService, FailsDeletingABookIfBookDoesNotExist)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = BookOperationStatus::BookAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(2)
            .WillRepeatedly(Return("SomeBook"));  // Always returns the same title
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/first.pdf");  // First time added
    auto result = bookService.addBook("some/second.pdf");  // Second time added
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST(ABookService, SucceedsUpdatingABook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString originalBookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    models::Book bookToUpdateWidth("SomeUpdatedTitle", "SomeUpdaedPath", 
                                   "SomeUpdatedCover");
    bookToUpdateWidth.addTag(firstTag);
    bookToUpdateWidth.addTag(secondTag);
    
    auto expectedStatus = BookOperationStatus::Success;
    auto expectedResult = bookToUpdateWidth;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return(originalBookTitle));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    
    auto resultStatus = bookService.updateBook(originalBookTitle, bookToUpdateWidth);
    auto result = bookService.getBook(bookToUpdateWidth.title());
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_NE(result, nullptr);
    
    EXPECT_EQ(expectedResult.title(), result->title());
    EXPECT_EQ(expectedResult.filePath(), result->filePath());
    EXPECT_EQ(expectedResult.cover(), result->cover());
    
    for(int i = 0; i < expectedResult.tags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags()[i], result->tags()[i]);
    }
}

TEST(ABookService, FailsUpdatingABookIfBookDoesNotExist)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString originalBookTitle = "SomeBook";
    models::Book bookToUpdateWidth("SomeUpdatedTitle", "SomeUpdaedPath", 
                                   "SomeUpdatedCover");
    
    auto expectedStatus = BookOperationStatus::BookDoesNotExist;
    auto expectedResult = bookToUpdateWidth;
    
    
    // Act
    auto resultStatus = bookService.updateBook(originalBookTitle, bookToUpdateWidth);
    auto result = bookService.getBook(bookToUpdateWidth.title());
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_EQ(result, nullptr);
}



TEST(ABookService, SucceedsGettingABook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString title = "SomeBook";
    QString path = "some/path.pdf";
    QByteArray cover = "SomeCover";
    
    models::Book expectedResult(title, path, cover);
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return(title));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return(cover));
    
    // Act
    bookService.addBook(path);
    auto result = bookService.getBook(title);
    
    // Assert
    EXPECT_EQ(expectedResult.title(), result->title());
    EXPECT_EQ(expectedResult.filePath(), result->filePath());
    EXPECT_EQ(expectedResult.cover(), result->cover());
    
    for(int i = 0; i < expectedResult.tags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags()[i].name(), result->tags()[i].name());
    }
}

TEST(ABookService, FailsGettingABookIfBookDoesNotExist)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString title = "SomeBook";
    
    auto expectedResult = nullptr;
    
    
    // Act
    auto result = bookService.getBook(title);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST(ABookService, SucceedsAddingATag)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    

    QString bookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    auto expectedResultStatus = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return(bookTitle));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    
    auto firstResultStatus = bookService.addTag(bookTitle, firstTag);
    auto secondResultStatus = bookService.addTag(bookTitle, secondTag);
    
    auto result = bookService.getBook(bookTitle);
    
    // Assert
    EXPECT_EQ(expectedResultStatus, firstResultStatus);
    EXPECT_EQ(expectedResultStatus, secondResultStatus);
    
    EXPECT_EQ(firstTag, result->tags()[0]);
    EXPECT_EQ(secondTag, result->tags()[1]);
}

TEST(ABookService, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    

    QString bookTitle = "SomeBook";
    
    models::Tag tag("FirstTag");
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return(bookTitle));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    
    bookService.addTag(bookTitle, tag);
    auto result = bookService.addTag(bookTitle, tag);
    
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(ABookService, FailsAddingATagIfBookDoesNotExist)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    

    QString bookTitle = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    auto result = bookService.addTag(bookTitle, firstTag);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST(ABookService, SucceedsSettingTheCurrentBook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString bookTitle = "SomeBook";
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(1)
            .WillOnce(Return("SomeBook"));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(1)
            .WillOnce(Return("SomeCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    
    auto result = bookService.setCurrentBook(bookTitle);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(ABookService, FailsSettingTheCurrentBookIfTheBookDoesNotExist)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    // Act
    auto result = bookService.setCurrentBook("NonExistentBook");
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST(ABookService, SucceedsGettingTheCurrentBook)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    QString bookTitle = "SomeBook";
    QString bookPath = "some/book.pdf";
    QByteArray bookCover = "SomeBook";
    
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    models::Book expectedResult(bookTitle, bookPath, bookCover);
    expectedResult.addTag(firstTag);
    expectedResult.addTag(secondTag);
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(2)
            .WillOnce(Return("SomeRandomBook"))
            .WillOnce(Return(bookTitle));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(2)
            .WillOnce(Return("SomeRandomCover"))
            .WillOnce(Return(bookCover));
    
    // Act
    bookService.addBook("random/book/path.pdf");
    bookService.addBook(bookPath);
    bookService.addTag(bookTitle, firstTag);
    bookService.addTag(bookTitle, secondTag);
    
    bookService.setCurrentBook(bookTitle);
    
    auto result = bookService.getCurrentBook();
    
    // Assert
    EXPECT_EQ(expectedResult.title(), result->title());
    EXPECT_EQ(expectedResult.filePath(), result->filePath());
    EXPECT_EQ(expectedResult.cover(), result->cover());
    
    for(int i = 0; i < expectedResult.tags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags()[i], result->tags()[i]);
    }
}

TEST(ABookService, FailsGettingTheCurrentBookIfNoCurrentBookExists)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = nullptr;
    
    
    // Act
    auto result = bookService.getCurrentBook();
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}


TEST(ABookService, SucceedsGettingAllBooks)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    models::Book firstBook("FirstBook", "FirstFilePath", "FirstCover");
    models::Book secondBook("SecondBook", "SecondFilePath", "SecondCover");
    models::Book thirdBook("ThirdBook", "ThirdFilePath", "ThirdCover");
    std::vector<models::Book> expectedResult { firstBook, secondBook, thirdBook };
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(3)
            .WillOnce(Return(firstBook.title()))
            .WillOnce(Return(secondBook.title()))
            .WillOnce(Return(thirdBook.title()));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(3)
            .WillOnce(Return(firstBook.cover()))
            .WillOnce(Return(secondBook.cover()))
            .WillOnce(Return(thirdBook.cover()));
    
    // Act
    bookService.addBook(firstBook.filePath());
    bookService.addBook(secondBook.filePath());
    bookService.addBook(thirdBook.filePath());
    
    auto results = bookService.getBooks();
    
    // Assert
    for(int i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_EQ(expectedResult[i].title(), results[i].title());
        EXPECT_EQ(expectedResult[i].filePath(), results[i].filePath());
        EXPECT_EQ(expectedResult[i].cover(), results[i].cover());
    }
}

TEST(ABookService, SucceedsGettingTheBookCount)
{
    // Arrange
    BookInfoManagerMock bookInfoManagerMock;
    application::services::BookService bookService(&bookInfoManagerMock);
    
    auto expectedResult = 2;
    
    
    // Expect
    EXPECT_CALL(bookInfoManagerMock, getBookTitle(_))
            .Times(2)
            .WillOnce(Return("SomeBook"))
            .WillOnce(Return("SomeOtherBook"));
    
    EXPECT_CALL(bookInfoManagerMock, getBookCover(_))
            .Times(2)
            .WillOnce(Return("SomeCover"))
            .WillOnce(Return("SomeOtherCover"));
    
    // Act
    bookService.addBook("some/path.pdf");
    bookService.addBook("some/other/path.pdf");
    
    auto result = bookService.getBookCount();
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}