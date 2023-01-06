#include <gtest/gtest.h>
#include "book_tests.hpp"
#include "login_model_tests.hpp"
#include "register_model_tests.hpp"
#include "tag_tests.hpp"
#include "user_tests.hpp"

int main()
{
    testing::InitGoogleTest();


    return RUN_ALL_TESTS();
}