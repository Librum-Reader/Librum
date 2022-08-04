#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "user_storage_gateway.hpp"
#include "login_model.hpp"

using namespace testing;
using namespace adapters;
using namespace domain::models;


class UserStorageAccessMock : public IUserStorageAccess
{
public:
    MOCK_METHOD(void, authenticateUser, (adapters::dtos::LoginDto loginDto), (override));
    MOCK_METHOD(void, createUser, (adapters::dtos::RegisterDto registerDto), (override));
};



TEST(AnUserStorageGateway, AuthenticatesTheUser)
{
    // Arrange
    UserStorageAccessMock userStorageAccessMock;
    LoginModel loginModel("johndoe@librum.com", "SomePassword123");
    
    // Assert
    EXPECT_CALL(userStorageAccessMock, authenticateUser(_))
            .Times(1);
    
    // Act
    gateways::UserStorageGateway userStorageGateway(&userStorageAccessMock);
    userStorageGateway.authenticateUser(loginModel);
}

TEST(AnUserStorageGateway, CreatesTheUser)
{
    // Arrange
    UserStorageAccessMock userStorageAccessMock;
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    EXPECT_CALL(userStorageAccessMock, createUser(_))
            .Times(1);
    
    // Act
    gateways::UserStorageGateway userStorageGateway(&userStorageAccessMock);
    userStorageGateway.createUser(registerModel);
}