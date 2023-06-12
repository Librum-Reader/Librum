#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QImage>
#include "i_user_storage_access.hpp"
#include "user_storage_gateway.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;

namespace tests::adapters
{

class UserStorageAccessMock : public IUserStorageAccess
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
    MOCK_METHOD(void, deleteTag, (const QString&, const QString&), (override));
    MOCK_METHOD(void, renameTag, (const QString&, const QJsonObject&),
                (override));
};

struct AUserStorageGateway : public testing::Test
{
    void SetUp() override
    {
        userStorageGateway =
            std::make_unique<UserStorageGateway>(&userStorageAccessMock);
    }

    UserStorageAccessMock userStorageAccessMock;
    std::unique_ptr<UserStorageGateway> userStorageGateway;
};

TEST_F(AUserStorageGateway, SucceedsGettingAUser)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, getUser(_)).Times(1);

    // Act
    userStorageGateway->getUser("secureToken");
}

TEST_F(AUserStorageGateway, SucceedsChangingTheFirstName)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeFirstName(_, _)).Times(1);

    // Act
    userStorageGateway->changeFirstName("secureToken", "SomeFirstName");
}

TEST_F(AUserStorageGateway, SucceedsChangingTheLastName)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeLastName(_, _)).Times(1);

    // Act
    userStorageGateway->changeLastName("secureToken", "SomeLastName");
}

TEST_F(AUserStorageGateway, SucceedsChangingTheEmail)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeEmail(_, _)).Times(1);

    // Act
    userStorageGateway->changeEmail("secureToken", "SomeEmail");
}

TEST_F(AUserStorageGateway, SucceedsChangingProfilePicture)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeProfilePicture(_, _)).Times(1);

    // Act
    userStorageGateway->changeProfilePicture("secureToken", "/some/image.png");
}

TEST_F(AUserStorageGateway, SucceedsDeletingATag)
{
    // Arrange
    QString authToken = "secureToken";
    QUuid bookUuid = QUuid::createUuid();

    // Expect
    EXPECT_CALL(userStorageAccessMock, deleteTag(_, _)).Times(1);

    // Act
    userStorageGateway->deleteTag(authToken, bookUuid);
}

TEST_F(AUserStorageGateway, SucceedsRenamingATag)
{
    // Arrange
    QString authToken = "secureToken";
    QUuid bookUuid = QUuid::createUuid();
    QString newName = "SomeName";

    // Expect
    EXPECT_CALL(userStorageAccessMock, renameTag(_, _)).Times(1);

    // Act
    userStorageGateway->renameTag(authToken, bookUuid, newName);
}

}  // namespace tests::adapters
