#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "authentication_controller.hpp"
#include "login_model.hpp"

using namespace testing;
using namespace adapters;
using namespace domain::models;


class AuthenticationServiceMock : public application::IAuthenticationService
{
public:
    MOCK_METHOD(void, authenticateUser, (LoginModel loginModel), (override));
    MOCK_METHOD(void, registerUser, (RegisterModel registerModel), (override));
};



TEST(AnAuthenticationController, LogsInTheUser)
{
    // Arrange
    AuthenticationServiceMock authServiceMock;
    
    // Assert
    EXPECT_CALL(authServiceMock, authenticateUser(_))
            .Times(Exactly(1));
    
    // Act
    controllers::AuthenticationController authController(&authServiceMock);
    authController.loginUser("johndoe@librum.com", "SomePassword123");
}

TEST(AnAuthenticationController, RegistersTheUser)
{
    // Arrange
    AuthenticationServiceMock authServiceMock;
    
    // Assert
    EXPECT_CALL(authServiceMock, registerUser(_))
            .Times(Exactly(1));
    
    // Act
    controllers::AuthenticationController authController(&authServiceMock);
    authController.registerUser("John", "Doe", "johndoe@librum.com", "SomePassword123", false);
}