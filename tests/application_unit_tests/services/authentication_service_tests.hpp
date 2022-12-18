#pragma once
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

TEST_F(AnAuthenticationService, SucceedsLogingUserIn)
{
    // Arrange
    QString someValidEmail = "someEmail@librum.com";
    QString someValidPassword = "SomePassword123";
    value_objects::LoginModel loginModel(someValidEmail, someValidPassword);


    // Expect
    EXPECT_CALL(authGatewayMock, authenticateUser(_)).Times(1);

    // Act
    authService->loginUser(loginModel);
}

TEST_F(AnAuthenticationService, FailsLogingUserInIfCredentialsInvalid)
{
    // Arrange
    QSignalSpy spy(authService.get(), &AuthenticationService::loginFinished);

    QString someInvalidEmail = "inval";
    QString someInvalidPassword = "Somep";
    value_objects::LoginModel loginModel(someInvalidEmail, someInvalidPassword);


    // Expect
    EXPECT_CALL(authGatewayMock, authenticateUser(_)).Times(0);

    // Act
    authService->loginUser(loginModel);

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}

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

TEST_F(AnAuthenticationService, FailsRegisteringUserIfCredentialsAreInvalid)
{
    // Arrange
    QSignalSpy spy(authService.get(),
                   &AuthenticationService::registrationFinished);

    QString invalidFirstName = "J";
    QString lastName = "Doe";
    QString email = "someEmail@librum.com";
    QString password = "SomePassword123";
    bool keepUpdated = true;
    value_objects::RegisterModel registerModel(invalidFirstName, lastName,
                                               email, password, keepUpdated);


    // Expect
    EXPECT_CALL(authGatewayMock, registerUser(_)).Times(0);

    // Act
    authService->registerUser(registerModel);

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}

TEST_F(AnAuthenticationService, SucceedsReemittingTheLoginSuccessSignal)
{
    // Arrange
    QSignalSpy spy(authService.get(), &AuthenticationService::loginFinished);

    // Act
    authService->processAuthenticationResult("validToken");

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

TEST_F(AnAuthenticationService, SucceedsReemittingTheLoginFailureSignal)
{
    // Arrange
    QSignalSpy spy(authService.get(), &AuthenticationService::loginFinished);

    // Act
    authService->processAuthenticationResult("");

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}

TEST_F(AnAuthenticationService, SucceedsReemittingTheRegistrationSuccessSignal)
{
    // Arrange
    QSignalSpy spy(authService.get(),
                   &AuthenticationService::registrationFinished);


    // Act
    authService->processRegistrationResult(true, "");

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

TEST_F(AnAuthenticationService, SucceedsReemittingTheRegistrationFailureSignal)
{
    // Arrange
    QSignalSpy spy(authService.get(),
                   &AuthenticationService::registrationFinished);

    // Act
    authService->processRegistrationResult(false, "Some reason");

    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}

}  // namespace tests::application