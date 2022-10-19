#pragma once
#include <gtest/gtest.h>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "tag.hpp"


using namespace domain::models;


namespace tests::domain
{

TEST(ABook, SucceedsAddingATag)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    
    // Act
    auto result = book.addTag(tag);
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(1, book.getTags().size());
    EXPECT_EQ(tag.getName(), book.getTags()[0].getName());
}

TEST(ABook, FailsAddingATagIfItAlreadyExists)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    
    // Act
    book.addTag(tag);  // First time added
    auto result = book.addTag(tag);  // Second time added
    
    // Assert
    EXPECT_FALSE(result);
}



TEST(ABook, SucceedsRemovingATag)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    
    // Act
    book.addTag(tag);
    int prevAmountOfTags = book.getTags().size();
    
    auto result = book.removeTag(tag.getName());
    
    // Assert
    EXPECT_EQ(prevAmountOfTags - 1, book.getTags().size());
    EXPECT_TRUE(result);
}

TEST(ABook, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    
    // Act
    book.addTag(tag);
    
    auto result = book.removeTag(Tag("NonExistentTag"));
    
    // Assert
    EXPECT_FALSE(result);
}



TEST(ABook, SucceedsGettingAllTags)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    Tag thirdTag("ThirdTag");
    
    // Act
    book.addTag(firstTag);
    book.addTag(secondTag);
    book.addTag(thirdTag);
    
    // Assert
    EXPECT_EQ(firstTag, book.getTags()[0]);
    EXPECT_EQ(secondTag, book.getTags()[1]);
    EXPECT_EQ(thirdTag, book.getTags()[2]);
}



TEST(ABook, SucceedsUpdatingBook)
{
    // Arrange
    Book book("some/path", BookMetaData{ .title = "ATitle", .author = "AnAuthor" });
    
    Book bookToUpdateWith("some/path", BookMetaData
                          { 
                              .title = "AnotherTitle", 
                              .author = "AnotherAuthor" 
                          });
    Tag tag("SomeTag");
    bookToUpdateWith.addTag(tag);
    
    auto expectedResult = bookToUpdateWith;
    
    
    // Act
    book.update(bookToUpdateWith);
    
    // Assert
    EXPECT_EQ(expectedResult.getTitle(), book.getTitle());
    EXPECT_EQ(expectedResult.getFilePath(), book.getFilePath());
    EXPECT_EQ(expectedResult.getCover(), book.getCover());
    EXPECT_EQ(expectedResult.getTags()[0], book.getTags()[0]);
}



TEST(ABook, SucceedsSerializingToJson)
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
    Book book("some/path", metaData, currentPage, uuid);
    
    // Act
    auto serializedBook = book.serializeToJson();
    auto jsonDoc = QJsonDocument::fromJson(serializedBook);
    auto bookObject = jsonDoc.object();
    
    // Assert
    EXPECT_EQ(metaData.title, bookObject["title"].toString());
    EXPECT_EQ(metaData.author, bookObject["author"].toString());
    EXPECT_EQ(metaData.creator, bookObject["creator"].toString());
    EXPECT_EQ(metaData.releaseDate, bookObject["releaseDate"].toString());
    EXPECT_EQ(metaData.format, bookObject["format"].toString());
    EXPECT_EQ(metaData.language, bookObject["language"].toString());
    EXPECT_EQ(metaData.documentSize, bookObject["documentSize"].toString());
    EXPECT_EQ(metaData.pagesSize, bookObject["pagesSize"].toString());
    EXPECT_EQ(metaData.pageCount, bookObject["pageCount"].toInt());
    EXPECT_EQ(metaData.addedToLibrary, bookObject["addedToLibrary"].toString());
    EXPECT_EQ(metaData.lastOpened, bookObject["lastOpened"].toString());
    
    auto cover = bookObject["cover"].toString();
    auto coverImage = QImage::fromData(QByteArray::fromBase64(cover.toUtf8()));
    EXPECT_EQ(metaData.cover, coverImage);
    
    EXPECT_EQ(book.getFilePath(), bookObject["filePath"].toString());
    EXPECT_EQ(book.getCurrentPage(), bookObject["currentPage"].toInt());
    EXPECT_EQ(book.getUuid(), bookObject["uuid"].toString());
}

} // namespace tests::domain