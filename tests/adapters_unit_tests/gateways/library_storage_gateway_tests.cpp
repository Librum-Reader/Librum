#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUuid>
#include "book.hpp"
#include "i_library_storage_access.hpp"
#include "library_storage_gateway.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;
using namespace domain;

namespace tests::adapters
{

class LibraryStorageAccessMock : public ILibraryStorageAccess
{
public:
    MOCK_METHOD(void, createBook, (const QString&, const QJsonObject&),
                (override));
    MOCK_METHOD(void, deleteBook, (const QString&, const QUuid&), (override));
    MOCK_METHOD(void, updateBook, (const QString&, const QJsonObject&),
                (override));
    MOCK_METHOD(void, uploadBookCover,
                (const QString&, const QUuid&, const QString&), (override));
    MOCK_METHOD(void, deleteBookCover, (const QString&, const QUuid&),
                (override));
    MOCK_METHOD(void, downloadCoverForBook, (const QString&, const QUuid&),
                (override));
    MOCK_METHOD(void, getBooksMetaData, (const QString&), (override));
    MOCK_METHOD(void, downloadBookMedia, (const QString&, const QUuid&),
                (override));
};

struct ALibraryStorageGateway : public ::testing::Test
{
    void SetUp() override
    {
        bookStorageGateway =
            std::make_unique<LibraryStorageGateway>(&bookStorageAccessMock);
    }

    LibraryStorageAccessMock bookStorageAccessMock;
    std::unique_ptr<LibraryStorageGateway> bookStorageGateway;
};

TEST_F(ALibraryStorageGateway, SucceedsCreatingABook)
{
    // Arrange
    entities::Book book("some/path.pdf", {}, 0);
    book.addTag(entities::Tag("FirstTag"));
    book.addTag(entities::Tag("SecondTag"));
    QJsonObject argPassedToMock;

    // Expect
    EXPECT_CALL(bookStorageAccessMock, createBook(_, _))
        .Times(1)
        .WillOnce(Invoke(
            [&argPassedToMock](const QString&, const QJsonObject& arg)
            {
                argPassedToMock = arg;
            }));


    // Act
    bookStorageGateway->createBook("some_token", book);

    // Assert
    // Check for correct conversion from uuid to guid json property
    QString argContent = QJsonDocument(argPassedToMock).toJson();
    EXPECT_FALSE(argContent.contains("uuid"));
}

TEST_F(ALibraryStorageGateway, SucceedsDeletingABook)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageGateway->deleteBook("some_token", uuid);
}

TEST_F(ALibraryStorageGateway, SucceedsUpdatingABook)
{
    // Arrange
    entities::Book book("some/path.pdf", {}, 0);
    book.addTag(entities::Tag("FirstTag"));
    book.addTag(entities::Tag("SecondTag"));
    QJsonObject argPassedToMock;

    // Expect
    EXPECT_CALL(bookStorageAccessMock, updateBook(_, _))
        .Times(1)
        .WillOnce(Invoke(
            [&argPassedToMock](const QString&, const QJsonObject& arg)
            {
                argPassedToMock = arg;
            }));


    // Act
    bookStorageGateway->updateBook("some_token", book);

    // Assert
    // Check for correct conversion from uuid to guid json property
    QString argContent = QJsonDocument(argPassedToMock).toJson();
    EXPECT_FALSE(argContent.contains("uuid"));
}

TEST_F(ALibraryStorageGateway, SucceedsGettingBooksMetaData)
{
    // Expect
    EXPECT_CALL(bookStorageAccessMock, getBooksMetaData(_)).Times(1);

    // Act
    bookStorageGateway->getBooksMetaData("some_token");
}

TEST_F(ALibraryStorageGateway, SucceedsChangingBookCover)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, uploadBookCover(_, _, _)).Times(1);

    // Act
    bookStorageGateway->changeBookCover("some_token", uuid,
                                        "path/to/cover.png");
}

TEST_F(ALibraryStorageGateway, SucceedsDeletingBookCover)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, deleteBookCover(_, _)).Times(1);

    // Act
    bookStorageGateway->deleteBookCover("some_token", uuid);
}

TEST_F(ALibraryStorageGateway, SucceedsGettingCoverForBook)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, downloadCoverForBook(_, _)).Times(1);

    // Act
    bookStorageGateway->getCoverForBook("some_token", uuid);
}

TEST_F(ALibraryStorageGateway, SucceedsDownloadingBook)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, downloadBookMedia(_, _)).Times(1);

    // Act
    bookStorageGateway->downloadBookMedia("some_token", uuid);
}

};  // namespace tests::adapters