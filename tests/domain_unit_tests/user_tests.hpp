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
    Tag tag("SomeTag");
    auto tagAmountBeforeAdding = user.getTags().size();

    // Act
    auto result = user.addTag(tag);
    auto tagResult = user.getTag(tag.getName());
    auto tagAmountAfterAdding = user.getTags().size();

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
    Tag tag("SomeTag");
    auto tagAmountBeforeAdding = user.getTags().size();

    // Act
    user.addTag(tag);  // First time
    auto result = user.addTag(tag);  // Second time
    auto tagAmountAfterAdding = user.getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagAmountBeforeAdding, 0);
    EXPECT_EQ(tagAmountAfterAdding, 1);
}

TEST(AUser, SucceedsRemovingATag)
{
    // Arrange
    User user;
    Tag tag("SomeTag");
    user.addTag(tag);
    auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    auto result = user.removeTag(tag.getName());
    auto tagResult = user.getTag(tag.getName());
    auto tagAmountAfterRemoving = user.getTags().size();

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
    auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    auto result = user.removeTag("NonExistantTag");
    auto tagAmountAfterRemoving = user.getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagAmountBeforeRemoving, tagAmountAfterRemoving);
}

TEST(AUser, SucceedsRenamingATag)
{
    // Arrange
    User user;
    QString newTagName = "SomeNewTag";
    QString tagName = "SomeTag";
    Tag tag(tagName);
    user.addTag(tag);

    // Act
    auto result = user.renameTag(tag.getName(), newTagName);
    auto tagResult = user.getTag(newTagName);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tagResult->getName(), newTagName);
}

TEST(AUser, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    User user;
    QString newTagName = "NewTagName";

    // Act
    auto result = user.renameTag("NonExistantTag", newTagName);
    auto tagResult = user.getTag(newTagName);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagResult, nullptr);
}

TEST(AUser, SucceedsGettingTags)
{
    // Arrange
    User user;
    user.addTag(Tag("SomeTag1"));
    user.addTag(Tag("SomeTag2"));
    user.addTag(Tag("SomeTag3"));

    // Act
    auto tagCount = user.getTags().size();

    // Assert
    EXPECT_EQ(tagCount, 3);
}

TEST(AUser, SucceedsGettingATag)
{
    // Arrange
    User user;
    QString tagName = "SomeTag";
    user.addTag(Tag(tagName));

    // Act
    auto result = user.getTag(tagName);

    // Assert
    EXPECT_EQ(result->getName(), tagName);
}

TEST(AUser, FailsGettingATagIfTheTagDoesNotExist)
{
    // Arrange
    User user;

    // Act
    auto result = user.getTag("NonExistentTag");

    // Assert
    EXPECT_EQ(result, nullptr);
}

}  // namespace tests::domain