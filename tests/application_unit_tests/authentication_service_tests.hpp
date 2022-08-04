#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "authentication_service.hpp"
#include "i_user_storage_gateway.hpp"
#include "login_model.hpp"
#include "register_model.hpp"

using namespace domain::models;
using namespace testing;


class UserStorageGatewayMock : public application::IUserStorageGateway
{
public:
    MOCK_METHOD(void, authenticateUser, (LoginModel loginModel), (override));
    MOCK_METHOD(void, createUser, (RegisterModel registerModel), (override));
};



TEST(AnAuthenticationService, AuthenticatesTheUserWhenTheDataIsValid)
{
    // Arrange
    UserStorageGatewayMock userStorageMock;
    LoginModel loginModel("johndoe@librum.com", "SomePassword123");
    
    // Assert
    EXPECT_CALL(userStorageMock, authenticateUser(_))
            .Times(Exactly(1));
    
    // Act
    application::services::AuthenticationService authService(&userStorageMock);
    authService.authenticateUser(loginModel);
}

TEST(AnAuthenticationService, DoesNotAuthenticateTheUserWhenTheDataIsInvalid)
{
    // Arrange
    UserStorageGatewayMock userStorageMock;
    QString tooShortPassword = "SomeP";
    LoginModel loginModel("johndoe@librum.com", tooShortPassword);
    
    // Assert
    EXPECT_CALL(userStorageMock, authenticateUser(_))
            .Times(Exactly(0));
    
    // Act
    application::services::AuthenticationService authService(&userStorageMock);
    authService.authenticateUser(loginModel);
}

TEST(AnAuthenticationService, RegistersTheUserWhenTheDataIsValid)
{
    // Arrange
    UserStorageGatewayMock userStorageMock;
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    EXPECT_CALL(userStorageMock, createUser(_))
            .Times(Exactly(1));
    
    // Act
    application::services::AuthenticationService authService(&userStorageMock);
    authService.registerUser(registerModel);
}

TEST(AnAuthenticationService, DoesNotRegisterTheUserWhenTheDataIsInvalid)
{
    // Arrange
    UserStorageGatewayMock userStorageMock;
    QString tooShortPassword = "SomeP";
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", tooShortPassword, true);
    
    // Assert
    EXPECT_CALL(userStorageMock, createUser(_))
            .Times(Exactly(0));
    
    // Act
    application::services::AuthenticationService authService(&userStorageMock);
    authService.registerUser(registerModel);
}