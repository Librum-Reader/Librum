#include <gtest/gtest.h>
#include "tag.hpp"


using namespace domain::entities;

namespace tests::domain
{

TEST(ATag, SucceedsNameCapitilizationOnConstruction)
{
    // Arrange
    QString tagName = "smallName";

    QString expectedResult = tagName;
    expectedResult[0] = expectedResult[0].toUpper();


    // Act
    Tag tag(tagName);

    // Assert
    EXPECT_NE(tag.getName(), tagName);
    EXPECT_EQ(tag.getName(), expectedResult);
}

TEST(ATag, SucceedsEqualityComparisonToAnotherTag)
{
    Tag firstTag("SomeTag");
    Tag secondTag("SomeTag", firstTag.getUuid().toString(QUuid::WithoutBraces));

    EXPECT_EQ(firstTag, secondTag);
}

TEST(ATag, FailsEqualityComparisonToAnotherTagIfUuidDiffers)
{
    Tag firstTag("SomeTag");
    Tag secondTag("SomeTag");

    EXPECT_NE(firstTag, secondTag);
}

TEST(ATag, FailsEqualityComparisonToAnotherTagIfNameDiffers)
{
    Tag firstTag("SomeTag");
    Tag secondTag("DifferentTag",
                  firstTag.getUuid().toString(QUuid::WithoutBraces));

    EXPECT_NE(firstTag, secondTag);
}

TEST(ATag, SucceedsRenaming)
{
    // Arrange
    QString newName = "NewTagName";
    Tag firstTag("SomeTag");


    // Act
    firstTag.setName(newName);

    // Assert

    EXPECT_EQ(firstTag.getName(), newName);
}

TEST(ATag, SucceedsNameCapitilizationOnRenaming)
{
    // Arrange
    QString newName = "newTagName";
    Tag firstTag("SomeTag");

    QString expectedResult = newName;
    expectedResult[0] = expectedResult[0].toUpper();

    // Act
    firstTag.setName(newName);

    // Assert

    EXPECT_NE(firstTag.getName(), newName);
    EXPECT_EQ(firstTag.getName(), expectedResult);
}

TEST(ATag, SucceedsValidityCheck)
{
    QString validName = "Programming";
    Tag firstTag(validName);

    EXPECT_TRUE(firstTag.isValid());
}

TEST(ATag, FailsValidityCheckIfNameIsTooShort)
{
    QString shortName = "X";
    Tag firstTag(shortName);

    EXPECT_FALSE(firstTag.isValid());
}

TEST(ATag, SucceedsSerializationToJson)
{
    Tag tag("SomeTag");


    // Act
    auto serializedTag = tag.toJson();
    auto jsonDoc = QJsonDocument::fromJson(serializedTag);
    auto tagObject = jsonDoc.object();

    // Assert
    EXPECT_EQ(tag.getName(), tagObject["name"].toString());
    EXPECT_EQ(tag.getUuid(), tagObject["uuid"].toString());
}

}  // namespace tests::domain