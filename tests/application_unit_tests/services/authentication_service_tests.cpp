#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QString>
#include "authentication_service.hpp"
#include "login_model.hpp"


using namespace testing;
using namespace application::services;
using namespace application;
using namespace domain;

namespace tests::application
{

class AuthenticationGatewayMock : public IAuthenticationGateway
{
public:
    MOCK_METHOD(void, authenticateUser,
                (const value_objects::LoginModel& loginModel), (override));
    MOCK_METHOD(void, registerUser,
                (const domain::value_objects::RegisterModel& registerModel),
                (override));
};

struct AnAuthenticationService : public ::testing::Test
{
    AnAuthenticationService()
    {
    }

    void SetUp() override
    {
        authService = std::make_unique<AuthenticationService>(&authGatewayMock);
    }

    AuthenticationGatewayMock authGatewayMock;
    std::unique_ptr<AuthenticationService> authService;
};

TEST_F(AnAuthenticationService, SucceedsRegisteringUser)
{
    // Arrange
    QString firstName = "John";
    QString lastName = "Doe";
    QString email = "someEmail@librum.com";
    QString password = "SomePassword123";
    bool keepUpdated = true;
    value_objects::RegisterModel registerModel(firstName, lastName, email,
                                               password, keepUpdated);


    // Expect
    EXPECT_CALL(authGatewayMock, registerUser(_)).Times(1);

    // Act
    authService->registerUser(registerModel);
}

TEST_F(AnAuthenticationService, SucceedsAuthenticatingUser)
{
    // Arrange
    QString email = "someEmail@librum.com";
    QString password = "SomePassword123";
    bool keepUpdated = true;
    value_objects::LoginModel loginModel(email, password, false);


    // Expect
    EXPECT_CALL(authGatewayMock, authenticateUser(_)).Times(1);

    // Act
    authService->loginUser(loginModel);
}

}  // namespace tests::application