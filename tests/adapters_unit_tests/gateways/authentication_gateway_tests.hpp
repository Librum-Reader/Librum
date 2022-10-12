#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QSignalSpy>
#include "authentication_gateway.hpp"
#include "i_authentication_access.hpp"
#include "login_model.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;
using namespace domain;


namespace tests::adapters
{

class AuthenticationAccessMock : public IAuthenticationAccess
{
public:
    MOCK_METHOD(void, authenticateUser, (const dtos::LoginDto& loginDto), (override));
    MOCK_METHOD(void, registerUser, (const dtos::RegisterDto& registerDto), (override));
};




TEST(AnAuthenticationGateway, SucceedsAuthenticatingAUser)
{
    // Arrange
    AuthenticationAccessMock authenticationAccessMock;
    AuthenticationGateway AuthenticationGateway(&authenticationAccessMock);
    
    models::LoginModel loginModel("someEmail@librum.com", "SomePassword123");
    
    
    // Expect
    EXPECT_CALL(authenticationAccessMock, authenticateUser(_))
            .Times(1);
    
    // Act
    AuthenticationGateway.authenticateUser(loginModel);
}


TEST(AnAuthenticationGateway, SucceedsRegisteringAUser)
{
    // Arrange
    AuthenticationAccessMock authenticationAccessMock;
    AuthenticationGateway AuthenticationGateway(&authenticationAccessMock);
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com", 
                                     "SomePassword123", false);
    
    
    // Expect
    EXPECT_CALL(authenticationAccessMock, registerUser(_))
            .Times(1);
    
    // Act
    AuthenticationGateway.registerUser(registerModel);
}



TEST(AnAuthenticationGateway, SucceedsReemittingAuthenticationResultSignal)
{
    // Arrange
    AuthenticationAccessMock authenticationAccessMock;
    AuthenticationGateway AuthenticationGateway(&authenticationAccessMock);
    
    QSignalSpy spy(&AuthenticationGateway,
                   SIGNAL(authenticationFinished(const QString&)));
    
    QString token("someToken");
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    AuthenticationGateway.reemitAuthenticationResult(token);
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(token, arguments[0].toString());
}


TEST(AnAuthenticationGateway, SucceedsReemittingRegistrationResultSignal)
{
    // Arrange
    AuthenticationAccessMock authenticationAccessMock;
    AuthenticationGateway AuthenticationGateway(&authenticationAccessMock);
    
    QSignalSpy spy(&AuthenticationGateway,
                   SIGNAL(registrationFinished(bool, const QString&)));
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    AuthenticationGateway.reemitRegistrationResult(true, "some token");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

} // namespace tests::adapters