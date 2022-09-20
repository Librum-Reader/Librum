#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QSignalSpy>
#include "i_user_storage_gateway.hpp"
#include "authentication_service.hpp"
#include "login_model.hpp"


using namespace testing;
using namespace application::services;
using namespace domain;



class UserStorageGatewayMock : public application::IUserStorageGateway
{
public:
    MOCK_METHOD(void, authenticateUser, (const models::LoginModel& loginModel), (override));
    MOCK_METHOD(void, registerUser, (const domain::models::RegisterModel& registerModel), (override));
    MOCK_METHOD(void, reemitRegistrationResult, (bool success, const QString& reason), (override));
    MOCK_METHOD(void, reemitAuthenticationResult, (const QString& token), (override));
};



TEST(AnAuthenticationService, SucceedsLogingUserIn)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QString someValidEmail = "someEmail@librum.com";
    QString someValidPassword = "SomePassword123";
    models::LoginModel loginModel(someValidEmail, someValidPassword);
    
    
    // Expect
    EXPECT_CALL(userStorageGateway, authenticateUser(_))
            .Times(1);
    
    // Act
    authService.loginUser(loginModel);
}

TEST(AnAuthenticationService, FailsLogingUserInIfCredentialsInvalid)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(loginFinished(bool)));
    
    QString someInvalidEmail = "inval";
    QString someInvalidPassword = "Somee";
    models::LoginModel loginModel(someInvalidEmail, someInvalidPassword);
    
    
    // Expect
    EXPECT_CALL(userStorageGateway, authenticateUser(_))
            .Times(0);
    
    // Act
    authService.loginUser(loginModel);
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}



TEST(AnAuthenticationService, SucceedsRegisteringUser)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QString firstName = "John";
    QString lastName = "Doe";
    QString email = "someEmail@librum.com";
    QString password = "SomePassword123";
    bool keepUpdated = true;
    models::RegisterModel registerModel(firstName, lastName, email,
                                        password, keepUpdated);
    
    
    // Expect
    EXPECT_CALL(userStorageGateway, registerUser(_))
            .Times(1);
    
    // Act
    authService.registerUser(registerModel);
}

TEST(AnAuthenticationService, FailsRegisteringUserIfCredentialsAreInvalid)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(registrationFinished(bool, const QString&)));
    
    QString invalidFirstName = "J";
    QString lastName = "Doe";
    QString email = "someEmail@librum.com";
    QString password = "SomePassword123";
    bool keepUpdated = true;
    models::RegisterModel registerModel(invalidFirstName, lastName, email,
                                        password, keepUpdated);
    
    
    // Expect
    EXPECT_CALL(userStorageGateway, registerUser(_))
            .Times(0);
    
    // Act
    authService.registerUser(registerModel);
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}



TEST(AnAuthenticationService, SucceedsReemittingTheLoginSuccessSignal)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(loginFinished(bool)));
    
    // Act
    authService.processAuthenticationResult("validToken");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

TEST(AnAuthenticationService, SucceedsReemittingTheLoginFailureSignal)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(loginFinished(bool)));
    
    // Act
    authService.processAuthenticationResult("");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}



TEST(AnAuthenticationService, SucceedsReemittingTheRegistrationSuccessSignal)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(registrationFinished(bool, const QString&)));
    
    // Act
    authService.processRegistrationResult(true, "");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

TEST(AnAuthenticationService, SucceedsReemittingTheRegistrationFailureSignal)
{
    // Arrange
    UserStorageGatewayMock userStorageGateway;
    AuthenticationService authService(&userStorageGateway);
    
    QSignalSpy spy(&authService, SIGNAL(registrationFinished(bool, const QString&)));
    
    // Act
    authService.processRegistrationResult(false, "Some reason");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(false, arguments[0].toBool());
}