#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <utility>
#include <QString>
#include <QVariantMap>
#include <QByteArray>
#include "book_controller.hpp"
#include "i_book_service.hpp"
#include "book.hpp"
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "tag_dto.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace adapters;
using namespace domain::models;
using namespace application;
using namespace application;


class BookServiceMock : public application::IBookService
{
public:
    MOCK_METHOD(BookOperationStatus, addBook, (const QString& filePath), (override));
    MOCK_METHOD(BookOperationStatus, deleteBook, (const QString& title), (override));
    MOCK_METHOD(BookOperationStatus, updateBook, (const QString& title,
                                                  const Book& book), (override));
    
    MOCK_METHOD(const std::vector<Book>&, getBooks, (), (const, override));
    MOCK_METHOD(const Book*, getBook, (const QString& title), (const, override));
    MOCK_METHOD(int, getBookCount, (), (const, override));
    MOCK_METHOD(BookOperationStatus, setCurrentBook, (const QString& title), (override));
    MOCK_METHOD(const Book*, getCurrentBook, (), (const, override));
    
    MOCK_METHOD(BookOperationStatus, addTag, (const QString& title,
                                              const domain::models::Tag& tag), (override));
    MOCK_METHOD(BookOperationStatus, removeTag, (const QString&,
                                                 const domain::models::Tag& tag), (override));
};



struct ABookController : public ::testing::Test
{
    ABookController() {}
    
    void SetUp() override
    {
        EXPECT_CALL(bookService, getBooks())
                .WillOnce(ReturnRef(bookVector));
        
        bookController = std::make_unique<controllers::BookController>(&bookService);
    }
    
    const std::vector<Book> bookVector;
    BookServiceMock bookService;
    std::unique_ptr<controllers::BookController> bookController;
};




TEST_F(ABookController, SucceedsAddingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    // Expect
    EXPECT_CALL(bookService, addBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->addBook("some/path.pdf");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsAddingABookIfTheBookAlreadyExists)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookService, addBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::BookAlreadyExists));
    
    // Act
    auto result = bookController->addBook("some/path.pdf");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsDeletingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookService, deleteBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->deleteBook("someBook");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsDeletingABookIfTheBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookService, deleteBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::BookDoesNotExist));
    
    // Act
    auto result = bookController->deleteBook("someBook");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsUpdatingABook)
{
    // Arrange
    Book bookToReturn("SomeBook");
    QVariantMap map{ {"Title", "AnotherName"}, {"Cover", QByteArray("123")} };
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookService, getBook(_))
            .Times(1)
            .WillOnce(Return(&bookToReturn));
    
    EXPECT_CALL(bookService, updateBook(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->updateBook("SomeBook", map);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfTheBookDoesNotExist)
{
    // Arrange
    QVariantMap map{ {"Title", "AnotherName"}, {"Cover", QByteArray("123")} };
    
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    // Expect
    EXPECT_CALL(bookService, getBook(_))
            .Times(1)
            .WillOnce(Return(nullptr));
    
    // Act
    auto result = bookController->updateBook("SomeBook", map);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfAPropertyDoesNotExist)
{
    // Arrange
    Book bookToReturn("SomeBook");
    QVariantMap map{ {"NonExistentProperty", QString("123")} };
    
    auto expectedResult = BookOperationStatus::PropertyDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookService, getBook(_))
            .Times(1)
            .WillOnce(Return(&bookToReturn));
    
    // Act
    auto result = bookController->updateBook("SomeBook", map);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsGettingABook)
{
    // Arrange
    QString title = "SomeBook";
    QString filePath = "some/path.pdf";
    QImage cover("0xfdd244123bc");
    QString firstTagName = "FirstTag";
    QString secondTagName = "SecondTag";
    std::vector<Book> booksToReturn{ Book(title, filePath, cover) };
    booksToReturn[0].addTag(Tag(firstTagName));
    booksToReturn[0].addTag(Tag(secondTagName));
    
    dtos::TagDto firstTag { .name = firstTagName};
    dtos::TagDto secondTag { .name = secondTagName };
    dtos::BookDto expectedResult;
    expectedResult.title = title;
    expectedResult.filePath = filePath;
    expectedResult.cover = cover;
    expectedResult.tags.append(firstTag);
    expectedResult.tags.append(secondTag);
    
    
    // Expect
    EXPECT_CALL(bookService, getBooks())
            .Times(1)
            .WillOnce(ReturnRef(booksToReturn));
    
    // Act
    auto result = bookController->getBook(title);
    
    // Assert
    EXPECT_EQ(expectedResult.title, result.title);
    EXPECT_EQ(expectedResult.filePath, result.filePath);
    EXPECT_EQ(expectedResult.cover, result.cover);
    
    for(int i = 0; i < expectedResult.tags.size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags[i].name, result.tags[i].name);
    }
}



TEST_F(ABookController, SucceedsGettingTheBookCount)
{
    // Arrange
    std::vector<Book> booksToReturn;
    booksToReturn.emplace_back("SomeBook");
    booksToReturn.emplace_back("SomeOtherBook");
    
    auto expectedResult = 2;
    
    
    // Expect
    EXPECT_CALL(bookService, getBookCount())
            .Times(1)
            .WillOnce(Return(2));
    
    // Act
    auto result = bookController->getBookCount();
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookController, SucceedsGettingTheCurrentBook)
{
    // Arrange
    QString title = "SomeBook";
    QString filePath = "some/book.pdf";
    QString firstTagName = "FirstTag";
    QString secondTagName = "SecondTag";
    Book bookToReturn(title, filePath);
    bookToReturn.addTag(Tag(firstTagName));
    bookToReturn.addTag(Tag(secondTagName));
    
    dtos::BookDto expectedResult;
    expectedResult.title = title;
    expectedResult.filePath = filePath;
    dtos::TagDto firstTag;
    firstTag.name = firstTagName;
    expectedResult.tags.append(firstTag);
    dtos::TagDto secondTag;
    secondTag.name = secondTagName;
    expectedResult.tags.append(secondTag);
    
    
    // Expect
    EXPECT_CALL(bookService, getCurrentBook())
            .Times(1)
            .WillOnce(Return(&bookToReturn));
    
    // Act
    auto result = bookController->getCurrentBook();
    
    // Assert
    EXPECT_EQ(expectedResult.title, result.title);
    EXPECT_EQ(expectedResult.cover, result.cover);
    EXPECT_EQ(expectedResult.filePath, result.filePath);
    
    for(int i = 0;i < expectedResult.tags.size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags[i].name, result.tags[i].name);
    }
}

TEST_F(ABookController, SucceedsSettingTheCurrentBook)
{
    // Arrange
    BookOperationStatus expectedResult = BookOperationStatus::Success;
    
    // Expect
    EXPECT_CALL(bookService, setCurrentBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->setCurrentBook("SomeBook");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsSettingCurrentBookIfNoBookWithExists)
{
    // Arrange
    QString nonExistentTitle = "SomeNonExistenBook";
    
    BookOperationStatus expectedResult = BookOperationStatus::BookDoesNotExist;
    
    // Expect
    EXPECT_CALL(bookService, setCurrentBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::BookDoesNotExist));
    
    // Act
    auto result = bookController->setCurrentBook(nonExistentTitle);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsAddingATag)
{
    // Arrange
    dtos::TagDto tag{ .name = "SomeTag" };
    
    auto expectedResult = BookOperationStatus::Success;
    
    // Expect
    EXPECT_CALL(bookService, addTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->addTag("SomeTitle", tag);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsAddingTagIfTagAlreadyExists)
{
    // Arrange
    dtos::TagDto tag{ .name = "SomeTag" };
    
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    
    // Expect
    EXPECT_CALL(bookService, addTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::TagAlreadyExists));
    
    // Act
    auto result = bookController->addTag("SomeTitle", tag);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsRemovingATag)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    // Expect
    EXPECT_CALL(bookService, removeTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->removeTag("SomeTitle", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::TagDoesNotExist;
    
    // Expect
    EXPECT_CALL(bookService, removeTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::TagDoesNotExist));
    
    // Act
    auto result = bookController->removeTag("SomeTitle", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}