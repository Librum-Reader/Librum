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


struct AnAuthenticationGateway : public ::testing::Test
{
    void SetUp() override
    {
        authGateway = std::make_unique<AuthenticationGateway>(&authAccessMock);
    }
    
    AuthenticationAccessMock authAccessMock;
    std::unique_ptr<AuthenticationGateway> authGateway;
};




TEST_F(AnAuthenticationGateway, SucceedsAuthenticatingAUser)
{
    // Arrange
    models::LoginModel loginModel("someEmail@librum.com", "SomePassword123");
    
    
    // Expect
    EXPECT_CALL(authAccessMock, authenticateUser(_))
            .Times(1);
    
    // Act
    authGateway->authenticateUser(loginModel);
}


TEST_F(AnAuthenticationGateway, SucceedsRegisteringAUser)
{
    // Arrange
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com", 
                                     "SomePassword123", false);
    
    
    // Expect
    EXPECT_CALL(authAccessMock, registerUser(_))
            .Times(1);
    
    // Act
    authGateway->registerUser(registerModel);
}



TEST_F(AnAuthenticationGateway, SucceedsReemittingAuthenticationResultSignal)
{
    // Arrange
    QSignalSpy spy(authGateway.get(), 
                   SIGNAL(authenticationFinished(const QString&)));
    
    QString token("someToken");
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    authGateway->reemitAuthenticationResult(token);
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(token, arguments[0].toString());
}


TEST_F(AnAuthenticationGateway, SucceedsReemittingRegistrationResultSignal)
{
    // Arrange
    QSignalSpy spy(authGateway.get(),
                   SIGNAL(registrationFinished(bool, const QString&)));
    
    models::RegisterModel registerModel("John", "Doe", "someEmail@librum.com",
                                     "SomePassword123", false);
    
    
    // Act
    authGateway->reemitRegistrationResult(true, "some token");
    
    // Assert
    auto arguments = spy[0];
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(true, arguments[0].toBool());
}

} // namespace tests::adapters