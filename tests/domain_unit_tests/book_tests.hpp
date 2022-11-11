#pragma once
#include <gtest/gtest.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
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
    Book book("some/path",
              BookMetaData { .title = "ATitle", .authors = "AnAuthor" });

    Book bookToUpdateWith(
        "some/path",
        BookMetaData { .title = "AnotherTitle", .authors = "AnotherAuthor" });
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
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .cover = QImage(""),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path", metaData, currentPage, uuid);


    // Act
    auto serializedBook = book.toJson();
    auto jsonDoc = QJsonDocument::fromJson(serializedBook);
    auto bookObject = jsonDoc.object();

    // Added to library
    auto addedToLibrary = QDateTime::fromString(
        bookObject["addedToLibrary"].toString(), "hh:mm:ss - dd.MM.yyyy");
    addedToLibrary.setTimeSpec(Qt::UTC);

    // Last opened
    auto lastOpened = QDateTime::fromString(bookObject["lastOpened"].toString(),
                                            "hh:mm:ss - dd.MM.yyyy");
    lastOpened.setTimeSpec(Qt::UTC);

    // Assert
    EXPECT_EQ(metaData.title, bookObject["title"].toString());
    EXPECT_EQ(metaData.authors, bookObject["authors"].toString());
    EXPECT_EQ(metaData.creator, bookObject["creator"].toString());
    EXPECT_EQ(metaData.creationDate, bookObject["creationDate"].toString());
    EXPECT_EQ(metaData.format, bookObject["format"].toString());
    EXPECT_EQ(metaData.language, bookObject["language"].toString());
    EXPECT_EQ(metaData.documentSize, bookObject["documentSize"].toString());
    EXPECT_EQ(metaData.pagesSize, bookObject["pagesSize"].toString());
    EXPECT_EQ(metaData.pageCount, bookObject["pageCount"].toInt());
    EXPECT_EQ(metaData.addedToLibrary.toSecsSinceEpoch(),
              addedToLibrary.toSecsSinceEpoch());

    auto cover = bookObject["cover"].toString();
    auto coverImage = QImage::fromData(QByteArray::fromBase64(cover.toUtf8()));
    EXPECT_EQ(metaData.cover, coverImage);

    EXPECT_EQ(book.getFilePath(), bookObject["filePath"].toString());
    EXPECT_EQ(book.getCurrentPage(), bookObject["currentPage"].toInt());
    EXPECT_EQ(book.getUuid(), bookObject["uuid"].toString());
}

TEST(ABook, SucceedsDeserializingFromJson)
{
    // Arrange
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime::currentDateTimeUtc(),
        .cover = QImage(""),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;
    Book book("some/path", metaData, currentPage, uuid);

    // Serialize to json
    auto serializedBook = book.toJson();
    auto jsonDoc = QJsonDocument::fromJson(serializedBook);
    auto serializedBookObject = jsonDoc.object();


    // Act
    auto result = Book::fromJson(serializedBookObject);


    // Assert
    EXPECT_EQ(metaData.title, result.getTitle());
    EXPECT_EQ(metaData.authors, result.getAuthors());
    EXPECT_EQ(metaData.creator, result.getCreator());
    EXPECT_EQ(metaData.creationDate, result.getCreationDate());
    EXPECT_EQ(metaData.format, result.getFormat());
    EXPECT_EQ(metaData.language, result.getLanguage());
    EXPECT_EQ(metaData.documentSize, result.getDocumentSize());
    EXPECT_EQ(metaData.pagesSize, result.getPagesSize());
    EXPECT_EQ(metaData.pageCount, result.getPageCount());
    EXPECT_EQ(metaData.addedToLibrary.toSecsSinceEpoch(),
              result.getAddedToLibrary().toSecsSinceEpoch());
    EXPECT_EQ(metaData.lastOpened.toSecsSinceEpoch(),
              result.getLastOpened().toSecsSinceEpoch());
    EXPECT_EQ(metaData.cover, result.getCover());
    EXPECT_EQ(book.getFilePath(), result.getFilePath());
    EXPECT_EQ(book.getCurrentPage(), result.getCurrentPage());
    EXPECT_EQ(book.getUuid(), result.getUuid());
}

TEST(ABook, SucceedsComparison)
{
    // Arrange
    // First book
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .cover = QImage(""),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;

    Book firstBook("some/path", metaData, currentPage, uuid);
    Book secondBook("some/path", metaData, currentPage, uuid);

    bool expectedResult = true;


    // Act
    auto result = firstBook == secondBook;

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(ABook, SucceedsFailsComparisonIfTheBooksDiffer)
{
    // Arrange
    // First book
    BookMetaData firstBookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .cover = QImage(""),
    };

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    int currentPage = 224;

    Book firstBook("some/path", firstBookMetaData, currentPage, uuid);

    BookMetaData secondBookMetaData {
        .title = "SomeOtherTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .cover = QImage(""),
    };

    Book secondBook("some/path", secondBookMetaData, currentPage, uuid);

    bool expectedResult = false;


    // Act
    auto result = firstBook == secondBook;

    // Assert
    EXPECT_EQ(expectedResult, result);
}

}  // namespace tests::domain