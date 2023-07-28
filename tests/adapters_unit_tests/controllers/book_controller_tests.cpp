#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QByteArray>
#include <QString>
#include <QVariantMap>
#include <memory>
#include <utility>
#include "book.hpp"
#include "book_controller.hpp"
#include "book_dto.hpp"
#include "book_meta_data.hpp"
#include "book_operation_status.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"
#include "tag_dto.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace adapters;
using namespace domain::entities;
using namespace domain::value_objects;
using namespace application;

namespace tests::adapters
{

class BookServiceMock : public IBookService
{
public:
    MOCK_METHOD(void, downloadBooks, (), (override));
    MOCK_METHOD(BookOperationStatus, addBook, (const QString&), (override));
    MOCK_METHOD(BookOperationStatus, deleteBook, (const QUuid&), (override));
    MOCK_METHOD(BookOperationStatus, uninstallBook, (const QUuid&), (override));
    MOCK_METHOD(BookOperationStatus, downloadBookMedia, (const QUuid&),
                (override));
    MOCK_METHOD(BookOperationStatus, updateBook, (const Book& book),
                (override));
    MOCK_METHOD(BookOperationStatus, changeBookCover,
                (const QUuid&, const QString&), (override));

    MOCK_METHOD(const std::vector<Book>&, getBooks, (), (const, override));
    MOCK_METHOD(const Book*, getBook, (const QUuid&), (const, override));
    MOCK_METHOD(Book*, getBook, (const QUuid&), (override));
    MOCK_METHOD(int, getBookIndex, (const QUuid&), (const, override));
    MOCK_METHOD(int, getBookCount, (), (const, override));
    MOCK_METHOD(bool, refreshLastOpenedDateOfBook, (const QUuid&), (override));

    MOCK_METHOD(BookOperationStatus, addTagToBook,
                (const QUuid&, const domain::entities::Tag&), (override));
    MOCK_METHOD(BookOperationStatus, renameTagOfBook,
                (const QUuid&, const QUuid&, const QString&), (override));
    MOCK_METHOD(BookOperationStatus, removeTagFromBook,
                (const QUuid&, const QUuid&), (override));

    MOCK_METHOD(BookOperationStatus, saveBookToFile,
                (const QUuid&, const QString&), (override));

    MOCK_METHOD(void, setupUserData, (const QString&, const QString&),
                (override));
    MOCK_METHOD(void, clearUserData, (), (override));
    MOCK_METHOD(void, updateUsedBookStorage, (long, long), (override));
};

struct ABookController : public ::testing::Test
{
    ABookController()
    {
    }

    void SetUp() override
    {
        EXPECT_CALL(bookServiceMock, getBooks())
            .WillOnce(ReturnRef(bookVector));

        bookController =
            std::make_unique<controllers::BookController>(&bookServiceMock);
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

TEST_F(ABookController, SucceedsDeletingAllTagsWithAUuid)
{
    // Arrange
    Book firstBook("some/path", {});
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    firstBook.addTag(firstTag);
    firstBook.addTag(secondTag);

    Book secondBook("some/path", {});
    Tag thirdTag("ThirdTag");
    Tag fourthTag("FourthTag");
    secondBook.addTag(firstTag);
    secondBook.addTag(thirdTag);
    secondBook.addTag(fourthTag);

    std::vector<Book> books { firstBook, secondBook };

    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
        .Times(1)
        .WillOnce(ReturnRef(books));
    EXPECT_CALL(bookServiceMock, removeTagFromBook(_, _)).Times(2);

    // Act
    bookController->removeAllTagsWithUuid(firstTag.getUuid().toString());
}

TEST_F(ABookController, SucceedsNotDeletingTagsIfUuidDoesNotExist)
{
    // Arrange
    Book firstBook("some/path", {});
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    firstBook.addTag(firstTag);

    Book secondBook("some/path", {});
    Tag thirdTag("ThirdTag");
    Tag fourthTag("FourthTag");
    secondBook.addTag(firstTag);
    secondBook.addTag(thirdTag);

    std::vector<Book> books { firstBook, secondBook };

    QUuid nonExistentUuid = QUuid::createUuid();


    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
        .Times(1)
        .WillOnce(ReturnRef(books));

    // Act
    bookController->removeAllTagsWithUuid(nonExistentUuid.toString());
}

TEST_F(ABookController, FailsDeletingAllTagsWithAUuidIfUuidIsInvalid)
{
    // Arrange
    QString invalidUuid = "Invalid uuid";


    // Expect
    EXPECT_CALL(bookServiceMock, getBooks()).Times(0);

    // Act
    bookController->removeAllTagsWithUuid(invalidUuid);
}

TEST_F(ABookController, SucceedsUninstallingABook)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookServiceMock, uninstallBook(_))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));

    // Act
    auto result = bookController->uninstallBook("some-book-uuid");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUninstallingABookIfTheBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Expect
    EXPECT_CALL(bookServiceMock, uninstallBook(_))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::BookDoesNotExist));

    // Act
    auto result = bookController->uninstallBook("someBook");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, SucceedsRenamingTags)
{
    // Arrange
    Book firstBook("some/path", {});
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    firstBook.addTag(firstTag);
    firstBook.addTag(secondTag);

    Book secondBook("some/path", {});
    Tag thirdTag("ThirdTag");
    Tag fourthTag("FourthTag");
    secondBook.addTag(firstTag);
    secondBook.addTag(thirdTag);
    secondBook.addTag(fourthTag);

    std::vector<Book> books { firstBook, secondBook };

    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
        .Times(1)
        .WillOnce(ReturnRef(books));

    EXPECT_CALL(bookServiceMock, renameTagOfBook(_, _, _)).Times(2);

    // Act
    bookController->renameTags(firstTag.getName(), "NewName");
}

TEST_F(ABookController, FailsRenamingTagsIfNoTagsWithNameExist)
{
    // Arrange
    Book firstBook("some/path", {});
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    firstBook.addTag(firstTag);
    firstBook.addTag(secondTag);

    Book secondBook("some/path", {});
    Tag thirdTag("ThirdTag");
    Tag fourthTag("FourthTag");
    secondBook.addTag(firstTag);
    secondBook.addTag(thirdTag);
    secondBook.addTag(fourthTag);

    std::vector<Book> books { firstBook, secondBook };

    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
        .Times(1)
        .WillOnce(ReturnRef(books));

    EXPECT_CALL(bookServiceMock, renameTagOfBook(_, _, _)).Times(0);

    // Act
    bookController->renameTags("NonExistentTag", "NewName");
}

TEST_F(ABookController, SucceedsUpdatingABook)
{
    // Arrange
    BookMetaData bookMetaData { .title = "SomeTitle", .authors = "SomeAuthor" };
    Book bookToReturn("some/path.pdf", bookMetaData);

    auto titleNumber = static_cast<int>(IBookController::MetaProperty::Title);
    auto authorsNumber =
        static_cast<int>(IBookController::MetaProperty::Authors);

    QVariantMap operationsMap {
        { QString::number(titleNumber), "AnotherTitle" },
        { QString::number(authorsNumber), "DifferentAuthor" },
    };

    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookServiceMock, getBook(_))
        .Times(1)
        .WillOnce(Return(&bookToReturn));

    EXPECT_CALL(bookServiceMock, updateBook(_))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));

    // Act
    auto result = bookController->updateBook(
        "some-book-uuid", QVariant::fromValue(operationsMap));

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfTheBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;


    // Expect
    EXPECT_CALL(bookServiceMock, getBook(_)).Times(1).WillOnce(Return(nullptr));

    // Act
    auto result = bookController->updateBook("some-book-uuid", QVariant());

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsUpdatingABookIfGivenPropertyDoesNotExist)
{
    // Arrange
    BookMetaData bookMetaData { .title = "SomeTitle", .authors = "SomeAuthor" };
    Book bookToReturn("some/path.pdf", bookMetaData);

    int nonExistentProperty = 150;
    QVariantMap map { { QString::number(nonExistentProperty), "SomeValue" } };

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

TEST_F(ABookController, SucceedsDownloadingABook)
{
    // Arrange
    QString bookUuid = QUuid::createUuid().toString();

    auto expectedStatus = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookServiceMock, downloadBookMedia(_))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));


    // Act
    auto resultStatus = bookController->downloadBookMedia(bookUuid);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedStatus), resultStatus);
}

TEST_F(ABookController, SucceedsGettingABook)
{
    // Arrange
    QString title = "SomeBook";
    QString authors = "SomeAuthor";
    QString filePath = "some/path.pdf";
    QString tagNames[2] { "FirstTag", "SecondTag" };
    auto bookMetaData = BookMetaData { .title = title, .authors = authors };
    Book book(filePath, bookMetaData);
    const auto& bookUuid = book.getUuid();

    std::vector<Book> booksToReturn { book };
    booksToReturn[0].addTag(Tag(tagNames[0]));
    booksToReturn[0].addTag(Tag(tagNames[1]));


    dtos::TagDto firstTag { .name = tagNames[0] };
    dtos::TagDto secondTag { .name = tagNames[1] };
    dtos::BookDto expectedResult {
        .title = title,
        .authors = authors,
        .filePath = filePath,
        .tags = { firstTag, secondTag },
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
    EXPECT_EQ(expectedResult.authors, result.authors);

    for(int i = 0; i < expectedResult.tags.size(); ++i)
    {
        EXPECT_EQ(expectedResult.tags[i].name, result.tags[i].name);
    }
}

TEST_F(ABookController, FailsGettingABookIfNoneExists)
{
    // Arrange
    std::vector<Book> booksToReturn { Book("some/path", {}) };
    QUuid nonExistententUuid = QUuid::createUuid();

    dtos::BookDto expectedResult;

    // Expect
    EXPECT_CALL(bookServiceMock, getBooks())
        .Times(1)
        .WillOnce(ReturnRef(booksToReturn));

    // Act
    auto result = bookController->getBook(nonExistententUuid.toString());

    // Assert
    EXPECT_EQ("", result.title);  // Empty book
}

TEST_F(ABookController, SucceedsGettingTheBookCount)
{
    // Arrange
    std::vector<Book> booksToReturn;
    BookMetaData bookData { .title = "SomeTitle", .authors = "SomeAuthor" };
    booksToReturn.emplace_back("/some/path", bookData);
    booksToReturn.emplace_back("/some/path2", bookData);

    auto expectedResult = 2;


    // Expect
    EXPECT_CALL(bookServiceMock, getBookCount()).Times(1).WillOnce(Return(2));

    // Act
    auto result = bookController->getBookCount();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ABookController, SucceedsAddingATag)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;
    QString bookUuid = QUuid::createUuid().toString();
    QString tagUuid = QUuid::createUuid().toString();

    // Expect
    EXPECT_CALL(bookServiceMock, addTagToBook(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));

    // Act
    auto result = bookController->addTag(bookUuid, "SomeTag", tagUuid);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsAddingTagIfTagAlreadyExists)
{
    // Arrange
    auto expectedResult = BookOperationStatus::TagAlreadyExists;
    QString bookUuid = QUuid::createUuid().toString();
    QString tagUuid = QUuid::createUuid().toString();


    // Expect
    EXPECT_CALL(bookServiceMock, addTagToBook(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::TagAlreadyExists));

    // Act
    auto result = bookController->addTag(bookUuid, "SomeTag", tagUuid);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsAddingTagIfTheUuidIsInvalid)
{
    // Arrange
    auto expectedResult = BookOperationStatus::OperationFailed;
    QString bookUuid = QUuid::createUuid().toString();
    QString tagUuid = "Invalid uuid";


    // Expect
    EXPECT_CALL(bookServiceMock, addTagToBook(_, _)).Times(0);

    // Act
    auto result = bookController->addTag(bookUuid, "SomeTag", tagUuid);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, SucceedsRemovingATag)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;


    // Expect
    EXPECT_CALL(bookServiceMock, removeTagFromBook(_, _))
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
    EXPECT_CALL(bookServiceMock, removeTagFromBook(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::TagDoesNotExist));

    // Act
    auto result = bookController->removeTag("some-book-uuid", "SomeTag");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, SucceedsChangingBookCover)
{
    // Arrange
    auto expectedResult = BookOperationStatus::Success;

    // Expect
    EXPECT_CALL(bookServiceMock, changeBookCover(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));

    // Act
    auto result = bookController->changeBookCover("someUuid", "some/path.png");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsChangingBookCoverIfBookDoesNotExist)
{
    // Arrange
    auto expectedResult = BookOperationStatus::BookDoesNotExist;

    // Expect
    EXPECT_CALL(bookServiceMock, changeBookCover(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::BookDoesNotExist));

    // Act
    auto result = bookController->changeBookCover("wrongUuid", "some/path.png");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, FailsChangingBookCoverIfFilePathIsInvalid)
{
    // Arrange
    auto expectedResult = BookOperationStatus::OperationFailed;

    // Expect
    EXPECT_CALL(bookServiceMock, changeBookCover(_, _))
        .Times(1)
        .WillOnce(Return(BookOperationStatus::OperationFailed));

    // Act
    auto result = bookController->changeBookCover("someUuid", "wrong/path.png");

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

TEST_F(ABookController, SucceedsRefreshingLastOpenedFlag)
{
    // Arrange
    QString bookUuid = "some-book-uuid";

    // Expect
    EXPECT_CALL(bookServiceMock, refreshLastOpenedDateOfBook(_))
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
    EXPECT_CALL(bookServiceMock, saveBookToFile)
        .Times(1)
        .WillOnce(Return(BookOperationStatus::Success));

    // Act
    auto result = bookController->saveBookToFile(bookUuid, url);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult), result);
}

}  // namespace tests::adapters
