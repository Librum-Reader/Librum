#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "i_user_storage_gateway.hpp"
#include "user_service.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace application::services;
using namespace application;
using namespace domain;

namespace tests::application
{

class UserStorageGatewayMock : public IUserStorageGateway
{
public:
    MOCK_METHOD(void, getUser, (const QString& authToken), (override));
    MOCK_METHOD(void, getProfilePicture, (const QString& authToken),
                (override));
    MOCK_METHOD(void, changeFirstName,
                (const QString& authToken, const QString& newFirstName),
                (override));
    MOCK_METHOD(void, changeLastName,
                (const QString& authToken, const QString& newFirstName),
                (override));
    MOCK_METHOD(void, changeEmail,
                (const QString& authToken, const QString& newFirstName),
                (override));
    MOCK_METHOD(void, changeProfilePicture,
                (const QString& authToken, const QString& path), (override));
    MOCK_METHOD(void, deleteProfilePicture, (const QString& authToken),
                (override));
    MOCK_METHOD(void, changeProfilePictureLastUpdated,
                (const QString& authToken, const QDateTime& newDateTime),
                (override));
    MOCK_METHOD(void, changeHasProfilePicture,
                (const QString& authToken, bool newValue), (override));
    MOCK_METHOD(void, deleteTag, (const QString&, const QUuid&), (override));
    MOCK_METHOD(void, renameTag,
                (const QString&, const QUuid&, const QString& newName),
                (override));
};

struct AUserService : public ::testing::Test
{
    void SetUp() override
    {
        userService = std::make_unique<UserService>(&userStorageGatewayMock);
    }

    UserStorageGatewayMock userStorageGatewayMock;
    std::unique_ptr<UserService> userService;
};

TEST_F(AUserService, SucceedsLoadingTheUser)
{
    // Act
    userService->loadUser(false);
}

TEST_F(AUserService, SucceedsSettingFirstName)
{
    // Arrange
    QString firstName = "SomeName";


    // Expect
    EXPECT_CALL(userStorageGatewayMock, changeFirstName(_, _)).Times(1);

    // Act
    userService->setFirstName(firstName);
}

TEST_F(AUserService, SucceedsGettingFirstName)
{
    // Arrange
    QString firstName = "SomeName";
    userService->setFirstName(firstName);

    QString expectedResult = firstName;


    // Act
    auto result = userService->getFirstName();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserService, SucceedsSettingLastName)
{
    // Arrange
    QString lastName = "SomeLastName";


    // Expect
    EXPECT_CALL(userStorageGatewayMock, changeLastName(_, _)).Times(1);

    // Act
    userService->setLastName(lastName);
}

TEST_F(AUserService, SucceedsGettingLastName)
{
    // Arrange
    QString lastName = "SomeLastName";
    userService->setLastName(lastName);

    QString expectedResult = lastName;


    // Act
    auto result = userService->getLastName();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserService, SucceedsSettingEmail)
{
    // Arrange
    QString email = "email@librum.com";


    // Expect
    EXPECT_CALL(userStorageGatewayMock, changeEmail(_, _)).Times(1);

    // Act
    userService->setEmail(email);
}

TEST_F(AUserService, SucceedsGettingEmail)
{
    // Arrange
    QString email = "email@librum.com";
    userService->setEmail(email);

    QString expectedResult = email;


    // Act
    auto result = userService->getEmail();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserService, SucceedsAddingATag)
{
    // Arrange
    entities::Tag tag("SomeTag");


    // Act
    auto result = userService->addTag(tag);
    auto tagCountAfterAdding = userService->getTags().size();

    // Assert
    EXPECT_EQ(tag.getUuid(), result);
    EXPECT_EQ(tagCountAfterAdding, 1);
}

TEST_F(AUserService, FailsAddingTagIfTagWithSameNameAlreadyExists)
{
    // Arrange
    entities::Tag tag("SomeTag");


    // Act
    userService->addTag(tag);  // First
    auto result = userService->addTag(tag);  // Second
    auto tagCountAfterAdding = userService->getTags().size();

    // Assert
    EXPECT_EQ(QUuid(), result);
    EXPECT_EQ(tagCountAfterAdding, 1);
}

TEST_F(AUserService, SucceedsGettingTags)
{
    // Arrange
    entities::Tag firstTag("SomeTag");
    entities::Tag secondTag("AnotherTag");
    userService->addTag(firstTag);
    userService->addTag(secondTag);


    // Act
    auto result = userService->getTags();

    // Assert
    EXPECT_EQ(firstTag, result[0]);
    EXPECT_EQ(secondTag, result[1]);
}

TEST_F(AUserService, FailsGettingTagsIfNoneExist)
{
    // Act
    auto result = userService->getTags();

    // Assert
    EXPECT_EQ(0, result.size());
}

TEST_F(AUserService, SucceedsDeletingATag)
{
    // Arrange
    entities::Tag tag("SomeTag");
    userService->addTag(tag);

    // Expect
    EXPECT_CALL(userStorageGatewayMock, deleteTag(_, _)).Times(1);

    // Act
    auto result = userService->deleteTag(tag.getUuid());
    auto tagCountAfterAdding = userService->getTags().size();

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tagCountAfterAdding, 0);
}

TEST_F(AUserService, FailsDeletingTagIfTagDoesNotExists)
{
    // Act
    auto result = userService->deleteTag(QUuid("NoneExistentTag"));

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(AUserService, SucceedsRenamingATag)
{
    // Arrange
    QString tagName = "SomeTag";
    QString newTagName = "SomeOtherTag";
    entities::Tag tag(tagName);
    userService->addTag(tag);

    // Expect
    EXPECT_CALL(userStorageGatewayMock, renameTag(_, _, _)).Times(1);

    // Act
    auto result = userService->renameTag(tag.getUuid(), newTagName);
    auto resultTags = userService->getTags();

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(newTagName, resultTags[0].getName());
}

TEST_F(AUserService, SucceedsRenamingATagIfNewNameIsUncapitalized)
{
    // Arrange
    QString tagName = "SomeTag";
    QString newTagName = "newName";  // Uncapitalized
    entities::Tag tag(tagName);
    userService->addTag(tag);

    QString expectedResult = newTagName;
    expectedResult[0] = expectedResult[0].toUpper();

    // Expect
    QString argPassedToMock;
    EXPECT_CALL(userStorageGatewayMock, renameTag(_, _, _))
        .Times(1)
        .WillOnce(Invoke(
            [&argPassedToMock](const QString&, const QUuid&,
                               const QString& tagName)
            {
                argPassedToMock = tagName;
            }));


    // Act
    auto result = userService->renameTag(tag.getUuid(), newTagName);
    auto resultTags = userService->getTags();

    // Assert
    EXPECT_TRUE(result);
    // Make sure mock gets new tag name capitalized
    EXPECT_EQ(expectedResult, argPassedToMock);
}

TEST_F(AUserService, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    QString newTagName = "SomeTag";


    // Act
    auto result =
        userService->renameTag(QUuid("NonExistentTagUuid"), newTagName);

    // Assert
    EXPECT_FALSE(result);
}

}  // namespace tests::application
