#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
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
    MOCK_METHOD(void, authenticateUser, (const dtos::LoginDto&), (override));
    MOCK_METHOD(void, registerUser, (const dtos::RegisterDto&), (override));
    MOCK_METHOD(void, checkIfEmailConfirmed, (const QString&), (override));
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
    value_objects::LoginModel loginModel("someEmail@librum.com",
                                         "SomePassword123", false);


    // Expect
    EXPECT_CALL(authAccessMock, authenticateUser(_)).Times(1);

    // Act
    authGateway->authenticateUser(loginModel);
}

TEST_F(AnAuthenticationGateway, SucceedsRegisteringAUser)
{
    // Arrange
    value_objects::RegisterModel registerModel(
        "John", "Doe", "someEmail@librum.com", "SomePassword123", false);


    // Expect
    EXPECT_CALL(authAccessMock, registerUser(_)).Times(1);

    // Act
    authGateway->registerUser(registerModel);
}

}  // namespace tests::adapters