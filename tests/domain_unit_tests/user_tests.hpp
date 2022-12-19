#pragma once
#include <gtest/gtest.h>
#include "user.hpp"


using namespace domain::entities;

namespace tests::domain
{

TEST(AUser, SucceedsAddingATag)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    Tag tag("SomeTag");
    auto tagAmountBeforeAdding = user.getTags().size();

    // Act
    auto result = user.addTag(tag);
    auto tagResult = user.getTagByUuid(tag.getUuid());
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
    User user("first", "last", "email@librum.com");
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
    User user("first", "last", "email@librum.com");
    Tag tag("SomeTag");
    user.addTag(tag);
    auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    auto result = user.deleteTag(tag.getUuid());
    auto tagResult = user.getTagByUuid(tag.getUuid());
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
    User user("first", "last", "email@librum.com");
    auto tagAmountBeforeRemoving = user.getTags().size();

    // Act
    auto result = user.deleteTag("NonExistantTagUuid");
    auto tagAmountAfterRemoving = user.getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagAmountBeforeRemoving, tagAmountAfterRemoving);
}

TEST(AUser, SucceedsRenamingATag)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    QString tagName = "SomeTag";
    Tag tag(tagName);
    QString newTagName = "SomeNewTag";
    user.addTag(tag);

    // Act
    auto result = user.renameTag(tag.getUuid(), newTagName);
    auto tagResult = user.getTagByUuid(tag.getUuid());

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tagResult->getName(), newTagName);
}

TEST(AUser, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    QString newTagName = "NewTagName";

    // Act
    auto result = user.renameTag("NonExistantTagUuid", newTagName);
    auto tagResult = user.getTagByName(newTagName);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagResult, nullptr);
}

TEST(AUser, SucceedsGettingTags)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    user.addTag(Tag("SomeTag1"));
    user.addTag(Tag("SomeTag2"));
    user.addTag(Tag("SomeTag3"));

    // Act
    auto tagCount = user.getTags().size();

    // Assert
    EXPECT_EQ(tagCount, 3);
}

TEST(AUser, SucceedsGettingATagByName)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    Tag tag("SomeTag");
    user.addTag(tag);

    // Act
    auto result = user.getTagByName(tag.getName());

    // Assert
    EXPECT_EQ(*result, tag);
}

TEST(AUser, SucceedsGettingATagByUuid)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    Tag tag("SomeTag");
    user.addTag(tag);

    // Act
    auto result = user.getTagByUuid(tag.getUuid());

    // Assert
    EXPECT_EQ(*result, tag);
}

}  // namespace tests::domain