#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <login_dto.hpp>
#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace adapters::controllers;
using namespace adapters;
using namespace domain::models;
using namespace application;



class AuthenticationServiceMock : public application::IAuthenticationService
{
public:
    MOCK_METHOD(void, loginUser, (const LoginModel& loginModel), (override));
    MOCK_METHOD(void, registerUser, (const domain::models::RegisterModel& registerModel), (override));
    MOCK_METHOD(void, processAuthenticationResult, (const QString& token), (override));
    MOCK_METHOD(void, processRegistrationResult, (bool success, const QString& reason), (override));
    MOCK_METHOD(void, setAuthenticationToken, (const QString& token), (override));
};



TEST(AnAuthenticationController, SucceedsLogingAUserIn)
{
    // Arrange
    AuthenticationServiceMock authServiceMock;
    AuthenticationController authController(&authServiceMock);
    
    QString email = "SomeEmail@librum.com";
    QString password = "SomePassword12345";

    
    // Expect
    EXPECT_CALL(authServiceMock, loginUser(_))
            .Times(1);
    
    
    // Act
    authController.loginUser(email, password);
}

TEST(AnAuthenticationController, SucceedsRegisteringAUser)
{
    // Arrange
    AuthenticationServiceMock authServiceMock;
    AuthenticationController authController(&authServiceMock);
    
    QString firstName = "Kai";
    QString lastName = "Doe";
    QString email = "SomeEmail@librum.com";
    QString password = "SomePassword12345";
    bool keepUpdated = true;

    
    // Expect
    EXPECT_CALL(authServiceMock, registerUser(_))
            .Times(1);
    
    
    // Act
    authController.registerUser(firstName, lastName, email, password, keepUpdated);
}