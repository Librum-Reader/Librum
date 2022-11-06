#pragma once
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
                (const QString& authToken, const QImage& newPicture),
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

TEST_F(AUserService, SucceedsGettingTheUser)
{
    // Expect
    EXPECT_CALL(userStorageGatewayMock, getUser(_)).Times(1);

    // Act
    userService->loadUser();
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

TEST_F(AUserService, SucceedsSettingProfilePicture)
{
    // Arrange
    QImage profilePicture("/some/image.png");

    // Expect
    EXPECT_CALL(userStorageGatewayMock, changeProfilePicture(_, _)).Times(1);

    // Act
    userService->setProfilePicture(profilePicture);
}

TEST_F(AUserService, SucceedsGettingProfilePicture)
{
    // Arrange
    QImage profilePicture(50, 50, QImage::Format_ARGB32);
    userService->setProfilePicture(profilePicture);

    const auto& expectedResult = profilePicture;

    // Act
    auto result = userService->getProfilePicture();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserService, SucceedsAddingATag)
{
    // Arrange
    models::Tag tag("SomeTag");


    // Act
    auto result = userService->addTag(tag);
    auto tagCountAfterAdding = userService->getTags().size();

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(tagCountAfterAdding, 1);
}

TEST_F(AUserService, FailsAddingTagIfTagWithSameNameAlreadyExists)
{
    // Arrange
    models::Tag tag("SomeTag");


    // Act
    userService->addTag(tag);  // First
    auto result = userService->addTag(tag);  // Second
    auto tagCountAfterAdding = userService->getTags().size();

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(tagCountAfterAdding, 1);
}

}  // namespace tests::application