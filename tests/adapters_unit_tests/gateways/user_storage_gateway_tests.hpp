#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;


namespace tests::adapters
{

class UserStorageAccessMock : public IUserStorageAccess
{
public:
    MOCK_METHOD(void, getUser, (const QString& authToken), (override));
    MOCK_METHOD(void, changeFirstName, (const QString& authToken,
                                        const QString& newFirstName), (override));
    MOCK_METHOD(void, changeLastName, (const QString& authToken,
                                       const QString& newFirstName), (override));
    MOCK_METHOD(void, changeEmail, (const QString& authToken,
                                    const QString& newFirstName), (override));
};


struct AUserStorageGateway : public testing::Test
{
    AUserStorageGateway() {}
    
    void SetUp() override
    {
        userStorageGateway = std::make_unique<UserStorageGateway>(&userStorageAccessMock);
    }
    
    UserStorageAccessMock userStorageAccessMock;
    std::unique_ptr<UserStorageGateway> userStorageGateway;
};



TEST_F(AUserStorageGateway, SucceedsGettingAUser)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, getUser(_))
            .Times(1);
    
    // Act
    userStorageGateway->getUser("secureToken");
}


TEST_F(AUserStorageGateway, SucceedsChangingTheFirstName)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeFirstName(_,_))
            .Times(1);
    
    // Act
    userStorageGateway->changeFirstName("secureToken", "SomeFirstName");
}


TEST_F(AUserStorageGateway, SucceedsChangingTheLastName)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeLastName(_,_))
            .Times(1);
    
    // Act
    userStorageGateway->changeLastName("secureToken", "SomeLastName");
}


TEST_F(AUserStorageGateway, SucceedsChangingTheEmail)
{
    // Expect
    EXPECT_CALL(userStorageAccessMock, changeEmail(_,_))
            .Times(1);
    
    // Act
    userStorageGateway->changeEmail("secureToken", "SomeEmail");
}

} // namespace tests::adapters