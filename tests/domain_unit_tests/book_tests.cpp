#include <gtest/gtest.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "tag.hpp"


using namespace domain::value_objects;
using namespace domain::entities;

namespace tests::domain
{

TEST(ABook, SucceedsGettingBoookProgressPercentageIfCurrentPageOne)
{
    BookMetaData bookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 10,
        .lastOpened = QDateTime::currentDateTimeUtc(),
    };

    Book book("some/path.pdf", bookMetaData);
    book.setPageCount(10);
    book.setCurrentPage(1);

    // Act
    int percentage = book.getBookReadingProgress();

    // Assert
    EXPECT_EQ(0, percentage);
}

TEST(ABook, SucceedsGettingBoookProgressPercentageIfCountOneAndCurrentPageOne)
{
    BookMetaData bookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 10,
        .lastOpened = QDateTime::currentDateTimeUtc(),
    };

    Book book("some/path.pdf", bookMetaData);
    book.setPageCount(1);
    book.setCurrentPage(1);

    // Act
    int percentage = book.getBookReadingProgress();

    // Assert
    EXPECT_EQ(100, percentage);
}

TEST(ABook, SucceedsGettingBoookProgressPercentageIfCurrentPageMiddle)
{
    BookMetaData bookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 10,
        .lastOpened = QDateTime::currentDateTimeUtc(),
    };

    Book book("some/path.pdf", bookMetaData);
    book.setPageCount(20);
    book.setCurrentPage(10);

    // Act
    int percentage = book.getBookReadingProgress();

    // Assert
    EXPECT_EQ(50, percentage);
}

TEST(ABook, SucceedsGettingBoookProgressPercentageIfCurrentPageEnd)
{
    BookMetaData bookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 10,
        .lastOpened = QDateTime::currentDateTimeUtc(),
    };

    Book book("some/path.pdf", bookMetaData);
    book.setPageCount(20);
    book.setCurrentPage(20);

    // Act
    int percentage = book.getBookReadingProgress();

    // Assert
    EXPECT_EQ(100, percentage);
}

TEST(ABook, FailsGettingBoookProgressPercentageIfLastOpenedInvalid)
{
    BookMetaData bookMetaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = "203 KiB",
        .pagesSize = "400 x 800",
        .pageCount = 10,
        .lastOpened = QDateTime(),  // Invalid
    };

    Book book("some/path.pdf", bookMetaData);
    book.setPageCount(10);
    book.setCurrentPage(5);

    // Act
    int percentage = book.getBookReadingProgress();

    // Assert
    EXPECT_EQ(0, percentage);
}

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
    EXPECT_EQ(tag, book.getTags()[0]);
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

    auto result = book.removeTag(tag.getUuid());

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

    auto result = book.removeTag("SomeUuid");

    // Assert
    EXPECT_FALSE(result);
}

TEST(ABook, SucceedsRenamingATag)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    book.addTag(tag);

    QString newName = "SomeNewName";

    // Act
    auto result = book.renameTag(tag.getUuid(), newName);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(newName, book.getTags()[0].getName());
}

TEST(ABook, FailsRenamingATagIfNoTagWithTheUuidExists)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag tag("SomeTag");
    book.addTag(tag);

    QUuid nonExistentUuid = QUuid::createUuid();
    QString newName = "SomeNewName";

    // Act
    auto result = book.renameTag(nonExistentUuid, newName);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_NE(newName, book.getTags()[0].getName());
}

TEST(ABook, FailsRenamingATagIfNewNameExists)
{
    // Arrange
    Book book("some/path", BookMetaData());
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    book.addTag(firstTag);
    book.addTag(secondTag);

    // Act
    auto result = book.renameTag(firstTag.getUuid(), secondTag.getName());

    // Assert
    EXPECT_FALSE(result);
    EXPECT_NE(secondTag.getName(), book.getTags()[0].getName());
}

TEST(ABook, SucceedsGettingAllTags)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData());
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    Tag thirdTag("ThirdTag");
    book.addTag(firstTag);
    book.addTag(secondTag);
    book.addTag(thirdTag);

    // Act
    auto result = book.getTags();

    // Assert
    EXPECT_EQ(firstTag, result[0]);
    EXPECT_EQ(secondTag, result[1]);
    EXPECT_EQ(thirdTag, result[2]);
}

TEST(ABook, SucceedsGettingTagByUuid)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData());
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    book.addTag(firstTag);
    book.addTag(secondTag);


    // Act
    auto* result = book.getTagByUuid(firstTag.getUuid());

    // Assert
    EXPECT_EQ(firstTag, *result);
}

TEST(ABook, FailsGettingTagByUuidIfTagDoesNotExist)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData());
    Tag firstTag("FirstTag");
    book.addTag(firstTag);


    // Act
    auto* result = book.getTagByUuid(QUuid::createUuid());

    // Assert
    EXPECT_EQ(nullptr, result);
}

TEST(ABook, SucceedsGettingTagByName)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData());
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    book.addTag(firstTag);
    book.addTag(secondTag);


    // Act
    auto* result = book.getTagByName(firstTag.getName());

    // Assert
    EXPECT_EQ(firstTag, *result);
}

TEST(ABook, FailsGettingTagByNameIfTagDoesNotExist)
{
    // Arrange
    Book book("some/path.pdf", BookMetaData());
    Tag firstTag("FirstTag");
    book.addTag(firstTag);


    // Act
    auto* result = book.getTagByName("NonExistentTag");

    // Assert
    EXPECT_EQ(nullptr, result);
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


    // Act
    book.update(bookToUpdateWith);

    // Assert
    EXPECT_EQ(bookToUpdateWith.getTitle(), book.getTitle());
    EXPECT_EQ(bookToUpdateWith.getFilePath(), book.getFilePath());
    EXPECT_EQ(bookToUpdateWith.getTags()[0], book.getTags()[0]);
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
        .hasCover = true,
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
    EXPECT_EQ(metaData.hasCover, bookObject["hasCover"].toBool());
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
        .hasCover = true,
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
    EXPECT_EQ(metaData.hasCover, result.hasCover());
    EXPECT_EQ(book.getFilePath(), result.getFilePath());
    EXPECT_EQ(book.getCurrentPage(), result.getCurrentPage());
    EXPECT_EQ(book.getUuid(), result.getUuid());
}

TEST(ABook, SucceedsEqualityComparison)
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
        .coverLastModified = QDateTime::currentDateTimeUtc(),
        .hasCover = true,
        .coverPath = "some/path.png",
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

TEST(ABook, FailsEqualityComparisonIfTheBooksDiffer)
{
    // Arrange
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
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
        .hasCover = true,
        .coverPath = "some/path.png"
    };
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
        .hasCover = true,
        .coverPath = "other/path.png"
    };
    Book secondBook("some/path", secondBookMetaData, currentPage, uuid);

    bool expectedResult = false;


    // Act
    auto result = firstBook == secondBook;

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(ABook, SucceedsGettingSizeInBytesForByteSizeFormat)
{
    // Arrange
    int sizeInBytes = 203;
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = QString::number(sizeInBytes) + " B",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .hasCover = false,
    };
    Book book("some/path.pdf", metaData);

    auto expectedSize = sizeInBytes;


    // Act
    auto bookSize = book.getSizeInBytes();

    // Assert
    EXPECT_EQ(bookSize, expectedSize);
}

TEST(ABook, SucceedsGettingSizeInBytesForKiBSizeFormat)
{
    // Arrange
    int sizeInKiB = 203;
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = QString::number(sizeInKiB) + " KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .hasCover = false,
    };
    Book book("some/path.pdf", metaData);

    auto expectedSize = sizeInKiB * 1024;


    // Act
    auto bookSize = book.getSizeInBytes();

    // Assert
    EXPECT_EQ(bookSize, expectedSize);
}

TEST(ABook, SucceedsGettingSizeInBytesForMiBSizeFormat)
{
    // Arrange
    int sizeInMiB = 203;
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = QString::number(sizeInMiB) + " MiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .hasCover = false,
    };
    Book book("some/path.pdf", metaData);

    auto expectedSize = sizeInMiB * 1024 * 1024;


    // Act
    auto bookSize = book.getSizeInBytes();

    // Assert
    EXPECT_EQ(bookSize, expectedSize);
}

TEST(ABook, SucceedsGettingSizeInBytesWhenCoverExists)
{
    // Arrange
    int sizeInKiB = 203;
    BookMetaData metaData {
        .title = "SomeTitle",
        .authors = "SomeAuthor",
        .creator = "SomeCreator",
        .creationDate = "Saturday, 11. September 2021 09:17:44 UTC",
        .format = "pdf",
        .language = "English",
        .documentSize = QString::number(sizeInKiB) + " KiB",
        .pagesSize = "400 x 800",
        .pageCount = 574,
        .lastOpened = QDateTime(),
        .hasCover = false,
    };
    Book book("some/path.pdf", metaData);

    // Create cover
    QFile cover("testCoverFile.txt");
    cover.open(QFile::WriteOnly);
    cover.write("Book cover data");

    book.setHasCover(true);
    book.setCoverPath(cover.fileName());


    auto expectedSize = sizeInKiB * 1024 + cover.size();


    // Act
    auto bookSize = book.getSizeInBytes();

    // Assert
    EXPECT_EQ(bookSize, expectedSize);
}

}  // namespace tests::domain