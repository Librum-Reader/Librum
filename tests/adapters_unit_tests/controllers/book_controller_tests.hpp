#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <utility>
#include <QString>
#include <QVariantMap>
#include <QByteArray>
#include "book_controller.hpp"
#include "book_meta_data.hpp"
#include "i_book_controller.hpp"
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


namespace tests::adapters
{

class BookServiceMock : public IBookService
{
public:
    MOCK_METHOD(BookOperationStatus, addBook, (const QString&), (override));
    MOCK_METHOD(BookOperationStatus, deleteBook, (const QUuid&), (override));
    MOCK_METHOD(BookOperationStatus, uninstallBook, (const QUuid&), (override));
    MOCK_METHOD(BookOperationStatus, updateBook, (const QUuid&,
                                                  const Book&book), (override));
    
    MOCK_METHOD(const std::vector<Book>&, getBooks, (), (const, override));
    MOCK_METHOD(const Book*, getBook, (const QUuid&), (const, override));
    MOCK_METHOD(Book*, getBook, (const QUuid&), (override));
    MOCK_METHOD(int, getBookIndex, (const QUuid&), (const, override));
    MOCK_METHOD(int, getBookCount, (), (const, override));
    MOCK_METHOD(bool, refreshLastOpenedFlag, (const QUuid&), (override));
    
    MOCK_METHOD(BookOperationStatus, addTag, (const QUuid&,
                                              const domain::models::Tag&), (override));
    MOCK_METHOD(BookOperationStatus, removeTag, (const QUuid&,
                                                 const domain::models::Tag&), (override));
    
    MOCK_METHOD(BookOperationStatus, saveBookToPath, (const QUuid&, 
                                                      const QUrl&), (override));
    
    MOCK_METHOD(void, setAuthenticationToken, 
                (const QString&, const QString&), (override));
    MOCK_METHOD(void, clearAuthenticationToken, (), (override));
};



struct ABookController : public ::testing::Test
{
    ABookController() {}
    
    void SetUp() override
    {
        EXPECT_CALL(bookServiceMock, getBooks())
                .WillOnce(ReturnRef(bookVector));
        
        bookController = std::make_unique<controllers::BookController>(&bookServiceMock);
    }
    
    const std::vector<Book> bookVector;
    BookServiceMock bookServiceMock;
    std::unique_ptr<controllers::BookController> bookController;
};




TEST_F(ABookController, SucceedsAddingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, addBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
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
    EXPECT_CALL(bookServiceMock, deleteBook(_))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->deleteBook("some-book-uuid");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsDeletingABookIfTheBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, deleteBook(_))
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
    BookMetaData bookMetaData{ .title = "SomeTitle", .author = "SomeAuthor" };
    Book bookToReturn("some/path.pdf", bookMetaData);
    
    auto titleNumber = static_cast<int>(IBookController::MetaProperties::Title);
    auto authorNumber = static_cast<int>(IBookController::MetaProperties::Author);
    QVariantMap map
    {
        {QString::number(titleNumber), "AnotherTitle"},
        {QString::number(authorNumber), "DifferentAuthor"}
    };
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, getBook(_))
            .Times(1)
            .WillOnce(Return(&bookToReturn));
    
    EXPECT_CALL(bookServiceMock, updateBook(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->updateBook("some-book-uuid", QVariant::fromValue(map));
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfTheBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, getBook(_))
            .Times(1)
            .WillOnce(Return(nullptr));
    
    // Act
    auto result = bookController->updateBook("some-book-uuid", QVariant());
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfGivenPropertyDoesNotExist)
{
    // Arrange
    BookMetaData bookMetaData{ .title = "SomeTitle", .author = "SomeAuthor" };
    Book bookToReturn("some/path.pdf", bookMetaData);
    
    int nonExistentProperty = 150;
    QVariantMap map
    { 
        { QString::number(nonExistentProperty), "SomeValue" }
    };
    
    auto expectedResult = BookOperationStatus::PropertyDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, getBook(_))
            .Times(1)
            .WillOnce(Return(&bookToReturn));
    
    // Act
    auto result = bookController->updateBook("some-book-uuid", map);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsGettingABook)
{
    // Arrange
    QString title = "SomeBook";
    QString author = "SomeAuthor";
    QString filePath = "some/path.pdf";
    QString tagNames[2] { "FirstTag", "SecondTag" };
    Book book(filePath, BookMetaData{ .title = title, .author = author });
    const auto& bookUuid = book.getUuid();
    
    std::vector<Book> booksToReturn{ book };
    booksToReturn[0].addTag(tagNames[0]);
    booksToReturn[0].addTag(tagNames[1]);
    
    
    dtos::TagDto firstTag { .name = tagNames[0] };
    dtos::TagDto secondTag { .name = tagNames[1] };
    dtos::BookDto expectedResult
    {
        .title = title,
        .author = author,
        .filePath = filePath,
        .tags = { firstTag, secondTag }
    };
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
            .Times(1)
            .WillOnce(ReturnRef(booksToReturn));
    
    // Act
    auto uuidAsString = bookUuid.toString(QUuid::WithoutBraces);
    auto result = bookController->getBook(uuidAsString);
    
    // Assert
    EXPECT_EQ(expectedResult.title, result.title);
    EXPECT_EQ(expectedResult.filePath, result.filePath);
    EXPECT_EQ(expectedResult.author, result.author);
    
    for(int i = 0; i < expectedResult.tags.size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags[i].name, result.tags[i].name);
    }
}


TEST_F(ABookController, SucceedsGettingTheBookCount)
{
    // Arrange
    std::vector<Book> booksToReturn;
    BookMetaData bookData{ .title = "SomeTitle", .author = "SomeAuthor" };
    booksToReturn.emplace_back("/some/path", bookData);
    booksToReturn.emplace_back("/some/path2", bookData);
    
    auto expectedResult = 2;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, getBookCount())
            .Times(1)
            .WillOnce(Return(2));
    
    // Act
    auto result = bookController->getBookCount();
    
    // Assert
    EXPECT_EQ(expectedResult, result);
}



TEST_F(ABookController, SucceedsAddingATag)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, addTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->addTag("some-book-uuid", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsAddingTagIfTagAlreadyExists)
{
    // Arrange
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, addTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::TagAlreadyExists));
    
    // Act
    auto result = bookController->addTag("some-book-uuid", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsRemovingATag)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, removeTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->removeTag("some-book-uuid", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::TagDoesNotExist;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, removeTag(_, _))
            .Times(1)
            .WillOnce(Return(BookOperationStatus::TagDoesNotExist));
    
    // Act
    auto result = bookController->removeTag("some-book-uuid", "SomeTag");
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}



TEST_F(ABookController, SucceedsRefreshingLastOpenedFlag)
{
    // Arrange
    QString bookUuid = "some-book-uuid";
    
    // Expect
    EXPECT_CALL(bookServiceMock, refreshLastOpenedFlag(_))
            .Times(1)
            .WillOnce(Return(true));
    
    // Act
    bookController->refreshLastOpenedFlag(bookUuid);
}



TEST_F(ABookController, SucceedsSavingABookToAPath)
{
    // Arrange
    QString bookUuid = "some-book-uuid";
    QString url = "/some/url/";
    
    auto expectedResult = BookOperationStatus::Success;
    
    
    // Expect
    EXPECT_CALL(bookServiceMock, saveBookToPath)
            .Times(1)
            .WillOnce(Return(BookOperationStatus::Success));
    
    // Act
    auto result = bookController->saveBookToPath(bookUuid, url);
    
    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

} // namespace tests::adapters