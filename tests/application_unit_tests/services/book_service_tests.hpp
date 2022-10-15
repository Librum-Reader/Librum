#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QSignalSpy>
#include <QUuid>
#include "book.hpp"
#include "book_operation_status.hpp"
#include "i_book_info_helper.hpp"
#include "book_service.hpp"
#include "tag.hpp"


using namespace testing;
using ::testing::ReturnRef;
using application::BookOperationStatus;
using namespace application::services;
using namespace application;
using namespace domain;


namespace tests::application
{

class BookInfoHelperMock : public IBookInfoHelper
{
public:
    MOCK_METHOD(bool, setupDocument, (const QString& filePath, int w, int h), (override));
    MOCK_METHOD(QString, getTitle, (), (const, override));
    MOCK_METHOD(QString, getAuthor, (), (const, override));
    MOCK_METHOD(QString, getCreator, (), (const, override));
    MOCK_METHOD(int, getPageCount, (), (const, override));
    MOCK_METHOD(QString, getCreationDate, (), (const, override));
    MOCK_METHOD(QString, getFormat, (), (const, override));
    MOCK_METHOD(QString, getDocumentSize, (), (const, override));
    MOCK_METHOD(QString, getPagesSize, (), (const, override));
    MOCK_METHOD(void, getCover, (), (const, override));
};


struct ABookService : public ::testing::Test
{
    void SetUp() override
    {
        EXPECT_CALL(bookInfoHelperMock, setupDocument(_,_,_))
                .WillRepeatedly(Return(true));
        
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



TEST_F(ABookService, SucceedsDeletingABook)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    bookService->addBook("some/other/path.pdf");
    const auto& firstBookUuid = bookService->getBooks()[0].getUuid();
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Act
    auto preDeleteBookCount = bookService->getBookCount();
    auto result = bookService->deleteBook(firstBookUuid);
    
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



TEST_F(ABookService, SucceedsUpdatingABook)
{
    // Arrange
    QSignalSpy spy(bookService.get(), &BookService::dataChanged);
    
    QString originalTitle = "SomeBook";
    QString originalAuthor = "SomeAuthor";
    QString originalPath = "/some/path.pdf";
    
    models::Book bookToUpdateWith("NewTitle", "NewAuthor", "NewPath");
    bookToUpdateWith.addTag(models::Tag("FirstTag"));
    bookToUpdateWith.addTag(models::Tag("FirstTag"));
    
    auto expectedStatus = BookOperationStatus::Success;
    auto expectedResult = bookToUpdateWith;
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(originalTitle));
    
    EXPECT_CALL(bookInfoHelperMock, getAuthor())
            .Times(1)
            .WillOnce(Return(originalAuthor));
    
    // Act
    bookService->addBook(originalPath);
    const QUuid& bookUuid = bookService->getBooks()[0].getUuid();
    
    auto resultStatus = bookService->updateBook(bookUuid, bookToUpdateWith);
    auto result = bookService->getBook(bookUuid);
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
    EXPECT_EQ(expectedResult.getTitle(), result->getTitle());
    EXPECT_EQ(expectedResult.getFilePath(), result->getFilePath());
    
    for(int i = 0; i < expectedResult.getTags().size(); ++i)
    {
        EXPECT_EQ(expectedResult.getTags()[i], result->getTags()[i]);
    }
    
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(bookService->getBookIndex(bookUuid), arguments[0].toInt());
}

TEST_F(ABookService, FailsUpdatingABookIfBookDoesNotExist)
{
    // Arrange
    QString bookUuid = "non-existend-uuid";
    models::Book bookToUpdateWidth("SomeUpdatedTitle", "SomeUpdatedAuthor",
                                   "SomeUpdaedPath");
    
    auto expectedStatus = BookOperationStatus::BookDoesNotExist;
    
    
    // Act
    auto resultStatus = bookService->updateBook(bookUuid, bookToUpdateWidth);
    
    // Assert
    EXPECT_EQ(expectedStatus, resultStatus);
}



TEST_F(ABookService, SucceedsGettingABook)
{
    // Arrange
    QString title = "SomeBook";
    QString path = "some/path.pdf";
    QString author = "SomeAuthor";
    
    models::Book expectedResult(title, author, path);
    
    
    // Expect
    EXPECT_CALL(bookInfoHelperMock, getTitle())
            .Times(1)
            .WillOnce(Return(title));
    
    EXPECT_CALL(bookInfoHelperMock, getAuthor())
            .Times(1)
            .WillOnce(Return(author));
    
    // Act
    bookService->addBook(path);
    const auto& bookUuid = bookService->getBooks()[0].getUuid();
    
    auto result = bookService->getBook(bookUuid);
    
    // Assert
    EXPECT_EQ(expectedResult.getTitle(), result->getTitle());
    EXPECT_EQ(expectedResult.getAuthor(), result->getAuthor());
    EXPECT_EQ(expectedResult.getFilePath(), result->getFilePath());
}

TEST_F(ABookService, FailsGettingABookIfBookDoesNotExist)
{
    // Arrange
    QString bookUuid = "non-existend-uuid";
    
    auto expectedResult = nullptr;
    
    
    // Act
    auto result = bookService->getBook(bookUuid);
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookService, SucceedsAddingATag)
{
    // Arrange
    models::Tag firstTag("FirstTag");
    models::Tag secondTag("SecondTag");
    auto expectedResultStatus = BookOperationStatus::Success;
    
    
    // Act
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();
    
    auto firstResultStatus = bookService->addTag(bookUuid, firstTag);
    auto secondResultStatus = bookService->addTag(bookUuid, secondTag);
    
    auto result = bookService->getBook(bookUuid);
    
    // Assert
    EXPECT_EQ(expectedResultStatus, firstResultStatus);
    EXPECT_EQ(expectedResultStatus, secondResultStatus);
    
    EXPECT_EQ(firstTag, result->getTags()[0]);
    EXPECT_EQ(secondTag, result->getTags()[1]);
}

TEST_F(ABookService, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    models::Tag tag("FirstTag");
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    
    
    // Act
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();
    
    bookService->addTag(bookUuid, tag);
    auto result = bookService->addTag(bookUuid, tag);
    
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookService, FailsAddingATagIfBookDoesNotExist)
{
    // Arrange
    QUuid bookUuid = "non-existend-uuid";
    models::Tag firstTag("FirstTag");
    
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    auto result = bookService->addTag(bookUuid, firstTag);
    
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
            .WillOnce(Return(firstBook.getTitle()))
            .WillOnce(Return(secondBook.getTitle()))
            .WillOnce(Return(thirdBook.getTitle()));
    
    EXPECT_CALL(bookInfoHelperMock, getAuthor())
            .Times(3)
            .WillOnce(Return(firstBook.getAuthor()))
            .WillOnce(Return(secondBook.getAuthor()))
            .WillOnce(Return(thirdBook.getAuthor()));
    
    // Act
    bookService->addBook(firstBook.getFilePath());
    bookService->addBook(secondBook.getFilePath());
    bookService->addBook(thirdBook.getFilePath());
    
    auto results = bookService->getBooks();
    
    // Assert
    for(int i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_EQ(expectedResult[i].getTitle(), results[i].getTitle());
        EXPECT_EQ(expectedResult[i].getAuthor(), results[i].getAuthor());
        EXPECT_EQ(expectedResult[i].getFilePath(), results[i].getFilePath());
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



TEST_F(ABookService, SucceedsRefreshingLastOpenedFlag)
{
    // Arrange
    bookService->addBook("some/path.pdf");
    const auto& bookUuid = bookService->getBooks()[0].getUuid();
    
    // Act
    auto before = bookService->getBook(bookUuid)->getLastOpened();
    bookService->refreshLastOpenedFlag(bookUuid);
    auto after = bookService->getBook(bookUuid)->getLastOpened();
    
    // Assert
    EXPECT_NE(before, after);
}

} // namespace tests::application