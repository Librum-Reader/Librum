#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QString>
#include <QUuid>
#include "book.hpp"
#include "book_storage_gateway.hpp"
#include "i_book_storage_access.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;
using namespace domain;

namespace tests::adapters
{

class BookStorageAccessMock : public IBookStorageAccess
{
public:
    MOCK_METHOD(void, createBook, (const QString&, const QJsonObject&),
                (override));
    MOCK_METHOD(void, deleteBook, (const QString&, const QUuid&), (override));
    MOCK_METHOD(void, updateBook, (const QString&, const QJsonObject&),
                (override));
    MOCK_METHOD(void, getBooksMetaData, (const QString&), (override));
    MOCK_METHOD(void, downloadBook, (const QString&, const QUuid&), (override));
};

struct ABookStorageGateway : public ::testing::Test
{
    void SetUp() override
    {
        bookStorageGateway =
            std::make_unique<BookStorageGateway>(&bookStorageAccessMock);
    }

    BookStorageAccessMock bookStorageAccessMock;
    std::unique_ptr<BookStorageGateway> bookStorageGateway;
};

TEST_F(ABookStorageGateway, SucceedsCreatingABook)
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

TEST_F(ABookStorageGateway, SucceedsDeletingABook)
{
    // Arrange
    QUuid uuid;


    // Expect
    EXPECT_CALL(bookStorageAccessMock, deleteBook(_, _)).Times(1);

    // Act
    bookStorageGateway->deleteBook("some_token", uuid);
}

TEST_F(ABookStorageGateway, SucceedsUpdatingABook)
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

TEST_F(ABookStorageGateway, SucceedsGettingBooksMetaData)
{
    // Expect
    EXPECT_CALL(bookStorageAccessMock, getBooksMetaData(_)).Times(1);

    // Act
    bookStorageGateway->getBooksMetaData("some_token");
}

};  // namespace tests::adapters