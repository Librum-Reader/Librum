#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QSignalSpy>
#include <QUuid>
#include "book_storage_gateway.hpp"
#include "i_book_storage_access.hpp"
#include "book.hpp"
#include "book_dto.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;
using namespace domain;



namespace tests::adapters
{

class BookStorageAccessMock : public IBookStorageAccess
{
public:
    MOCK_METHOD(void, createBook, (const QString&, const dtos::BookDto&), (override));
    MOCK_METHOD(void, deleteBook, (const QString&, const QUuid&), (override));
    MOCK_METHOD(void, updateBook, (const QString&, const dtos::BookDto&), (override));
    MOCK_METHOD(void, getBooksMetaData, (const QString&), (override));
    MOCK_METHOD(void, downloadBook, (const QString&, const QUuid&), (override));
};


struct ABookStorageGateway : public ::testing::Test
{
    void SetUp() override
    {
        bookStorageGateway = std::make_unique<BookStorageGateway>(&bookStorageAccessMock);
    }
    
    BookStorageAccessMock bookStorageAccessMock;
    std::unique_ptr<BookStorageGateway> bookStorageGateway;
};



TEST_F(ABookStorageGateway, SucceedsCreatingABook)
{
    // Arrange
    models::Book book("some/path.pdf", {}, 0);
    
    // Expect
    EXPECT_CALL(bookStorageAccessMock, createBook(_,_))
            .Times(1);
    
    // Act
    bookStorageGateway->createBook("some_token", book);
}

TEST_F(ABookStorageGateway, SucceedsDeletingABook)
{
    // Arrange
    QUuid uuid;
    
    
    // Expect
    EXPECT_CALL(bookStorageAccessMock, deleteBook(_,_))
            .Times(1);
    
    // Act
    bookStorageGateway->deleteBook("some_token", uuid);
}

TEST_F(ABookStorageGateway, SucceedsUpdatingABook)
{
    // Arrange
    models::Book book("some/path.pdf", {}, 0);
    
    // Expect
    EXPECT_CALL(bookStorageAccessMock, updateBook(_,_))
            .Times(1);
    
    // Act
    bookStorageGateway->updateBook("some_token", book);
}

TEST_F(ABookStorageGateway, SucceedsGettingBooksMetaData)
{
    // Expect
    EXPECT_CALL(bookStorageAccessMock, getBooksMetaData(_))
            .Times(1);
    
    // Act
    bookStorageGateway->getBooksMetaData("some_token");
}

};