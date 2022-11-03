#pragma once
#include <gtest/gtest.h>
#include "user.hpp"


using namespace domain::models;

namespace tests::domain
{

TEST(AUser, SucceedsAddingATag)
{
    // Arrange
    User user;
    const Tag tag("SomeTag");
    const auto tagAmountBeforeAdding = user.getTags().size();

    // Act
    const auto result = user.addTag(tag);
    const auto tagResult = user.getTag(tag.getName());
    const auto tagAmountAfterAdding = user.getTags().size();

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tag, *tagResult);
    EXPECT_EQ(tagAmountBeforeAdding, 0);
    EXPECT_EQ(tagAmountAfterAdding, 1);
}

TEST(AUser, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    User user;
    const Tag tag("SomeTag");
    const auto tagAmountBeforeAdding = user.getTags().size();

    // Act
    user.addTag(tag);  // First time
    const auto result = user.addTag(tag);  // Second time
    const auto tagAmountAfterAdding = user.getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagAmountBeforeAdding, 0);
    EXPECT_EQ(tagAmountAfterAdding, 1);
}

TEST(AUser, SucceedsRemovingATag)
{
    // Arrange
    User user;
    const Tag tag("SomeTag");
    user.addTag(tag);
    const auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    const auto result = user.removeTag(tag.getName());
    const auto tagResult = user.getTag(tag.getName());
    const auto tagAmountAfterRemoving = user.getTags().size();

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tagResult, nullptr);
    EXPECT_EQ(tagAmountBeforeRemoving, 1);
    EXPECT_EQ(tagAmountAfterRemoving, 0);
}

TEST(AUser, FailsRemovingATagIfTagDoesNotExist)
{
    // Arrange
    User user;
    const auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    const auto result = user.removeTag("NonExistantTag");
    const auto tagAmountAfterRemoving = user.getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagAmountBeforeRemoving, tagAmountAfterRemoving);
}

TEST(AUser, SucceedsRenamingATag)
{
    // Arrange
    User user;
    const QString newTagName = "SomeNewTag";
    const QString tagName = "SomeTag";
    const Tag tag(tagName);
    user.addTag(tag);

    // Act
    const auto result = user.renameTag(tag.getName(), newTagName);
    const auto tagResult = user.getTag(newTagName);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_NE(tagResult, nullptr);
}

TEST(AUser, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    User user;
    const QString newTagName = "NewTagName";

    // Act
    const auto result = user.renameTag("NonExistantTag", newTagName);
    const auto tagResult = user.getTag(newTagName);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagResult, nullptr);
}

TEST(AUser, SucceedsGettingBooks)
{
    // Arrange
    User user;
    user.addTag(Tag("SomeTag1"));
    user.addTag(Tag("SomeTag2"));
    user.addTag(Tag("SomeTag3"));

    // Act
    const auto tagCount = user.getTags().size();

    // Assert
    EXPECT_EQ(tagCount, 3);
}

TEST(AUser, SucceedsGettingABook)
{
    // Arrange
    User user;
    const QString tagName = "SomeTag";
    user.addTag(Tag(tagName));

    // Act
    const auto result = user.getTag(tagName);

    // Assert
    EXPECT_EQ(result->getName(), tagName);
}

TEST(AUser, FailsGettingABookI)
{
    // Arrange
    User user;
    const QString tagName = "SomeTag";
    user.addTag(Tag(tagName));

    // Act
    const auto result = user.getTag(tagName);

    // Assert
    EXPECT_EQ(result->getName(), tagName);
}

}  // namespace tests::domain