#include "gtest/gtest.h"
#include "book_service_tests.hpp"
#include "user_service_tests.hpp"
#include "authentication_service_tests.hpp"
#include "downloaded_books_tracker_tests.hpp"


int main()
{
    testing::InitGoogleTest();
       
    
    return RUN_ALL_TESTS();
}