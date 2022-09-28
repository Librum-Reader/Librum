#include "gtest/gtest.h"
#include "book_service_tests.hpp"
#include "authentication_service_tests.hpp"
#include "book_info_helper_tests.hpp"


int main()
{
    testing::InitGoogleTest();
       
    
    return RUN_ALL_TESTS();
}