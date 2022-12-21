#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QImage>
#include <QString>
#include "QSignalSpy"
#include "gmock/gmock.h"
#include "i_user_controller.hpp"
#include "i_user_service.hpp"
#include "qnamespace.h"
#include "tag.hpp"
#include "user_controller.hpp"

using ::testing::ReturnRef;
using namespace testing;
using namespace application;
using namespace adapters::controllers;

namespace tests::adapters
{

class UserServiceMock : public application::IUserService
{
public:
    MOCK_METHOD(void, loadUser, (), (override));
    MOCK_METHOD(QString, getFirstName, (), (const, override));
    MOCK_METHOD(void, setFirstName, (const QString&), (override));
    MOCK_METHOD(QString, getLastName, (), (const, override));
    MOCK_METHOD(void, setLastName, (const QString&), (override));
    MOCK_METHOD(QString, getEmail, (), (const, override));
    MOCK_METHOD(void, setEmail, (const QString&), (override));
    MOCK_METHOD(QImage, getProfilePicture, (), (const, override));
    MOCK_METHOD(void, setProfilePicture, (const QImage&), (override));
    MOCK_METHOD(void, setupUserData, (const QString&, const QString&),
                (override));
    MOCK_METHOD(void, clearUserData, (), (override));
    MOCK_METHOD(const std::vector<domain::entities::Tag>&, getTags, (),
                (const, override));
    MOCK_METHOD(QUuid, addTag, (const domain::entities::Tag&), (override));
    MOCK_METHOD(bool, deleteTag, (const QUuid&), (override));
    MOCK_METHOD(bool, renameTag, (const QUuid&, const QString&), (override));
};

struct AUserController : public ::testing::Test
{
    AUserController()
    {
    }

    void SetUp() override
    {
        // Set default return value for 'getTags'
        EXPECT_CALL(userServiceMock, getTags())
            .WillRepeatedly(ReturnRef(defaultReturn));

        userController = std::make_unique<UserController>(&userServiceMock);
    }

    UserServiceMock userServiceMock;
    std::vector<domain::entities::Tag> defaultReturn;
    std::unique_ptr<UserController> userController;
};

TEST_F(AUserController, SucceedsLoadingABook)
{
    // Expect
    EXPECT_CALL(userServiceMock, loadUser()).Times(1);

    // Act
    userController->loadUser();
}

TEST_F(AUserController, SucceedsGettingFirstName)
{
    // Arrange
    QString expectedResult = "SomeName";


    // Expect
    EXPECT_CALL(userServiceMock, getFirstName())
        .Times(1)
        .WillOnce(Return(expectedResult));

    // Act
    auto result = userController->getFirstName();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserController, SucceedsSettingFirstName)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::firstNameChanged);


    // Expect
    EXPECT_CALL(userServiceMock, getFirstName())
        .Times(1)
        .WillOnce(Return("SomeOldName"));

    EXPECT_CALL(userServiceMock, setFirstName(_)).Times(1);

    // Act
    userController->setFirstName("SomeName");

    // Assert
    EXPECT_EQ(1, spy.count());
}

TEST_F(AUserController, FailsSettingFirstNameIfItsTheSameAsCurrent)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::firstNameChanged);

    QString firstName = "SomeName";


    // Expect
    EXPECT_CALL(userServiceMock, getFirstName())
        .Times(1)
        .WillOnce(Return(firstName));

    EXPECT_CALL(userServiceMock, setFirstName(_)).Times(0);

    // Act
    userController->setFirstName(firstName);

    // Assert
    EXPECT_EQ(0, spy.count());
}

TEST_F(AUserController, SucceedsGettingLastName)
{
    // Arrange
    QString expectedResult = "SomeLastName";


    // Expect
    EXPECT_CALL(userServiceMock, getLastName())
        .Times(1)
        .WillOnce(Return(expectedResult));

    // Act
    auto result = userController->getLastName();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserController, SucceedsSettingLastName)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::lastNameChanged);


    // Expect
    EXPECT_CALL(userServiceMock, getLastName())
        .Times(1)
        .WillOnce(Return("SomeOldLastName"));

    EXPECT_CALL(userServiceMock, setLastName(_)).Times(1);

    // Act
    userController->setLastName("SomeLastName");

    // Assert
    EXPECT_EQ(1, spy.count());
}

TEST_F(AUserController, FailsSettingLastNameIfItsTheSameAsCurrent)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::lastNameChanged);

    QString lastName = "SomeName";


    // Expect
    EXPECT_CALL(userServiceMock, getLastName())
        .Times(1)
        .WillOnce(Return(lastName));

    EXPECT_CALL(userServiceMock, setLastName(_)).Times(0);

    // Act
    userController->setLastName(lastName);

    // Assert
    EXPECT_EQ(0, spy.count());
}

TEST_F(AUserController, SucceedsGettingEmail)
{
    // Arrange
    QString expectedResult = "SomeEmail";


    // Expect
    EXPECT_CALL(userServiceMock, getEmail())
        .Times(1)
        .WillOnce(Return(expectedResult));

    // Act
    auto result = userController->getEmail();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(AUserController, SucceedsSettingEmail)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::emailChanged);


    // Expect
    EXPECT_CALL(userServiceMock, getEmail())
        .Times(1)
        .WillOnce(Return("SomeOldEmail"));

    EXPECT_CALL(userServiceMock, setEmail(_)).Times(1);

    // Act
    userController->setEmail("SomeEmail");

    // Assert
    EXPECT_EQ(1, spy.count());
}

TEST_F(AUserController, FailsSettingEmailIfItsTheSameAsCurrent)
{
    // Arrange
    QSignalSpy spy(userController.get(), &UserController::emailChanged);

    QString email = "SomeEmail";


    // Expect
    EXPECT_CALL(userServiceMock, getEmail()).Times(1).WillOnce(Return(email));

    EXPECT_CALL(userServiceMock, setEmail(_)).Times(0);

    // Act
    userController->setEmail(email);

    // Assert
    EXPECT_EQ(0, spy.count());
}

TEST_F(AUserController, SucceedsGettingProfilePicture)
{
    // Arrange
    QImage image(50, 50, QImage::Format_ARGB32);

    QString expectedResultStart = "data:image/png;base64,";


    // Expect
    EXPECT_CALL(userServiceMock, getProfilePicture())
        .Times(1)
        .WillOnce(Return(image));

    // Act
    QString result = userController->getProfilePicture();

    // Assert
    EXPECT_TRUE(result.startsWith(expectedResultStart));
}

TEST_F(AUserController, FailsGettingProfilePictureIfNoProfilePictureExists)
{
    // Arrange
    QImage emptyImage("");

    QString expectedResult = "";


    // Expect
    EXPECT_CALL(userServiceMock, getProfilePicture())
        .Times(1)
        .WillOnce(Return(emptyImage));

    // Act
    QString result = userController->getProfilePicture();

    // Assert
    EXPECT_EQ(expectedResult, result);
}

// TODO: Profile picture tests

}  // namespace tests::adapters