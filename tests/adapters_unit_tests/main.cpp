#include "gtest/gtest.h"
#include "authentication_controller_tests.hpp"
#include "user_storage_gateway_tests.hpp"


TEST(test, test)
{
    ASSERT_EQ(1,1);
}


int main()
{
    testing::InitGoogleTest();
    
    
    return RUN_ALL_TESTS();
}