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
    user.addTag(tag);  // First time

    // Act
    auto result = user.addTag(tag);  // Second time

    // Assert
    EXPECT_FALSE(result);
}

TEST(AUser, FailsAddingATagIfTagIsInvalid)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    Tag tag("T");  // Name too short

    // Act
    auto result = user.addTag(tag);

    // Assert
    EXPECT_FALSE(result);
}

TEST(AUser, SucceedsDeletingATag)
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

TEST(AUser, FailsDeletingATagIfTagDoesNotExist)
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

TEST(AUser, FailsRenamingATagIfNewNameAlreadyExists)
{
    // Arrange
    User user("first", "last", "email@librum.com");
    Tag firstTag("SomeName");
    Tag secondTag("AnotherName");
    user.addTag(firstTag);
    user.addTag(secondTag);

    // Act
    auto result = user.renameTag(secondTag.getUuid(), firstTag.getName());

    // Assert
    EXPECT_FALSE(result);
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

TEST(AUser, FailsGettingATagByNameIfTagDoesNotExist)
{
    // Arrange
    User user("first", "last", "email@librum.com");

    // Act
    auto result = user.getTagByName("NonExistentTag");

    // Assert
    EXPECT_EQ(nullptr, result);
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

TEST(AUser, FailsGettingATagByUuidIfTagDoesNotExist)
{
    // Arrange
    User user("first", "last", "email@librum.com");

    // Act
    auto result = user.getTagByUuid(QUuid::createUuid());

    // Assert
    EXPECT_EQ(nullptr, result);
}

}  // namespace tests::domain