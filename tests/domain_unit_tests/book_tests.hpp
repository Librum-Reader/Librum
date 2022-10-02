#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "book.hpp"
#include "tag.hpp"


using namespace domain::models;


TEST(ABook, SucceedsAddingATag)
{
    // Arrange
    Book book("SomeBook");
    Tag tag("SomeTag");
    
    // Act
    auto result = book.addTag(tag);
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(1, book.tags().size());
    EXPECT_EQ(tag.name(), book.tags()[0].name());
}

TEST(ABook, FailsAddingATagIfItAlreadyExists)
{
    // Arrange
    Book book("SomeBook");
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
    Book book("SomeBook");
    Tag tag("SomeTag");
    
    // Act
    book.addTag(tag);
    int prevAmountOfTags = book.tags().size();
    
    auto result = book.removeTag(tag.name());
    
    // Assert
    EXPECT_EQ(prevAmountOfTags - 1, book.tags().size());
    EXPECT_TRUE(result);
}

TEST(ABook, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    Book book("SomeBook");
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
    Book book("SomeBook");
    Tag firstTag("FirstTag");
    Tag secondTag("SecondTag");
    Tag thirdTag("ThirdTag");
    
    // Act
    book.addTag(firstTag);
    book.addTag(secondTag);
    book.addTag(thirdTag);
    
    // Assert
    EXPECT_EQ(firstTag, book.tags()[0]);
    EXPECT_EQ(secondTag, book.tags()[1]);
    EXPECT_EQ(thirdTag, book.tags()[2]);
}



TEST(ABook, SucceedsUpdatingBook)
{
    // Arrange
    Book book("SomeBook", "SomePath", QImage("SomeCover"));
    
    Tag tag("SomeTag");
    Book bookToUpdateWith("SomeUpdatedBook", "SomeUpdatedPath", QImage("SomeUpdatedCover"));
    bookToUpdateWith.addTag(tag);
    
    auto expectedResult = bookToUpdateWith;
    
    // Act
    book.update(bookToUpdateWith);
    
    // Assert
    EXPECT_EQ(expectedResult.title(), book.title());
    EXPECT_EQ(expectedResult.filePath(), book.filePath());
    EXPECT_EQ(expectedResult.cover(), book.cover());
    EXPECT_EQ(expectedResult.tags()[0], book.tags()[0]);
}