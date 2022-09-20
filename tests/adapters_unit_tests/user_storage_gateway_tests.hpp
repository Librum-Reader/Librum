#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QSignalSpy>
#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "login_model.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;
using namespace domain;



class UserStorageMock : public adapters::IUserStorageAccess
{
public:
    MOCK_METHOD(void, authenticateUser, (const dtos::LoginDto& loginDto), (override));
    MOCK_METHOD(void, registerUser, (const dtos::RegisterDto& registerDto), (override));
};




TEST(AUserStorageGateway, SucceedsAuthenticatingAUser)
{
    // Arrange
    UserStorageMock userStorageMock;
    UserStorageGateway userStorageGateway(&userStorageMock);
    
    models::LoginModel loginModel("someEmail@librum.com", "SomePassword123");
    
    
    // Expect
    EXPECT_CALL(userStorageMock, authenticateUser(_))
            .Times(1);
    
    // Act
    userStorageGateway.authenticateUser(loginModel);
}


TEST(AUserStorageGateway, SucceedsRegisteringAUser)
{
    // Arrange
    UserStorageMock userStorageMock;
    UserStorageGateway userStorageGateway(&userStorageMock);
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com", 
                                     "SomePassword123", false);
    
    
    // Expect
    EXPECT_CALL(userStorageMock, registerUser(_))
            .Times(1);
    
    // Act
    userStorageGateway.registerUser(registerModel);
}



TEST(AUserStorageGateway, SucceedsReemittingAuthenticationResultSignal)
{
    // Arrange
    UserStorageMock userStorageMock;
    UserStorageGateway userStorageGateway(&userStorageMock);
    
    QSignalSpy spy(&userStorageGateway,
                   SIGNAL(authenticationFinished(const QString&)));
    
    QString token("someToken");
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    userStorageGateway.reemitAuthenticationResult(token);
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(token, arguments[0].toString());
}


TEST(AUserStorageGateway, SucceedsReemittingRegistrationResultSignal)
{
    // Arrange
    UserStorageMock userStorageMock;
    UserStorageGateway userStorageGateway(&userStorageMock);
    
    QSignalSpy spy(&userStorageGateway,
                   SIGNAL(registrationFinished(bool, const QString&)));
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    userStorageGateway.reemitRegistrationResult(true, "some token");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}