#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QString>
#include "i_user_controller.hpp"
#include "i_user_service.hpp"
#include "tag.hpp"
#include "user_controller.hpp"

using ::testing::ReturnRef;
using namespace testing;
using namespace application;
using namespace adapters::controllers;
using namespace domain::entities;

namespace tests::adapters
{

class UserServiceMock : public application::IUserService
{
public:
    MOCK_METHOD(void, loadUser, (bool), (override));
    MOCK_METHOD(void, deleteUser, (), (override));
    MOCK_METHOD(void, downloadUser, (), (override));
    MOCK_METHOD(QString, getFirstName, (), (const, override));
    MOCK_METHOD(void, setFirstName, (const QString&), (override));
    MOCK_METHOD(QString, getLastName, (), (const, override));
    MOCK_METHOD(void, setLastName, (const QString&), (override));
    MOCK_METHOD(QString, getEmail, (), (const, override));
    MOCK_METHOD(void, setEmail, (const QString&), (override));
    MOCK_METHOD(void, changePassword, (const QString&), (override));
    MOCK_METHOD(long, getUsedBookStorage, (), (const, override));
    MOCK_METHOD(long, getBookStorageLimit, (), (const, override));
    MOCK_METHOD(QString, getProfilePicturePath, (), (const, override));
    MOCK_METHOD(void, setProfilePicturePath, (const QString&), (override));
    MOCK_METHOD(void, deleteProfilePicture, (), (override));
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
    EXPECT_CALL(userServiceMock, loadUser(false)).Times(1);

    // Act
    userController->loadUser(false);
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

TEST_F(AUserController, SucceedsAddingATag)
{
    // Arrange
    QString tagName = "SomeTag";
    QUuid tagUuid = QUuid::createUuid();

    auto expectedResult = tagUuid;


    // Expect
    EXPECT_CALL(userServiceMock, addTag(_)).Times(1).WillOnce(Return(tagUuid));

    // Act
    auto result = userController->addTag(tagName);

    // Assert
    EXPECT_EQ(expectedResult, QUuid(result));
}

TEST_F(AUserController, FailsAddingATagIfTagAlreadyExists)
{
    // Arrange
    QString tagName = "SomeTag";

    auto expectedResult = QUuid();


    // Expect
    EXPECT_CALL(userServiceMock, addTag(_)).Times(1).WillOnce(Return(QUuid()));

    // Act
    auto result = userController->addTag(tagName);

    // Assert
    EXPECT_EQ(expectedResult, QUuid(result));
}

TEST_F(AUserController, SucceedsDeletingATag)
{
    // Arrange
    Tag tag("SomeTag");


    // Expect
    EXPECT_CALL(userServiceMock, deleteTag(_)).Times(1).WillOnce(Return(true));

    // Act
    auto result = userController->deleteTag(tag.getUuid().toString());

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(AUserController, FailsDeletingATagIfTagDoesNotExist)
{
    // Arrange
    QUuid tagUuid = QUuid::createUuid();


    // Expect
    EXPECT_CALL(userServiceMock, deleteTag(_)).Times(1).WillOnce(Return(false));

    // Act
    auto result = userController->deleteTag(tagUuid.toString());

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(AUserController, SucceedsRenamingATag)
{
    // Arrange
    Tag tag("SomeTag");


    // Expect
    EXPECT_CALL(userServiceMock, renameTag(_, _))
        .Times(1)
        .WillOnce(Return(true));

    // Act
    auto result =
        userController->renameTag(tag.getUuid().toString(), "NewName");

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(AUserController, FailsRenamingATagIfTagDoesNotExist)
{
    // Arrange
    QUuid tagUuid = QUuid::createUuid();


    // Expect
    EXPECT_CALL(userServiceMock, renameTag(_, _))
        .Times(1)
        .WillOnce(Return(false));

    // Act
    auto result = userController->renameTag(tagUuid.toString(), "SomeName");

    // Assert
    EXPECT_FALSE(result);
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

TEST_F(AUserController, SucceedsGettingTagUuidForName)
{
    // Arrange
    Tag tagToFind("SomeName");
    std::vector<Tag> tags {
        Tag("SomeTag"),
        tagToFind,
    };

    QUuid expectedResult = tagToFind.getUuid();


    // Expect
    EXPECT_CALL(userServiceMock, getTags()).Times(1).WillOnce(ReturnRef(tags));

    // Act
    auto result = userController->getTagUuidForName(tagToFind.getName());

    // Assert
    EXPECT_EQ(expectedResult, QUuid(result));
}

TEST_F(AUserController, FailsGettingTagUuidForNameIfTagDoesNotExist)
{
    // Arrange
    std::vector<Tag> tags { Tag("SomeTag") };

    QUuid expectedResult = QUuid();


    // Expect
    EXPECT_CALL(userServiceMock, getTags()).Times(1).WillOnce(ReturnRef(tags));

    // Act
    auto result = userController->getTagUuidForName("NonExistentName");

    // Assert
    EXPECT_EQ(expectedResult, QUuid(result));
}

TEST_F(AUserController, FailsSettingAProfilePictureIfPathIsInvalid)
{
    // Arrange
    QString nonExistentPath = "some/path.png";

    // Expect
    EXPECT_CALL(userServiceMock, setProfilePicturePath(_)).Times(0);

    // Act
    userController->setProfilePicture(nonExistentPath);
}

}  // namespace tests::adapters
